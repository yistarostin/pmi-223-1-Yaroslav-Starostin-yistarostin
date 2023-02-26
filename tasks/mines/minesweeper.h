#pragma once

#include <array>
#include <string>
#include <unordered_set>
#include <vector>
constexpr size_t NeightbourCount = 8;

class Minesweeper {
public:
    struct Cell {
        size_t x = 0;
        size_t y = 0;
        bool operator==(const Cell& other) const;

        struct CellHash {
            std::size_t operator()(const Cell& cell) const;
        };

        bool IsValid(size_t width, size_t height) const;
        std::array<Cell, NeightbourCount> GetNeighbours() const;
    };

    enum class GameStatus {
        NOT_STARTED,
        IN_PROGRESS,
        VICTORY,
        DEFEAT,
    };

    using RenderedField = std::vector<std::string>;
    using CellSet = std::unordered_set<Cell, Cell::CellHash>;
    Minesweeper(size_t width, size_t height, size_t mines_count);
    Minesweeper(size_t width, size_t height, const std::vector<Cell>& cells_with_mines);

    void NewGame(size_t width, size_t height, size_t mines_count);
    void NewGame(size_t width, size_t height, const std::vector<Cell>& cells_with_mines);

    void OpenCell(const Cell& cell);
    void MarkCell(const Cell& cell);

    GameStatus GetGameStatus() const;
    time_t GetGameTime() const;

    RenderedField RenderField() const;

private:
    size_t width_;
    size_t height_;
    size_t field_size_;
    CellSet flagged_cells_;
    CellSet mined_cells_;
    CellSet opened_calls_;
    GameStatus game_status_;
    time_t start_time_;
    time_t end_time_;
    void Lose();
    void Win();
};
