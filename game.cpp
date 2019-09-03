#include "game.hpp"

namespace gamestuff {
    Game::Game(void) : scores(0),
                       totalLinesRemoved(0),
                       speedInMilSec(SpeedInMilliSec::START_SPEED),
                       window(sf::VideoMode(FieldSize::CELLS_IN_ROW * FieldSize::CELL_SIZE + FieldSize::MARGIN + FieldSize::MARGIN_RIGHT, FieldSize::CELLS_IN_COL * FieldSize::CELL_SIZE + 2 * FieldSize::MARGIN), "Tetris"),
                       fallingShape(nullptr),
                       nextShape(nullptr) {
        srand(time(NULL));
        this->createFields();
        this->createShapes();
        this->chooseNewShape();
        (this->mainFont).loadFromFile("font.ttf");
        (this->window).setVerticalSyncEnabled(true);
        this->uploadHighScore();
    }
    Game::~Game() {
        for (int i = (this->shapes).size() - 1; i >= 0; --i) {
            delete (this->shapes)[i];
        }
    }
    void Game::startGame(void) {
        bool pause = false;
        while ((this->window).isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    std::cout << "Your scores: " << this->scores << std::endl;
                    std::cout << "Removed lines: " << this->totalLinesRemoved << std::endl;
                    window.close();
                }
                if (event.type == sf::Event::KeyPressed) {
                    if ((event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right) && !pause) {
                        this->fallingShape->moveSide(this->field, 1);
                    } else if ((event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left) && !pause) {
                        this->fallingShape->moveSide(this->field, -1);
                    } else if ((event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up) && !pause) {
                        this->fallingShape->rotate(this->field);
                    } else if ((event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down) && !pause) {
                        if (this->fallingShape->fall(this->field)){
                            ++(this->scores);
                        }
                    } else if (event.key.code == sf::Keyboard::Escape) {
                        pause = pause ? false : true;
                    }
                }
            }
            if (!pause) {
                this->redrawAndShow(pause);
            } else {
                this->drawPauseImage();
            }
        }
    }
    void Game::createFields(void) {
        unsigned int cellsInCol = FieldSize::CELLS_IN_COL;
        unsigned int cellsInRow = FieldSize::CELLS_IN_ROW;
        for (unsigned int i = 0; i < cellsInCol; ++i) {
            (this->field).push_back({});
            for (unsigned int j = 0; j < cellsInRow; ++j) {
                (*std::next((this->field).begin(), i)).push_back(sf::Color::Transparent);
            }
        }

        cellsInCol = ShapeSize::MAX_CELLS_IN_COL;
        cellsInRow = ShapeSize::MAX_CELLS_IN_ROW;
        for (unsigned int i = 0; i < cellsInCol; ++i) {
            (this->nextShapeField).push_back({});
            for (unsigned int j = 0; j < cellsInRow; ++j) {
                (*std::next((this->nextShapeField).begin(), i)).push_back(sf::Color::Transparent);
            }
        }
    }
    void Game::redrawAndShow(bool& pause) {
        static sf::Clock clock;
        static int maxSpeed = SpeedInMilliSec::MAX_SPEED; 
        if (clock.getElapsedTime().asMilliseconds() > std::max(this->speedInMilSec, maxSpeed)) {
            if (!this->fallingShape->fall(field)) {
                this->removeFullLines();
                if(!this->chooseNewShape()) {
                    pause = true;
                    return;
                }
            }
            clock.restart();
        }
        (this->window).clear(sf::Color::Black);        
        this->drawFields();
        this->drawScoresAndLines();
        (this->window).display();
    }
    void Game::drawFields(void) {
        static sf::RectangleShape cell(sf::Vector2f(FieldSize::CELL_SIZE, FieldSize::CELL_SIZE));
        cell.setOutlineColor(sf::Color(81, 81, 81));
        cell.setOutlineThickness(1);
        for (unsigned int i = 0; i < (this->field).size(); ++i) {
            for (unsigned int j = 0; j < (*std::next((this->field).begin(), i)).size(); ++j) {
                cell.setPosition(sf::Vector2f(FieldSize::MARGIN + FieldSize::CELL_SIZE * j, FieldSize::MARGIN + FieldSize::CELL_SIZE * i));
                cell.setFillColor((*std::next((this->field).begin(), i))[j]);
                window.draw(cell);
            }
        }
        for (unsigned int i = 0; i < (this->nextShapeField).size(); ++i) {
            for (unsigned int j = 0; j < (*std::next((this->nextShapeField).begin(), i)).size(); ++j) {
                if (((*std::next((this->nextShapeField).begin(), i))[j]) == sf::Color::Transparent) {
                    continue;
                }
                cell.setPosition(sf::Vector2f(FieldSize::MARGIN * 2 + FieldSize::CELLS_IN_ROW * FieldSize::CELL_SIZE + FieldSize::CELL_SIZE * j, FieldSize::MARGIN + FieldSize::CELL_SIZE * i));
                cell.setFillColor((*std::next((this->nextShapeField).begin(), i))[j]);
                window.draw(cell);
            }
        }
    }
    bool Game::chooseNewShape(void) {
        static int startIndexJ = FieldSize::CELLS_IN_ROW / 2 - ShapeSize::MAX_CELLS_IN_ROW / 2;
        if (this->nextShape != nullptr) {
            this->fallingShape = this->nextShape;
        } else {
            this->fallingShape = (this->shapes)[rand() % (this->shapes).size()];
        } 
        this->fallingShape->setPosition(0, startIndexJ); 
        if(!this->fallingShape->canDraw(this->field)) {
            return false;
        }
        this->fallingShape->draw(this->field);
        for (auto &row : this->nextShapeField) {
            for (auto &cell : row) {
                cell = sf::Color::Transparent;
            }
        }
        this->nextShape = (this->shapes)[rand() % (this->shapes).size()];
        this->nextShape->setPosition(0, 0);
        this->nextShape->draw(this->nextShapeField);
        this->fallingShape->setPosition(0, startIndexJ); 
        return true;
    }
    void Game::removeFullLines(void) {
        bool shouldRemove = true;
        int linesRemoved = 0;
        for (unsigned int i = 0; i < field.size(); ++i) {
            shouldRemove = true;
            for (unsigned int j = 0; j < (*std::next((this->field).begin(), i)).size(); ++j) {
                if ((*std::next((this->field).begin(), i))[j] == sf::Color::Transparent) {
                    shouldRemove = false;
                    break;
                }
            }
            if (shouldRemove) {
                (this->field).erase(std::next((this->field).begin(), i));
                (this->field).push_front({});
                unsigned int cellsInRow = FieldSize::CELLS_IN_ROW;
                for (unsigned int i = 0; i < cellsInRow; ++i) {
                    (*(this->field).begin()).push_back(sf::Color::Transparent);
                }
                ++linesRemoved;
                ++(this->totalLinesRemoved);
            }
        }
        static int speedStep = SpeedInMilliSec::SPEED_INCR_STEP;
        this->scores += 100 * linesRemoved * linesRemoved;
        if (linesRemoved && !(this->totalLinesRemoved % 10)) {
            this->speedInMilSec -= (this->speedInMilSec > 0) ? speedStep : 0;
        }
    }
    void Game::createShapes(void) {
        (this->shapes).push_back(new gamestuff::OBlock(0, 0, sf::Color::Cyan));
        (this->shapes).push_back(new gamestuff::TBlock(0, 0, sf::Color::Red));
        (this->shapes).push_back(new gamestuff::ZBlock(0, 0, sf::Color::Yellow));
        (this->shapes).push_back(new gamestuff::SBlock(0, 0, sf::Color::Blue));
        (this->shapes).push_back(new gamestuff::JBlock(0, 0, sf::Color::Green));
        (this->shapes).push_back(new gamestuff::LBlock(0, 0, sf::Color::Magenta));
        (this->shapes).push_back(new gamestuff::IBlock(0, 0, sf::Color::White));
    }
    void Game::drawScoresAndLines(void) {
        static sf::Text scores("It's string", this->mainFont, 30);
        scores.setString("Scores: " + std::to_string(this->scores));
        scores.setFillColor(sf::Color::White);
        scores.setPosition(sf::Vector2f(FieldSize::MARGIN * 2 + FieldSize::CELLS_IN_ROW * FieldSize::CELL_SIZE, FieldSize::MARGIN * 2 + ShapeSize::MAX_CELLS_IN_COL * FieldSize::CELL_SIZE));
        (this->window).draw(scores);
        
        static sf::Text lines("It's string", this->mainFont, 30);
        scores.setString("Lines: " + std::to_string(this->totalLinesRemoved));
        scores.setFillColor(sf::Color::White);
        scores.setPosition(sf::Vector2f(FieldSize::MARGIN * 2 + FieldSize::CELLS_IN_ROW * FieldSize::CELL_SIZE, FieldSize::MARGIN * 2 + ShapeSize::MAX_CELLS_IN_COL * FieldSize::CELL_SIZE + 2 * scores.getCharacterSize()));
        (this->window).draw(scores);
        
        static sf::Text highScore("It's string", this->mainFont, 30);
        highScore.setString("High score: " + std::to_string(std::max(this->highScore, this->scores)));
        highScore.setFillColor(sf::Color::White);
        highScore.setPosition(sf::Vector2f(FieldSize::MARGIN * 2 + FieldSize::CELLS_IN_ROW * FieldSize::CELL_SIZE, FieldSize::MARGIN * 2 + ShapeSize::MAX_CELLS_IN_COL * FieldSize::CELL_SIZE + 2 * scores.getCharacterSize() + 3 * lines.getCharacterSize()));
        (this->window).draw(highScore);
    }
    void Game::drawPauseImage(void) {
        // static sf::Text pause("Pause..", this->mainFont, 65);
        // pause.setOrigin(pause.getLocalBounds().width / 2, pause.getLocalBounds().height / 2);
        // pause.setPosition((this->window).getSize().x / 2, (this->window).getSize().y / 2);
        // (this->window).clear(sf::Color::Black);
        // (this->window).draw(pause);
        (this->window).clear();
        this->drawFields();
        (this->window).display();
    }
    void Game::uploadHighScore(void) {
        std::ifstream dataFile("data/data.txt");
        std::string scores = "";
        if (!dataFile.is_open()) {
            std::cout << "The data file is demaged.\n";
            this->highScore = 0;
            return;
        }
        std::getline(dataFile, scores);
        if (!dataFile.good()) {
            std::cout << "The data file is demaged.\n";
            this->highScore = 0;
            dataFile.close();
            return;
        } 
        if(isUnsignedNumber(scores)) {
            try {
                this->highScore = std::stoull(scores);
            }
            catch (std::out_of_range &ex) {
                this->highScore = 0;
                std::cout << "Out of range.\n";
            }
        } else {
            this->highScore = 0;
            std::cout << "Wrong data format.\n";
        }
        dataFile.close();
    }
    void Game::saveScore(void) {

    }
    bool isUnsignedNumber(std::string numberStr) {
        if (!numberStr.size()) {
            return false;
        }
        for (const char& c : numberStr) {
            if (c < '0' || c > '9') {
                return false;
            }
        }
        return true;
    }
} // namespace gamestuff