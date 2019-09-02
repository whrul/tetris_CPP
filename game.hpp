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
        CELLS_IN_ROW = 13,
        CELLS_IN_COL = 25,
        CELL_SIZE = 35,
        MARGIN = 80,
        MARGIN_RIGHT = 300,
    };
    enum TimeSteps {
        START_STEP = 250,
        END_STEP = 90,
    };
    class Game {
        unsigned long long scores;
        std::list<std::vector<sf::Color>> field;
        std::list<std::vector<sf::Color>> nextShapeField;
        sf::RenderWindow window;
        gamestuff::Shape *fallingShape;
        gamestuff::Shape *nextShape;
        std::vector<gamestuff::Shape*> shapes; //bad alloc except
        void createFields(void);//should clear if not empty?
        void redrawAndShow(void);
        void drawFields(void);
        void chooseNewShape(void);//indexJ
        void removeFullLines(void);
        void createShapes(void);
      public:
        Game(void);
        ~Game();
        void startGame(void);
    };
} // namespace gamestuff

#endif