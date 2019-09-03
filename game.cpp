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
    }
    Game::~Game() {
        for (int i = (this->shapes).size() - 1; i >= 0; --i) {
            delete (this->shapes)[i];
        }
    }
    void Game::startGame(void) {
        while ((this->window).isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    std::cout << "Your scores: " << this->scores << std::endl;
                    std::cout << "Removed lines: " << this->totalLinesRemoved << std::endl;
                    window.close();
                }
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right) {
                        this->fallingShape->moveSide(this->field, 1);
                    } else if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left) {
                        this->fallingShape->moveSide(this->field, -1);
                    } else if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up) {
                        this->fallingShape->rotate(this->field);
                    } else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down) {
                        if (this->fallingShape->fall(this->field)){
                            ++(this->scores);
                        }
                    }
                }
            }
            this->redrawAndShow();
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
    void Game::redrawAndShow(void) {
        static sf::Clock clock;
        static int maxSpeed = SpeedInMilliSec::MAX_SPEED;
        (this->window).clear(sf::Color::Black);
        if (clock.getElapsedTime().asMilliseconds() > std::max(this->speedInMilSec, maxSpeed)) {
            if (!this->fallingShape->fall(field)) {
                this->chooseNewShape();
            }
            clock.restart();
        }
        this->removeFullLines();
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
    void Game::chooseNewShape(void) {
        static int startIndexJ = FieldSize::CELLS_IN_ROW / 2 - ShapeSize::MAX_CELLS_IN_ROW / 2;
        if (this->nextShape != nullptr) {
            this->fallingShape = this->nextShape;
        } else {
            this->fallingShape = (this->shapes)[rand() % (this->shapes).size()];
        } 
        this->nextShape = (this->shapes)[rand() % (this->shapes).size()];
        for (auto &row : this->nextShapeField) {
            for (auto &cell : row) {
                cell = sf::Color::Transparent;
            }
        }
        this->nextShape->setPosition(0, 0);
        this->nextShape->draw(this->nextShapeField);
        this->fallingShape->setPosition(0, startIndexJ); 
    }
    void Game::removeFullLines(void) {
        this->fallingShape->hide(this->field);
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
        this->fallingShape->draw(this->field);
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
        static sf::Text scores("It's string", this->mainFont, 35);
        scores.setString("Scores: " + std::to_string(this->scores));
        scores.setFillColor(sf::Color::White);
        scores.setPosition(sf::Vector2f(FieldSize::MARGIN * 2 + FieldSize::CELLS_IN_ROW * FieldSize::CELL_SIZE, FieldSize::MARGIN * 2 + ShapeSize::MAX_CELLS_IN_COL * FieldSize::CELL_SIZE));
        (this->window).draw(scores);
        static sf::Text lines("It's string", this->mainFont, 35);
        scores.setString("Lines: " + std::to_string(this->totalLinesRemoved));
        scores.setFillColor(sf::Color::White);
        scores.setPosition(sf::Vector2f(FieldSize::MARGIN * 2 + FieldSize::CELLS_IN_ROW * FieldSize::CELL_SIZE, FieldSize::MARGIN * 2 + ShapeSize::MAX_CELLS_IN_COL * FieldSize::CELL_SIZE + 2 * scores.getCharacterSize()));
        (this->window).draw(scores);
    }
} // namespace gamestuff