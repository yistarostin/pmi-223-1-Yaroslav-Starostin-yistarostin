#include "unixpath.h"

#include <ranges>
#include <vector>

constexpr char UnixDelimiter = '/';
constexpr std::string_view GotoPrevDirectory = "..";
constexpr std::string_view GotoCurrDirectory = ".";

std::vector<std::string_view> Split(std::string_view str, char delimeter){
    std::vector<std::string_view> splitted_str;
    auto l = str.begin();
    for(auto r = str.begin(); r  != str.end(); ++r){
        if(*r == delimeter){
            splitted_str.emplace_back(std::string_view{l, r});
            l = r+1;
        }
    }
    splitted_str.emplace_back(std::string_view{l, str.end()});
    return splitted_str;
}
void WalkThePath(std::string_view path, std::vector<std::string_view>& current_location) {
    for (const auto word : Split(path, UnixDelimiter)) {
        std::string_view now = std::string_view(word.begin(), word.end());
        if (now == GotoPrevDirectory) {
            if (!current_location.empty()) {  // if we are not already in /
                current_location.pop_back();
            }
        } else if (!now.empty() && now != GotoCurrDirectory) {
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
        normalized_path += UnixDelimiter;
        normalized_path += directory;
    }
    if (normalized_path.empty()) {
        normalized_path += UnixDelimiter;
    }
    return normalized_path;
}