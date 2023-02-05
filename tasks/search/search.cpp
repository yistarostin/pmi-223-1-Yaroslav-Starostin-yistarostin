#include "search.h"

#include <cctype>
#include <functional>
#include <unordered_map>
#include <unordered_set>

template <typename F>
requires std::invocable<F&, int>
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

std::unordered_map<std::string_view, size_t> GenerateIDF(const std::vector<std::string_view>& tokenized_by_lines){
    std::unordered_map<std::string_view, size_t> IDF; // word -> IDS(word)
    for(std::string_view line : tokenized_by_lines){
        std::unordered_set<std::string_view> in_current_line;
        for(std::string_view word : Tokenize(line, [](char c){return !std::isalpha(c);})){
            if(in_current_line.count(word) == 0){
                in_current_line.insert(word);
                ++IDF[word];
            }
        }
    }
    return IDF;
}
std::unordered_set<std::string_view> GetInterestingLines(const std::vector<std::string_view>& tokenized_by_lines, const std::vector<std::string_view>& tokenized_query){
    std::unordered_set<std::string_view> interesting_lines;
    const std::unordered_set<std::string_view> words_bag(tokenized_query.begin(), tokenized_query.end());
    for(std::string_view line: tokenized_by_lines){
        auto tokenized_line = Tokenize(line, [](char c){return !std::isalpha(c);});
        if(std::any_of(tokenized_line.begin(), tokenized_line.end(), [&words_bag](std::string_view word){return words_bag.contains(word);})){
            interesting_lines.insert(line); //TODO: what if 2 lines and be equal ??? 😡😡
        }
    }
    return interesting_lines;
}

long double GetLineTF(std::string_view line, std::string_view target_word){
    std::size_t total_words = line.size();
    std::size_t matched_words = 0;
    for(std::string_view word : Tokenize(line, [](char c){return !std::isalpha(c);})){
        if(target_word == word){
            ++matched_words;
        }
    }
    return static_cast<double>(matched_words) / static_cast<double>(total_words);
}

std::vector<std::string_view> Search(std::string_view text, std::string_view query, size_t results_count) {
    std::unordered_map<std::string_view, size_t> words_count; // word -> text.count(word)
    const auto tokenized_query{Tokenize(query, [](char c){return !std::isalpha(c);})};
    const auto tokenized_by_words{Tokenize(text, [](char c){return !std::isalpha(c);})};
    const auto tokenized_by_lines{Tokenize(text, iscntrl)};
    size_t text_length{tokenized_by_words.size()};
    const auto IDF{GenerateIDF(tokenized_by_lines)};
    const auto interesting_lines{GetInterestingLines(tokenized_by_lines, tokenized_query)};

    return {};
}
