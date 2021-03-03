#include "board.h"
#include <iostream>

int main(int argc, char *argv[]) {

    Board board;
    board.AddGemer("Stas");
    board.AddGemer("Olga");

    std::cout << board.TryToStartGame() << std::endl;
    std::cout << board.GetGamers().size() << std::endl;
    std::cout << board.IsRunning() << std::endl;

    std::cout << "---------------------\n";

    while (board.IsRunning()) {
        std::cout << (size_t)board.GetCountOfLives() << std::endl;
        board.PlayCard(0);
    }

    std::cout << "---------------------\n";
    std::cout << "SCORE: " << board.GetScore() << std::endl;

    return EXIT_SUCCESS;
}
