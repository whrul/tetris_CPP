// Author: Walerij Hrul

#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "shape.hpp"

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
        std::vector<std::vector<sf::Color>> field;
        sf::RenderWindow window;
        gamestuff::Shape *fallingShape; //bad alloc except
        void createField(void);
        void redrawAndShow(void);
        void drawFiled(void);
      public:
        Game(void);
        ~Game();
        void startGame(void);
    };
} // namespace gamestuff

#endif