#pragma once

#include <cstddef>  // For std::ptrdiff_t
#include <forward_list>
#include <iostream>
#include <iterator>
#include <sstream>

template <typename Seq1It, typename Seq2It>
class ZipIterator {
    Seq1It it1_;
    Seq2It it2_;

public:
    ZipIterator() = delete;
    ZipIterator(const ZipIterator& other) = default;

    ZipIterator(const Seq1It& seq1_it, const Seq2It& seq2_it) : it1_(seq1_it), it2_(seq2_it) {
    }

    ZipIterator(ZipIterator&& other) = default;
    ZipIterator& operator=(const ZipIterator& other) = default;
    ZipIterator& operator=(ZipIterator&& other) = default;
    ~ZipIterator() = default;

    auto operator*() {
        return make_pair(*it1_, *it2_);
    }

    auto operator++() {
        return make_pair(++it1_, ++it2_);
    }

    bool operator==(const ZipIterator& other) const {
        return it1_ == other.it1_ || it2_ == other.it2_;
    }

    bool operator!=(const ZipIterator& other) const = default;
};

template <typename ZipIterator>
struct ZippedSequnce {
    ZipIterator begin_;
    ZipIterator end_;

    ZipIterator begin() {
        return begin_;
    }

    ZipIterator end() {
        return end_;
    }
};

template <typename Sequence1, typename Sequence2>
auto Zip(const Sequence1& sequence1, const Sequence2& sequence2) {
    auto begin1 = std::begin(sequence1);
    auto begin2 = std::begin(sequence2);
    auto end1 = std::end(sequence1);
    auto end2 = std::end(sequence2);
    auto begin_iterator = ZipIterator<decltype(begin1), decltype(begin2)>{begin1, begin2};
    auto end_iterator = ZipIterator<decltype(end1), decltype(end2)>{end1, end2};
    return ZippedSequnce<decltype(begin_iterator)>{begin_iterator, end_iterator};
}
