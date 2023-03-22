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
    Image Read() const;                    // TODO: return type is fuckined, use std::optional or smth
    bool Write(const Image& image) const;  // TODO: same here
};
