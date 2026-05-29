#ifndef GAME_H
#define GAME_H

#include "board.hpp"
#include "bubble/include/bubble_color.hpp"


class Game {
public:
    static constexpr std::size_t kRows = 8;
    static constexpr std::size_t kCols = 8;
    static constexpr std::size_t kMissesBeforeNewRow = 3;

    Game();

    void printBoard(std::ostream& out) const;
    void loop();

private:
    bool isGameOver() const;
    Bubble::ColorManager m_colorManager;
    Board m_board;
    std::size_t m_missCount{0};
};

#endif
