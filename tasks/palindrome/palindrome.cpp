#include "palindrome.h"

#include <algorithm>
#include <cctype>

bool IsPalindrome(const std::string& str) {
    std::string without_spaces;
    std::copy_if(str.begin(), str.end(), std::back_inserter(without_spaces), ::isalpha);

    return std::equal(without_spaces.begin(),
                      without_spaces.begin() + static_cast<std::ptrdiff_t>(without_spaces.size() / 2),
                      without_spaces.rbegin());
}