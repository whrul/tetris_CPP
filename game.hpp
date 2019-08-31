// Author: Walerij Hrul

#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <SFML/Window.hpp>

namespace gamestuff {
    enum FieldSize {
        WIDTH = 550,
        HEIGHT = 800,
    };
    class Game {
        unsigned int scores;
        std::vector<unsigned char> field;
        sf::Window window;
      public:
        Game(void);
        ~Game();
        void startGame(void);
    };
} //namespace gamestuff

#endif