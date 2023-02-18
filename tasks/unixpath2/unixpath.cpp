#include "unixpath.h"

#include <endian.h>

#include <initializer_list>


constexpr const char UnixDelimiter = '/';
constexpr const char* const GotoPrevDirectory = "..";
constexpr const char* const GotoCurrDirectory = ".";
UnixPath::UnixPath(std::string_view initial_dir) : initial_dir_(initial_dir.begin(), initial_dir.end()) {
}

std::string UnixPath::GetAbsolutePath() const{
    //TODO: use caching
    //      we actually can't mark this method const if we wonna save cache
    //      so we either need to get rid of const, to calculate hash just after the changing the directory or just not to use cache
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
