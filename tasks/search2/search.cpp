#include "search.h"

#include <cctype>
#include <cmath>
#include <functional>
#include <string>
#include <string_view>
#include <unordered_set>

constexpr const size_t HasherBase = 29;  // least prime larger than 26 (|alphabet|)

std::size_t InsensitivePolynomialHash::operator()(std::string_view s) const {
    std::size_t hash{0};
    for (char c : s) {
        hash = hash * HasherBase +
               (std::tolower(c) - 'a' + 1);  // overflow is ok as size_t is used, it just makes x -> x MOD 2**32
    }
    return hash;
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

template <typename F>
std::vector<std::string_view> Tokenize(std::string_view str, F fn) {
    std::vector<std::string_view> splitted_str;
    auto l = str.begin();
    for (auto r = str.begin(); r != str.end(); ++r) {
        if (fn(*r)) {
            if (l != r) {
                splitted_str.emplace_back(std::string_view{l, r});
            }
            l = r + 1;
        }
    }
    if (l != str.end()) {
        splitted_str.emplace_back(std::string_view{l, str.end()});
    }
    return splitted_str;
}

bool CheckStringsEqualityIgnoringCase(std::string_view a, std::string_view b) {
    if (a.size() != b.size()) {
        return false;
    }
    for (size_t i = 0; i < a.size(); ++i) {
        if (std::tolower(a[i]) != std::tolower(b[i])) {
            return false;
        }
    }
    return true;
}

auto TokenizeToWords(std::string_view line) {
    return Tokenize(line, [](char c) { return !std::isalpha(c); });
}

InsensitiveHashMap GenerateIDF(const std::vector<std::string_view>& tokenized_by_lines,
                               const InsensitiveHashSet& query_words) {
    InsensitiveHashMap idf;  // word -> IDF(word)
    for (std::string_view line : tokenized_by_lines) {
        InsensitiveHashSet in_current_line;
        for (std::string_view word : TokenizeToWords(line)) {
            if (query_words.count(word)) {
                if (!in_current_line.count(word)) {
                    std::string copy;
                    std::transform(word.begin(), word.end(), std::back_inserter(copy), ::tolower);
                    in_current_line.insert(copy);
                    ++idf[word];
                }
            }
        }
    }
    return idf;
}

long double SearchEngine::GetLineTF(std::vector<std::string_view> line, std::string_view target_word) const {
    std::size_t total_words = line.size();
    std::size_t matched_words = 0;
    for (const std::string_view& word : line) {
        if (CheckStringsEqualityIgnoringCase(word, target_word)) {
            ++matched_words;
        }
    }
    return static_cast<long double>(matched_words) / static_cast<long double>(total_words);
}

void SearchEngine::BuildIndex(std::string_view text) {
    tokenized_by_lines_ = Tokenize(text, [](char c) -> bool { return iscntrl(c); });
    tokenized_by_words_.clear();
    for (auto line : tokenized_by_lines_) {
        tokenized_by_words_.emplace_back(TokenizeToWords(line));
    }

    has_index_ = true;
}

auto GetInterestingLines(const std::vector<std::string_view>& tokenized_by_lines, const InsensitiveHashSet& words_bag) {
    std::vector<size_t> interesting_lines;
    for (size_t line_index_in_text = 0; std::string_view line : tokenized_by_lines) {
        auto tokenized_line = TokenizeToWords(line);
        if (std::any_of(tokenized_line.begin(), tokenized_line.end(),
                        [&words_bag](std::string_view word) { return words_bag.count(word); })) {
            interesting_lines.push_back(line_index_in_text);
        }
        ++line_index_in_text;
    }
    return interesting_lines;
}

std::vector<std::string_view> SearchEngine::Search(std::string_view query, size_t results_count) const {
    if (!has_index_) {
        return {};
    }
    InsensitiveHashMap words_count;  // word -> text.count(word)
    auto tokenized_query{TokenizeToWords(query)};
    std::sort(tokenized_query.begin(), tokenized_query.end());
    tokenized_query.erase(std::unique(tokenized_query.begin(), tokenized_query.end()), tokenized_query.end());
    InsensitiveHashSet query_words(tokenized_query.begin(), tokenized_query.end());
    auto idf{GenerateIDF(tokenized_by_lines_, query_words)};
    auto lines_containing_query_words{GetInterestingLines(tokenized_by_lines_, query_words)};
    if (lines_containing_query_words.empty()) {  // there are no lines with positive TF_IDF
        return {};
    }
    std::vector<LineMetric> lines_matching_query;
    lines_matching_query.reserve(lines_containing_query_words.size());
    for (auto index : lines_containing_query_words) {
        long double s = 0;
        for (std::string_view word : query_words) {
            auto word_idf = static_cast<long double>(idf[word]) / static_cast<long double>(tokenized_by_lines_.size());
            auto reverse_idf = idf[word] == 0 ? 0 : -std::log(word_idf);
            auto tf = GetLineTF(tokenized_by_words_[index], word);
            s += reverse_idf * tf;
        }
        if (s > 0) {
            lines_matching_query.emplace_back(s, index);
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
