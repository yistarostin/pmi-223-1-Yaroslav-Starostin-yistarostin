#include "password.h"

#include <algorithm>
#include <array>
#include <functional>

constexpr size_t MinPasswordLength{8u};
constexpr size_t MaxPasswordLength{14u};

bool ValidatePassword(const std::string& password) {
    bool are_characters_valid = std::all_of(password.begin(), password.end(), ::isgraph);
    bool is_length_appropriate = MinPasswordLength <= password.length() && password.length() <= MaxPasswordLength;
    std::array<bool, 4> character_classes = {std::any_of(password.begin(), password.end(), ::isdigit),
                                             std::any_of(password.begin(), password.end(), ::islower),
                                             std::any_of(password.begin(), password.end(), ::isupper),
                                             std::any_of(password.begin(), password.end(), ::ispunct)};
    auto has_enought_different_types =
        std::count_if(character_classes.begin(), character_classes.end(), std::identity()) >= 3;
    return is_length_appropriate && are_characters_valid && has_enought_different_types;
}
