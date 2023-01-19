#include "associative_operation.h"
bool IsAssociative(const std::vector<std::vector<std::size_t>>& table) {
    for (std::size_t x = 0; x < table.size(); ++x) {
        for (std::size_t y = 0; y < table.size(); ++y) {
            for (std::size_t z = 0; z < table.size(); ++z) {
                if (table[table[x][y]][z] != table[x][table[x][y]]) {
                    return false;
                }
            }
        }
    }
    return true;
}
