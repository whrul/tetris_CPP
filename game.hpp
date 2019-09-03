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
#include <string>
#include <cstdbool>
#include "shape.hpp"

namespace gamestuff {
    enum FieldSize {
        CELLS_IN_ROW = 10,
        CELLS_IN_COL = 20,
        CELL_SIZE = 34,
        MARGIN = 70,
        MARGIN_RIGHT = 300,
    };
    enum SpeedInMilliSec {
        START_SPEED = 200,
        MAX_SPEED = 75,
        SPEED_INCR_STEP = 30,
    };
    class Game {
        unsigned long long scores;
        unsigned long long totalLinesRemoved;
        int speedInMilSec;
        std::list<std::vector<sf::Color>> field;
        std::list<std::vector<sf::Color>> nextShapeField;
        sf::RenderWindow window;
        gamestuff::Shape *fallingShape;
        gamestuff::Shape *nextShape;
        std::vector<gamestuff::Shape*> shapes; //bad alloc except
        sf::Font mainFont;
        void createFields(void);//should clear if not empty?
        void redrawAndShow(void);
        void drawFields(void);
        void chooseNewShape(void);
        void removeFullLines(void);
        void createShapes(void);
        void drawScoresAndLines(void);
        void drawPauseImage(void);
      public:
        Game(void);
        ~Game();
        void startGame(void);
    };
} // namespace gamestuff

#endif