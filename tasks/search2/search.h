#pragma once

#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct InsensitivePolynomialHash {
    std::size_t operator()(std::string_view) const;
};

using InsensitiveHashMap = std::unordered_map<std::string_view, std::size_t, InsensitivePolynomialHash>;
using InsensitiveHashSet = std::unordered_set<std::string_view, InsensitivePolynomialHash>;

class SearchEngine {
    // std::unordered_map<std::string_view, std::size_t, InsensitivePolynomialHash> idf_values_;
    std::vector<std::vector<std::string_view>> tokenized_by_words_;
    std::vector<std::string_view> tokenized_by_lines_;
    bool index_exists_;

public:
    void BuildIndex(std::string_view text);
    std::vector<std::string_view> Search(std::string_view query, size_t results_count) const;
};
