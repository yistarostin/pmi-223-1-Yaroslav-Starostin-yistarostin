#include "minesweeper.h"

#include <algorithm>
#include <ctime>
#include <iostream>
#include <numeric>
#include <queue>
#include <random>
using Cell = Minesweeper::Cell;

bool Cell::operator==(const Cell& other) const {
    return x == other.x && y == other.y;
}

std::size_t Cell::CellHash::operator()(const Cell& cell) const {
    return cell.x + cell.y;
}

bool Cell::IsValid(size_t width, size_t height) const {
    return 0 <= x && x < width && 0 <= y && y < height;
}

std::array<Cell, NeightbourCount> Cell::GetNeighbours() const {
    std::array<Cell, NeightbourCount> neighbours;
    for (int neighbour_number = 0, dx = -1; dx < 2; ++dx) {
        for (int dy = -1; dy < 2; ++dy) {
            if (dx == 0 and dy == 0) {  // current cell
                continue;
            }
            neighbours[neighbour_number] = Cell{.x = x + dx, .y = y + dy};
            ++neighbour_number;
        }
    }
    return neighbours;
}

Minesweeper::Minesweeper(size_t width, size_t height, size_t mines_count) {
    std::vector<size_t> permutation(width * height);
    std::iota(permutation.begin(), permutation.end(), 0u);
    std::shuffle(permutation.begin(), permutation.end(), std::mt19937(std::random_device()()));
    std::vector<Cell> mined_cells(mines_count);
    for (size_t mine_number = 0; size_t cell_number : permutation) {
        if (mine_number == mines_count) {
            break;
        }
        mined_cells[mine_number] = Cell{.x = cell_number - (cell_number / height * height), .y = cell_number / height};
        ++mine_number;
    }
    (*this) = Minesweeper(width, height, mined_cells);
}

Minesweeper::Minesweeper(size_t width, size_t height, const std::vector<Cell>& cells_with_mines)
    : width_(width), height_(height), field_size_(width * height), start_time_(std::time(nullptr)) {
    mined_cells_.insert(cells_with_mines.begin(), cells_with_mines.end());
    game_status_ = GameStatus::IN_PROGRESS;
}

void Minesweeper::NewGame(size_t width, size_t height, size_t mines_count) {
    auto rhs = Minesweeper(width, height, mines_count);
    (*this) = rhs;
}

void Minesweeper::NewGame(size_t width, size_t height, const std::vector<Cell>& cells_with_mines) {
    (*this) = Minesweeper(width, height, cells_with_mines);
}

void Minesweeper::OpenCell(const Cell& cell) {
    if (game_status_ == GameStatus::VICTORY || game_status_ == GameStatus::DEFEAT) {
        return;
    }
    if (opened_calls_.contains(cell) || flagged_cells_.contains(cell)) {
        return;
    }
    if (mined_cells_.contains(cell)) {
        return Lose();
    }
    // BFS;
    std::queue<Cell> to_open({cell});
    while (!to_open.empty()) {
        Cell current_cell = to_open.front();
        to_open.pop();
        opened_calls_.insert(current_cell);
        auto neighbours = current_cell.GetNeighbours();
        if (std::all_of(neighbours.begin(), neighbours.end(),
                        [&](const Cell& c) { return !mined_cells_.contains(c); })) {
            for (const auto& next : neighbours) {
                if (next.IsValid(width_, height_) && opened_calls_.count(next) + flagged_cells_.count(next) == 0) {
                    to_open.push(next);
                }
            }
        }
        // std::cout << to_open.size() << std::endl;
    }
    if (opened_calls_.size() + mined_cells_.size() == field_size_) {
        Win();
    }
}

void Minesweeper::MarkCell(const Cell& cell) {
    if (game_status_ == GameStatus::VICTORY || game_status_ == GameStatus::DEFEAT) {
        return;
    }
    if (flagged_cells_.contains(cell)) {
        flagged_cells_.erase(cell);
    } else if (!opened_calls_.count(cell)) {
        flagged_cells_.insert(cell);
    }
}

void Minesweeper::Lose() {
    game_status_ = GameStatus::DEFEAT;
    for (size_t w = 0; w < width_; ++w) {
        for (size_t h = 0; h < height_; ++h) {
            opened_calls_.insert(Cell{.x = w, .y = h});
        }
    }
    end_time_ = std::time(nullptr);
}

void Minesweeper::Win() {
    game_status_ = GameStatus::VICTORY;
    end_time_ = std::time(nullptr);
}

auto Minesweeper::GetGameStatus() const -> GameStatus {
    return game_status_;
}

std::time_t Minesweeper::GetGameTime() const {
    if (game_status_ == GameStatus::DEFEAT || game_status_ == GameStatus::VICTORY) {
        return end_time_ - start_time_;
    }
    return std::time(nullptr) - start_time_;
}

auto Minesweeper::RenderField() const -> RenderedField {
    RenderedField rendered_field(height_);
    for (size_t h = 0; h < height_; ++h) {
        std::string current_line;
        current_line.reserve(width_);
        for (size_t w = 0; w < width_; ++w) {
            Cell cell{.x = w, .y = h};
            char cell_ch = '-';
            if (game_status_ == GameStatus::IN_PROGRESS || game_status_ == GameStatus::VICTORY) {
                if (flagged_cells_.contains(cell)) {
                    cell_ch = '?';
                } else if (opened_calls_.contains(cell)) {
                    auto neighbours = cell.GetNeighbours();
                    size_t mines_near = std::count_if(neighbours.begin(), neighbours.end(),
                                                      [&](const Cell& cell) { return mined_cells_.contains(cell); });
                    cell_ch = static_cast<char>('0' + mines_near);
                    if (mines_near == 0) {
                        cell_ch = '.';
                    }
                } else {
                    cell_ch = '-';
                }
            } else {
                if (mined_cells_.contains(cell)) {
                    cell_ch = '*';
                } else {
                    auto neighbours = cell.GetNeighbours();
                    size_t mines_near = std::count_if(neighbours.begin(), neighbours.end(),
                                                      [&](const Cell& cell) { return mined_cells_.contains(cell); });
                    cell_ch = static_cast<char>('0' + mines_near);
                    if (mines_near == 0) {
                        cell_ch = '.';
                    }
                }
            }
            current_line.push_back(cell_ch);
        }
        rendered_field[h] = current_line;
    }
    return rendered_field;
}