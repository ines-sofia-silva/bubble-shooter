#include "game.h"

#include <iostream>

int main() {
    Game game;

    std::cout << "Use an angle between 20 and 160. Type 'exit' to quit.\n";
    std::cout << "Initial board:\n";
    game.printBoard(std::cout);
    game.loop();

    return 0;
}
