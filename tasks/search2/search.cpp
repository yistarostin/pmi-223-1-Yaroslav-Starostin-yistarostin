#include "search.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <functional>
#include <string_view>
#include <unordered_set>

constexpr const size_t HasherBase = 29;  // least prime larger than 26 (|alphabet|)

std::size_t InsensitivePolynomialHash::operator()(std::string_view s) const {
    std::size_t hash{1};
    for (char c : s) {
        hash = hash * HasherBase +
               (std::tolower(c) - 'a' + 1);  // overflow is ok as size_t is used, it just makes x -> x MOD 2**32
    }
    return hash;
}

std::vector<std::string_view> Tokenize(std::string_view str, std::function<bool(char)> fn) {
    std::vector<std::string_view> splitted_str;
    auto l = str.begin();
    for (auto r = str.begin(); r != str.end(); ++r) {
        if (fn(*r)) {
            if (l != r) {
                splitted_str.emplace_back(l, r);
            }
            l = r + 1;
        }
    }
    if (l != str.end()) {
        splitted_str.emplace_back(l, str.end());
    }
    return splitted_str;
}

bool operator<(std::string_view a, std::string_view b) {
    for (std::size_t i = 0; i < std::min<size_t>(a.size(), b.size()); ++i) {
        if (std::tolower(a[i]) != std::tolower(b[i])) {
            return std::tolower(a[i]) < std::tolower(b[i]);
        }
    }
    return a.size() < b.size();
    // auto [left_It, right_It] = std::mismatch(a.begin(), a.end(), b.begin(), b.end(),
    // [](char a, char b) { return std::tolower(a) == std::tolower(b); });
    // return right_It != b.end() && (left_It == a.end() || (*left_It < *right_It));
}

bool operator==(std::string_view a, std::string_view b) {
    if (a.size() != b.size()) {
        return false;
    }
    return std::equal(a.begin(), a.end(), b.begin(), b.end(),
                      [](char a, char b) { return std::tolower(a) == std::tolower(b); });
}

auto TokenizeToWords(std::string_view line) {
    return Tokenize(line, [](char c) { return !std::isalpha(c); });
}

InsensitiveHashMap GenerateIDF(const std::vector<std::vector<std::string_view>>& tokenized_by_words,
                               const InsensitiveHashSet& query_words) {
    InsensitiveHashMap idf;  // word -> IDF(word)
    for (const std::vector<std::string_view>& line : tokenized_by_words) {
        InsensitiveHashSet in_current_line;
        for (std::string_view word : line) {
            if (query_words.count(word)) {
                if (in_current_line.count(word) == 0) {
                    in_current_line.insert(word);
                    ++idf[word];
                }
            }
        }
    }
    return idf;
}

struct LineMetric {
    long double metrics_value;
    size_t indexInOriginalText;

    LineMetric(long double mv, size_t index) : metrics_value(mv), indexInOriginalText(index) {
    }

    auto operator<(const LineMetric& other) const {
        if (metrics_value != other.metrics_value) {
            return metrics_value > other.metrics_value;
        }
        return indexInOriginalText < other.indexInOriginalText;
    }
};

auto GetInterestingLines(const std::vector<std::vector<std::string_view>>& tokenized_by_words,
                         const InsensitiveHashSet& words_bag) {
    std::vector<size_t> interesting_lines;
    for (size_t line_index_in_text = 0; const std::vector<std::string_view>& line : tokenized_by_words) {
        // If line has at least 1 word from query
        if (std::any_of(line.begin(), line.end(),
                        [&words_bag](std::string_view word) { return words_bag.contains(word); })) {
            interesting_lines.push_back(line_index_in_text);
        }
        ++line_index_in_text;
    }
    return interesting_lines;
}

void SearchEngine::BuildIndex(std::string_view text) {
    tokenized_by_lines_ = Tokenize(text, [](char c) { return iscntrl(c); });
    tokenized_by_words_.clear();
    tokenized_by_words_.reserve(tokenized_by_lines_.size());
    for (auto line : tokenized_by_lines_) {
        tokenized_by_words_.push_back(TokenizeToWords(line));
    }
    InsensitiveHashSet query;
    for (const auto& line : tokenized_by_words_) {
        for (const auto& word : line) {
            query.insert(word);
        }
    }
    idf_values_ = GenerateIDF(tokenized_by_words_, query);
}

long double GetLineTF(const std::vector<std::string_view>& line, std::string_view target_word) {
    std::size_t total_words = line.size();
    std::size_t matched_words = std::count(line.begin(), line.end(), target_word);
    return static_cast<long double>(matched_words) / static_cast<long double>(total_words);
}

std::vector<std::string_view> SearchEngine::Search(std::string_view query, size_t results_count) const {
    if (idf_values_.empty()) {
        return {};
    }
    std::vector<std::string_view> tokenized_query{TokenizeToWords(query)};
    InsensitiveHashSet words_bag{tokenized_query.begin(), tokenized_query.end()};
    tokenized_query.erase(std::unique(tokenized_query.begin(), tokenized_query.end()), tokenized_query.end());
    auto lines_containing_query_words{GetInterestingLines(tokenized_by_words_, words_bag)};
    if (lines_containing_query_words.empty()) {
        return {};
    }

    std::vector<LineMetric> lines_matching_query;
    lines_matching_query.reserve(lines_containing_query_words.size());
    for (auto index : lines_containing_query_words) {
        const std::vector<std::string_view>& line = tokenized_by_words_[index];
        long double line_metrics_sum = 0;
        for (std::string_view word : words_bag) {
            auto word_idf = static_cast<long double>(idf_values_.contains(word) ? idf_values_.at(word) : 0) /
                            static_cast<long double>(tokenized_by_lines_.size());
            auto reverse_idf = idf_values_.contains(word) ? -std::log(word_idf) : 0;
            auto tf = GetLineTF(line, word);
            line_metrics_sum += reverse_idf * tf;
        }
        if (line_metrics_sum > 0) {
            lines_matching_query.emplace_back(line_metrics_sum, index);
        }
    }
    std::stable_sort(lines_matching_query.begin(), lines_matching_query.end());
    size_t result_length{std::min<size_t>(results_count, lines_matching_query.size())};
    std::vector<std::string_view> search_result(result_length);
    for (size_t i = 0; i < result_length; ++i) {
        search_result[i] = tokenized_by_lines_[lines_matching_query[i].indexInOriginalText];
    }
    return search_result;
}
