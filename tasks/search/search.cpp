#include "search.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <functional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

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
        if (std::isalpha(a[i])) {
            if (!(std::isalpha(b[i]) && std::tolower(a[i]) == std::tolower(b[i]))) {
                return false;
            }
        } else {
            if (a[i] != b[i]) {
                return false;
            }
        }
    }
    return true;
}

struct LineMetric {
    long double metrics_value;
    size_t indexInOriginalText;
    std::string_view line_content;

    LineMetric(long double mv, size_t index, std::string_view line)
        : metrics_value(mv), indexInOriginalText(index), line_content(line) {
    }

    auto operator<(const LineMetric& other) const {
        if (metrics_value != other.metrics_value) {
            return metrics_value > other.metrics_value;
        }
        return indexInOriginalText < other.indexInOriginalText;
    }
};

auto TokenizeToWords(std::string_view line) {
    return Tokenize(line, [](char c) { return !std::isalpha(c); });
}

std::unordered_map<std::string_view, size_t> GenerateIDF(const std::vector<std::string_view>& tokenized_by_lines,
                                                         const std::unordered_set<std::string_view>& query_words) {
    std::unordered_map<std::string_view, size_t> idf;  // word -> IDF(word)
    for (std::string_view line : tokenized_by_lines) {
        std::unordered_set<std::string> in_current_line;  // TODO: put strings in lower case
        for (std::string_view word : TokenizeToWords(line)) {
            if (std::any_of(query_words.begin(), query_words.end(),
                            [word](std::string_view token) { return CheckStringsEqualityIgnoringCase(word, token); })) {
                if (!std::any_of(in_current_line.begin(), in_current_line.end(), [word](std::string_view token) {
                        return CheckStringsEqualityIgnoringCase(word, token);
                    })) {
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

auto GetInterestingLines(const std::vector<std::string_view>& tokenized_by_lines,
                         const std::unordered_set<std::string_view>& words_bag) {
    std::vector<std::pair<std::string_view, size_t>> interesting_lines;
    for (size_t i = 0; std::string_view line : tokenized_by_lines) {
        auto tokenized_line = TokenizeToWords(line);
        if (std::any_of(tokenized_line.begin(), tokenized_line.end(), [&words_bag](std::string_view word) {
                return std::any_of(words_bag.begin(), words_bag.end(), [word](std::string_view token) {
                    return CheckStringsEqualityIgnoringCase(token, word); /*words_bag.contains(word);*/
                });
            })) {
            interesting_lines.emplace_back(line, i++);  // TODO: what if 2 lines and be equal ??? 😡😡
        }
    }
    return interesting_lines;
}

long double GetLineTF(std::string_view line, std::string_view target_word) {
    auto tokenized_line = TokenizeToWords(line);
    std::size_t total_words = tokenized_line.size();
    std::size_t matched_words = 0;
    for (std::string_view word : tokenized_line) {
        if (CheckStringsEqualityIgnoringCase(word, target_word)) {
            ++matched_words;
        }
    }
    return static_cast<long double>(matched_words) / static_cast<long double>(total_words);
}

std::vector<std::string_view> Search(std::string_view text, std::string_view query, size_t results_count) {
    std::unordered_map<std::string_view, size_t> words_count;  // word -> text.count(word)
    auto tokenized_query{TokenizeToWords(query)};
    std::sort(tokenized_query.begin(), tokenized_query.end());
    tokenized_query.erase(std::unique(tokenized_query.begin(), tokenized_query.end()), tokenized_query.end());
    std::vector<std::string> lowered_query(tokenized_query.size());
    for (size_t i = 0; i < tokenized_query.size(); ++i) {
        std::transform(tokenized_query[i].begin(), tokenized_query[i].end(), std::back_inserter(lowered_query[i]),
                       ::tolower);
    }
    std::unordered_set<std::string_view> query_words;
    for (const std::string& normalized_query_item : lowered_query) {
        query_words.insert(std::string_view{normalized_query_item.begin(), normalized_query_item.end()});
    }
    const auto tokenized_by_lines{Tokenize(text, iscntrl)};
    auto idf{GenerateIDF(tokenized_by_lines, query_words)};
    auto interesting_lines{GetInterestingLines(tokenized_by_lines, query_words)};
    if (interesting_lines.empty()) {  // there are no lines with positive TF_IDF
        return {};
    }
    std::vector<LineMetric> result;
    result.reserve(interesting_lines.size());
    for (auto [line, index] : interesting_lines) {
        long double s = 0;
        for (std::string_view word : query_words) {
            auto word_idf = static_cast<long double>(idf[word]) / static_cast<long double>(tokenized_by_lines.size());
            auto reverse_idf = idf[word] == 0 ? 0 : -std::log(word_idf);
            auto tf = GetLineTF(line, word);
            s += reverse_idf * tf;
        }
        if (s > 0) {
            result.emplace_back(s, index, line);
        }
    }
    std::stable_sort(result.begin(), result.end());
    size_t result_length{std::min<size_t>(results_count, result.size())};
    std::vector<std::string_view> search_result(result_length);
    for (size_t i = 0; i < result_length; ++i) {
        search_result[i] = result[i].line_content;
    }
    return search_result;
}
