#pragma once
#include <bitset>
#include <cstddef>
#include <ranges>
#include <string>

#include "image.h"

class IO {
    std::string path_;

public:
    explicit IO(const std::string& path);
    Image Read() const;
    bool Write(const Image& image) const;
};
