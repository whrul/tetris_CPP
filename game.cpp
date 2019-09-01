#include "game.hpp"

namespace gamestuff {
    Game::Game(void) : scores(0), 
                       window(sf::VideoMode(gamestuff::FieldSize::WIDTH + gamestuff::FieldSize::MARGIN + gamestuff::FieldSize::MARGIN_RIGHT, gamestuff::FieldSize::HEIGHT + 2 * gamestuff::FieldSize::MARGIN), "Tetris"),
                       fallingShape(nullptr) {
        srand(time(NULL));
        this->createField();
        this->createShapes();
        this->chooseNewShape();
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
                        this->fallingShape->fall(this->field);
                    }
                }
            }
            this->redrawAndShow();
        }
    }
    void Game::createField(void) {
        unsigned int cellsInCol = gamestuff::FieldSize::HEIGHT / gamestuff::FieldSize::CELL_SIZE;
        unsigned int cellsInRow = gamestuff::FieldSize::WIDTH / gamestuff::FieldSize::CELL_SIZE;
        for (unsigned int i = 0; i < cellsInCol; ++i) {
            (this->field).push_back({});
            for (unsigned int j = 0; j < cellsInRow; ++j) {
                (*std::next((this->field).begin(), i)).push_back(sf::Color::Transparent);
            }
        }
    }
    void Game::redrawAndShow(void) {
        static sf::Clock clock;
        static double actualStep = gamestuff::TimeSteps::START_STEP;
        static double endStep = gamestuff::TimeSteps::END_STEP;
        (this->window).clear(sf::Color::Black);
        if (clock.getElapsedTime().asMilliseconds() > std::max(endStep, actualStep)) {
            if (!this->fallingShape->fall(field)) {
                this->chooseNewShape();
            }
            clock.restart();
            if(actualStep > 0) {
                actualStep -= 0.25;
            }
        }
        this->removeFullLines();
        this->drawField();
        (this->window).display();
    }
    void Game::drawField(void) {
        static sf::RectangleShape cell(sf::Vector2f(gamestuff::FieldSize::CELL_SIZE, gamestuff::FieldSize::CELL_SIZE));
        cell.setOutlineColor(sf::Color(81, 81, 81));
        cell.setOutlineThickness(1);
        for (unsigned int i = 0; i < (this->field).size(); ++i) {
            for (unsigned int j = 0; j < (*std::next((this->field).begin(), i)).size(); ++j) {
                cell.setPosition(sf::Vector2f(gamestuff::FieldSize::MARGIN + gamestuff::FieldSize::CELL_SIZE * j, gamestuff::FieldSize::MARGIN + gamestuff::FieldSize::CELL_SIZE * i));
                cell.setFillColor((*std::next((this->field).begin(), i))[j]);
                window.draw(cell);
            }
        }
    }
    void Game::chooseNewShape(void) {
        this->fallingShape = (this->shapes)[rand() % (this->shapes).size()];
        this->fallingShape->setPosition(0,0);
    }
    void Game::removeFullLines(void) {
        this->fallingShape->hide(this->field);
        bool shouldRemove = true;
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
                unsigned int cellsInRow = gamestuff::FieldSize::WIDTH / gamestuff::FieldSize::CELL_SIZE;
                for (unsigned int i = 0; i < cellsInRow; ++i) {
                    (*(this->field).begin()).push_back(sf::Color::Transparent);
                }
            }
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
} // namespace gamestuff