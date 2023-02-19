#include "unixpath.h"

#include <algorithm>
#include <cstddef>

constexpr const char UnixDelimiter = '/';
constexpr const char* const GotoPrevDirectory = "..";
constexpr const char* const GotoCurrDirectory = ".";

std::vector<std::string> Split(std::string_view str, char delimeter) {
    std::vector<std::string> splitted_str;
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

std::string ConstructPath(const std::vector<std::string>& parsed_path,
                          bool absolute = true) {  // I might want to use std::string_view
                                                   // instead of string, but not a big deal
    std::string path_to_current;
    for (const auto& directory : parsed_path) {
        path_to_current += UnixDelimiter;
        path_to_current += directory;
    }
    if (path_to_current.empty()) {
        path_to_current += UnixDelimiter;
    }
    return {std::next(path_to_current.begin(), (absolute ? 0 : 1)), path_to_current.end()};
}

UnixPath::UnixPath(std::string_view initial_dir)
    : initial_dir_(initial_dir.begin(), initial_dir.end()),
      current_location_(),
      absolute_to_initial_(Split(initial_dir, UnixDelimiter)) {
    ChangeDirectory(initial_dir_);
}

std::string UnixPath::GetAbsolutePath() const {
    return ConstructPath(current_location_);
}

void UnixPath::ChangeDirectory(std::string_view path) {
    if (!path.empty() && path[0] == UnixDelimiter) {
        current_location_.clear();
    }
    for (const auto& word : Split(path, UnixDelimiter)) {
        std::string_view now{word.begin(), word.end()};
        if (now == GotoPrevDirectory) {
            if (!current_location_.empty()) {  // if we are not already in /
                current_location_.pop_back();
            }
        } else if (!now.empty() && now != GotoCurrDirectory) {
            current_location_.emplace_back(now);
        }
    }
}

std::string UnixPath::GetRelativePath() const {
    std::vector<std::string> relative_path;
    auto first_path_mismatch = std::mismatch(current_location_.begin(), current_location_.end(),
                                             absolute_to_initial_.begin(), absolute_to_initial_.end());
    std::ptrdiff_t have_walked = first_path_mismatch.second - absolute_to_initial_.begin();

    for (std::ptrdiff_t i = 0; std::next(absolute_to_initial_.begin(), have_walked + i) != absolute_to_initial_.end();
         ++i) {  // just N times add ".." to relative_path
        relative_path.push_back(GotoPrevDirectory);
    }
    if (relative_path.empty()) {
        relative_path.push_back(GotoCurrDirectory);
    }
    std::copy(std::next(current_location_.begin(), have_walked), current_location_.end(),
              std::back_inserter(relative_path));
    return ConstructPath(relative_path, false);
}
