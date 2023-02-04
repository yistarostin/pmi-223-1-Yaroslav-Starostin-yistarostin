#include "unixpath.h"

#include <ranges>
#include <vector>

constexpr std::string_view UNIX_DELIMITER = "/";
constexpr std::string_view GOTO_PREV_DIRECTORY = "..";
constexpr std::string_view GOTO_CURR_DIRECTORY = ".";

void WalkThePath(std::string_view path, std::vector<std::string_view>& current_location) {
    for (const auto word : std::views::split(path, UNIX_DELIMITER)) {
        std::string_view now = std::string_view(word.begin(), word.end());
        if (now == GOTO_PREV_DIRECTORY) {
            if (!current_location.empty()) {  // if we are not already in /
                current_location.pop_back();
            }
        } else if (!now.empty() && now != GOTO_CURR_DIRECTORY) {
            current_location.push_back(now);
        }
    }
}

std::string NormalizePath(std::string_view current_working_dir, std::string_view local_path) {
    std::vector<std::string_view> current_location;
    WalkThePath(current_working_dir, current_location);
    WalkThePath(local_path, current_location);
    std::string normalized_path;
    for (auto directory : current_location) {
        normalized_path += UNIX_DELIMITER;
        normalized_path += directory;
    }
    if (normalized_path.empty()) {
        normalized_path += UNIX_DELIMITER;
    }
    return normalized_path;
}
