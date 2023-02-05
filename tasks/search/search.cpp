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

bool CompareStringIgnoringCase(std::string_view a, std::string_view b) {
    if (a.size() != b.size()) {
        return false;
    }
    for (size_t i = 0; i < a.size(); ++i) {
        if (std::isalpha(a[i])) {
            if (!(isalpha(b[i]) && tolower(a[i]) == tolower(b[i]))) {
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

std::unordered_map<std::string_view, size_t> GenerateIDF(const std::vector<std::string_view>& tokenized_by_lines,
                                                         const std::unordered_set<std::string_view>& query_words) {
    std::unordered_map<std::string_view, size_t> idf;  // word -> IDF(word)
    for (std::string_view line : tokenized_by_lines) {
        std::unordered_set<std::string> in_current_line;  // TODO: put strings in lower case
        for (std::string_view word : Tokenize(line, [](char c) { return !std::isalpha(c); })) {
            if (std::any_of(query_words.begin(), query_words.end(),
                            [word](std::string_view token) { return CompareStringIgnoringCase(word, token); })) {
                if (!std::any_of(in_current_line.begin(), in_current_line.end(),
                                 [word](std::string_view token) { return CompareStringIgnoringCase(word, token); })) {
                    std::string copy;
                    for (char c : word) {
                        copy.push_back(static_cast<char>(tolower(c)));
                    }
                    in_current_line.insert(copy);
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
    std::string_view line_content;

    auto operator<(const LineMetric& other) const {
        if (metrics_value != other.metrics_value) {
            return metrics_value > other.metrics_value;
        }
        return indexInOriginalText < other.indexInOriginalText;
    }
};

auto GetInterestingLines(const std::vector<std::string_view>& tokenized_by_lines,
                         const std::vector<std::string_view>& tokenized_query) {
    std::vector<std::pair<std::string_view, size_t>> interesting_lines;
    const std::unordered_set<std::string_view> words_bag(tokenized_query.begin(), tokenized_query.end());
    for (size_t i = 0; std::string_view line : tokenized_by_lines) {
        auto tokenized_line = Tokenize(line, [](char c) { return !std::isalpha(c); });
        if (std::any_of(tokenized_line.begin(), tokenized_line.end(),
                        [&words_bag](std::string_view word) { return words_bag.contains(word); })) {
            interesting_lines.emplace_back(line, i++);  // TODO: what if 2 lines and be equal ??? 😡😡
        }
    }
    return interesting_lines;
}

long double GetLineTF(std::string_view line, std::string_view target_word) {
    auto tokenized_line = Tokenize(line, [](char c) { return !std::isalpha(c); });
    std::size_t total_words = tokenized_line.size();
    std::size_t matched_words = 0;
    for (std::string_view word : tokenized_line) {
        if (target_word == word) {
            ++matched_words;
        }
    }
    return static_cast<double>(matched_words) / static_cast<double>(total_words);
}

std::vector<std::string_view> Search(std::string_view text, std::string_view query, size_t results_count) {
    std::unordered_map<std::string_view, size_t> words_count;  // word -> text.count(word)
    auto tokenized_query{Tokenize(query, [](char c) { return !std::isalpha(c); })};
    std::sort(tokenized_query.begin(), tokenized_query.end());
    tokenized_query.erase(std::unique(tokenized_query.begin(), tokenized_query.end()), tokenized_query.end());
    std::vector<std::string> qq(tokenized_query.size());
    for (size_t i = 0; i < tokenized_query.size(); ++i) {
        std::transform(tokenized_query[i].begin(), tokenized_query[i].end(), std::back_inserter(qq[i]), ::tolower);
    }
    std::unordered_set<std::string_view> query_words;
    for (const std::string& kek : qq) {
        query_words.insert(std::string_view{kek.begin(), kek.end()});
    }
    const auto tokenized_by_lines{Tokenize(text, iscntrl)};
    auto idf{GenerateIDF(tokenized_by_lines, query_words)};
    auto interesting_lines{GetInterestingLines(tokenized_by_lines, tokenized_query)};
    if (interesting_lines.empty()) {  // there are no lines with positive TF_IDF
        return {};
    }
    std::vector<LineMetric> result;
    result.reserve(interesting_lines.size());
    for (auto [line, index] : interesting_lines) {
        long double s = 0;
        for (std::string_view word : query_words) {
            auto word_idf = static_cast<long double>(idf[word]) / static_cast<long double>(tokenized_by_lines.size());
            auto reverse_idf = idf[word] == 0 ? 0 : std::log(1 / word_idf);
            auto tf = GetLineTF(line, word);
            s += reverse_idf * tf;
        }
        if (s > 0) {
            result.push_back(LineMetric{.metrics_value = s, .indexInOriginalText = index, .line_content = line});
        }
    }
    std::sort(result.begin(), result.end());
    std::vector<std::string_view> search_result;
    for (size_t i = 0; i < std::min<size_t>(results_count, result.size()); ++i) {
        search_result.push_back(result[i].line_content);
    }
    return search_result;
}
