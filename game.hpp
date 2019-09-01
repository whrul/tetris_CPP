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
#include <algorithm>
#include "shape.hpp"

namespace gamestuff {
    enum FieldSize {
        WIDTH = 400,
        HEIGHT = 800,
        CELL_SIZE = 35,
        MARGIN = 80,
        MARGIN_RIGHT = 300,
    };
    enum TimeSteps {
        START_STEP = 250,
        END_STEP = 90,
    };
    class Game {
        unsigned int scores;
        std::list<std::vector<sf::Color>> field;
        sf::RenderWindow window;
        gamestuff::Shape *fallingShape;
        std::vector<gamestuff::Shape*> shapes; //bad alloc except
        void createField(void);//should clear if not empty?
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