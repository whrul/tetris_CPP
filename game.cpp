#include "game.hpp"

namespace gamestuff {
    Game::Game(void) : scores(0), 
                       window(sf::VideoMode(gamestuff::FieldSize::WIDTH + gamestuff::FieldSize::MARGIN + gamestuff::FieldSize::MARGIN_RIGHT, gamestuff::FieldSize::HEIGHT + 2 * gamestuff::FieldSize::MARGIN), "Tetris") {
        this->createField();
        this->fallingShape = new gamestuff::OBlock;
    }
    Game::~Game() {
        delete this->fallingShape;
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
                        this->fallingShape->draw(this->field);
                    } else if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left) {
                        this->fallingShape->moveSide(this->field, -1);
                        this->fallingShape->draw(this->field);
                    }
                }
            }
            this->redrawAndShow();
        }
    }
    void Game::createField(void) {
        //should clear if not empty?
        unsigned int cellsInCol = gamestuff::FieldSize::HEIGHT / gamestuff::FieldSize::CELL_SIZE;
        unsigned int cellsInRow = gamestuff::FieldSize::WIDTH / gamestuff::FieldSize::CELL_SIZE;
        for (unsigned int i = 0; i < cellsInCol; ++i) {
            (this->field).push_back({});
            for (unsigned int j = 0; j < cellsInRow; ++j) {
                (this->field)[i].push_back(sf::Color::Transparent);
            }
        }
    }
    void Game::redrawAndShow(void) {
        static sf::Clock clock;
        (this->window).clear(sf::Color::Black);
        if (clock.getElapsedTime().asMilliseconds() > 500) {
            if (this->fallingShape->canFall(this->field)) {
                this->fallingShape->fall(this->field);
                this->fallingShape->draw(this->field);
            } else {
                this->fallingShape->draw(this->field);
                this->createNewShape();
            }
            clock.restart();
        }
        this->drawField();
        (this->window).display();
    }
    void Game::drawField(void) {
        sf::RectangleShape cell(sf::Vector2f(gamestuff::FieldSize::CELL_SIZE, gamestuff::FieldSize::CELL_SIZE));
        cell.setOutlineColor(sf::Color(81, 81, 81));
        cell.setOutlineThickness(1);
        for (unsigned int i = 0; i < (this->field).size(); ++i) {
            for (unsigned int j = 0; j < (this->field)[i].size(); ++j) {
                cell.setPosition(sf::Vector2f(gamestuff::FieldSize::MARGIN + gamestuff::FieldSize::CELL_SIZE * j, gamestuff::FieldSize::MARGIN + gamestuff::FieldSize::CELL_SIZE * i));
                cell.setFillColor((this->field)[i][j]);
                window.draw(cell);
            }
        }
    }
    void Game::createNewShape(void) {
        delete this->fallingShape;
        this->fallingShape = new gamestuff::OBlock;
    }
} // namespace gamestuff