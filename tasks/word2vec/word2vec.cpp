#include "word2vec.h"

#include <algorithm>
#include <cmath>
#include <vector>

int64_t ComputeWordsDistance(const std::vector<int>& lhs, const std::vector<int>& rhs) {
    int64_t result{0};
    for (size_t i = 0; i < lhs.size(); ++i) {
        result += std::abs(static_cast<int64_t>(lhs[i]) * rhs[i]);
    }
    return result;
}

std::vector<std::string> FindClosestWords(const std::vector<std::string>& words,
                                          const std::vector<std::vector<int>>& vectors) {
    if (words.size() < 2) {
        return {};
    }
    auto& target_word_vector = vectors[0];
    auto closest_word_iterator = std::max_element(
        std::next(vectors.begin()), vectors.end(), [&](const std::vector<int>& old, const std::vector<int>& current) {
            return ComputeWordsDistance(vectors[0], old) < ComputeWordsDistance(vectors[0], current);
        });
    auto target_distance = ComputeWordsDistance(target_word_vector, *closest_word_iterator);
    std::vector<std::string> closest_to_first;
    for (size_t i = 1; i < vectors.size(); ++i) {
        if (ComputeWordsDistance(target_word_vector, vectors[i]) == target_distance) {
            closest_to_first.push_back(words[i]);
        }
    }
    return closest_to_first;
}
