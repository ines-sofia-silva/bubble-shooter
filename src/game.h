#ifndef GAME_H
#define GAME_H

#include "board.hpp"

// #include <cstddef>
// #include <iosfwd>
// #include <optional>
// #include <utility>
// #include <random>

class Game {
public:
    static constexpr std::size_t kRows = 8;
    static constexpr std::size_t kCols = 8;
    static constexpr std::size_t kNColors= 4;

    Game();

    void printBoard(std::ostream& out) const;
    void loop();

private:
    Board m_board;
};

#endif
