#include "tests.hpp"
#include "game.hpp"

int main() {
    gamestuff::Tests::completeAllTests();
    gamestuff::Game game;
    game.startGame();
    return 0;
}