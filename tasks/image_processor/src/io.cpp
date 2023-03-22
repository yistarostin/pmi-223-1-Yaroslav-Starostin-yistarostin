
#include "io.h"

IO::IO(const std::string &path) : path_(path) {
}

Image IO::Read() const {
    return Image();
}

bool IO::Write(const Image &image) const {
    return false;
}
