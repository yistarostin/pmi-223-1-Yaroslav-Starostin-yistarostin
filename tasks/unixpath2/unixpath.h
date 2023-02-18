#pragma once

#include <string>
#include <string_view>
#include <stack>
#include <vector>

class UnixPath {
    std::string initial_dir_;
    std::vector<std::string> current_location_;
    bool is_cache_valid_;
    std::string cache_;
public:
    explicit UnixPath(std::string_view initial_dir);

    void ChangeDirectory(std::string_view path);

    std::string GetAbsolutePath() const;
    std::string GetRelativePath() const;
};
