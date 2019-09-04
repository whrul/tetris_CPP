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
#include <fstream>
#include "shape.hpp"

namespace gamestuff {
    enum FieldSize {
        CELLS_IN_ROW = 10,
        CELLS_IN_COL = 20,
        CELL_SIZE = 34,
        MARGIN = 70,
        MARGIN_RIGHT = 360,
    };
    enum SpeedInMilliSec {
        START_SPEED = 200,
        MAX_SPEED = 75,
        SPEED_INCR_STEP = 30,
    };
    enum GameStatus {
        GAME_IS_ON = 0,
        PAUSE,
        GAME_OVER,
        RESTART,
        EXIT,
    };
    class Game {
        bool canNotLaunchTheGame;
        unsigned long long scores;
        unsigned long long highScore;
        unsigned long long totalLinesRemoved;
        int speedInMilSec;
        GameStatus status;
        GameStatus menuPosition;
        bool blockMovement;
        std::list<std::vector<sf::Color>> field;
        std::list<std::vector<sf::Color>> nextShapeField;
        sf::RenderWindow window;
        gamestuff::Shape *fallingShape;
        gamestuff::Shape *nextShape;
        std::vector<gamestuff::Shape*> shapes;
        sf::Font mainFont;
        std::vector<int> linesForDestroy;
        void createFields(void);
        void redrawAndShow(void);
        void drawFields(void);
        bool chooseNewShape(void);
        int removeFullLines(void);
        int dropAllDown(void);
        void createShapes(void);
        void drawScoresAndLines(void);
        void drawPauseImage(void);
        void drawGameOverImage(void);
        void uploadHighScore(void);
        void saveHighScore(void);
        void restartGame();
      public:
        Game(void);
        ~Game();
        void startGame(void);
    };
    bool isUnsignedNumber(std::string numberStr);
} // namespace gamestuff

#endif