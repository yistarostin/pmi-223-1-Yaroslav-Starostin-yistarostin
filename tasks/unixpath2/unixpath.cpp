#include "unixpath.h"

#include <endian.h>

#include <initializer_list>

constexpr const char UnixDelimiter = '/';
constexpr const char* const GotoPrevDirectory = "..";
constexpr const char* const GotoCurrDirectory = ".";

UnixPath::UnixPath(std::string_view initial_dir) : initial_dir_(initial_dir.begin(), initial_dir.end()) {
}

std::string UnixPath::GetAbsolutePath() const {
    // TODO: use caching
    //       we actually can't mark this method const if we wonna save cache
    //       so we either need to get rid of const, to calculate hash just after the changing the directory or just not
    //       to use cache
    //
    std::string path_to_current;
    for (const auto& directory : current_location_) {
        path_to_current += UnixDelimiter;
        path_to_current += directory;
    }
    if (path_to_current.empty()) {
        path_to_current += UnixDelimiter;
    }
    return path_to_current;
}

std::vector<std::string_view> Split(std::string_view str, char delimeter) {
    std::vector<std::string_view> splitted_str;
    auto l = str.begin();
    for (auto r = str.begin(); r != str.end(); ++r) {
        if (*r == delimeter) {
            if (l != r) {
                splitted_str.emplace_back(l, r);
            }
            l = r + 1;
        }
    }
    if (l != str.end()) {
        splitted_str.emplace_back(l, str.end());
    }
    return splitted_str;
}

void UnixPath::ChangeDirectory(std::string_view path) {
    for (const auto& word : Split(path, UnixDelimiter)) {
        std::string_view now = std::string_view(word.begin(), word.end());
        if (now == GotoPrevDirectory) {
            if (!current_location_.empty()) {  // if we are not already in /
                current_location_.pop_back();
            }
        } else if (!now.empty() && now != GotoCurrDirectory) {
            current_location_.emplace_back(now);
        }
    }
}
