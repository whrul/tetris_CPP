// Author: Walerij Hrul

#ifndef GAME_HPP
#define GAME_HPP

#include <list>
#include <vector>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "shape.hpp"

namespace gamestuff {
    enum FieldSize {
        WIDTH = 500,
        HEIGHT = 800,
        CELL_SIZE = 50,
        MARGIN = 70,
        MARGIN_RIGHT = 300,
    };
    class Game {
        unsigned int scores;
        std::list<std::vector<sf::Color>> field;
        sf::RenderWindow window;
        gamestuff::Shape *fallingShape;
        std::vector<gamestuff::Shape*> shapes; //bad alloc except
        void createField(void);
        void redrawAndShow(void);
        void drawField(void);
        void chooseNewShape(void);
        void removeFullLines(void);
        void createShapes(void);
      public:
        Game(void);
        ~Game();
        void startGame(void);
    };
} // namespace gamestuff

#endif