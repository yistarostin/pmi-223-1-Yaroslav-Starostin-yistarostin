#include "search.h"

#include <cctype>
#include <cmath>
#include <functional>
#include <unordered_map>
#include <unordered_set>

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

std::unordered_map<std::string_view, size_t> GenerateIDF(const std::vector<std::string_view>& tokenized_by_lines,
                                                         const std::unordered_set<std::string_view>& query_words) {
    std::unordered_map<std::string_view, size_t> idf;  // word -> IDF(word)
    for (std::string_view line : tokenized_by_lines) {
        std::unordered_set<std::string_view> in_current_line;
        for (std::string_view word : Tokenize(line, [](char c) { return !std::isalpha(c); })) {
            if (query_words.contains(word)) {
                if (in_current_line.count(word) == 0) {
                    in_current_line.insert(word);
                    ++idf[word];
                }
            }
        }
    }
    return idf;
}

std::unordered_set<std::string_view> GetInterestingLines(const std::vector<std::string_view>& tokenized_by_lines,
                                                         const std::vector<std::string_view>& tokenized_query) {
    std::unordered_set<std::string_view> interesting_lines;
    const std::unordered_set<std::string_view> words_bag(tokenized_query.begin(), tokenized_query.end());
    for (std::string_view line : tokenized_by_lines) {
        auto tokenized_line = Tokenize(line, [](char c) { return !std::isalpha(c); });
        if (std::any_of(tokenized_line.begin(), tokenized_line.end(),
                        [&words_bag](std::string_view word) { return words_bag.contains(word); })) {
            interesting_lines.insert(line);  // TODO: what if 2 lines and be equal ??? 😡😡
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
    const auto tokenized_query{Tokenize(query, [](char c) { return !std::isalpha(c); })};
    const auto query_words = std::unordered_set<std::string_view>(tokenized_query.begin(), tokenized_query.end());
    const auto tokenized_by_lines{Tokenize(text, iscntrl)};
    auto idf{GenerateIDF(tokenized_by_lines, query_words)};
    auto interesting_lines{GetInterestingLines(tokenized_by_lines, tokenized_query)};
    if (interesting_lines.empty()) {  // there are no lines with positive TF_IDF
        return {};
    }
    std::unordered_map<std::string_view, long double> tf_idf_per_line;
    tf_idf_per_line.reserve(interesting_lines.size());
    for (auto line : interesting_lines) {
        long double s = 0;
        for (std::string_view word : tokenized_query) {
            auto word_idf = static_cast<long double>(idf[word]) / static_cast<long double>(tokenized_by_lines.size());
            auto reverse_idf = std::log(1 / word_idf);
            auto tf = GetLineTF(line, word);
            s += reverse_idf * tf;
        }
        tf_idf_per_line[line] = s;
    }
    auto [line, tf_idf] = *max_element(tf_idf_per_line.begin(), tf_idf_per_line.end(),
                                       []<typename Pair>(const Pair& a, const Pair& b) { return a.second < b.second; });
    return {line};
}
