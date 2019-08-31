// Author: Walerij Hrul

#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

namespace gamestuff {
    enum FieldSize {
        WIDTH = 550,
        HEIGHT = 800,
        CELL_SIZE = 50,
        MARGIN = 70,
        MARGIN_RIGHT = 300,
    };
    class Game {
        unsigned int scores;
        std::vector<std::vector<unsigned char>> field;
        sf::RenderWindow window;
      public:
        Game(void);
        ~Game();
        void startGame(void);
        void createField(void);
        void redrawAndShow(void);
        void drawFiled(void);
    };
} //namespace gamestuff

#endif