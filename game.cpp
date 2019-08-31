#include "game.hpp"

namespace gamestuff {
    Game::Game(void) : scores(0), 
                       window(sf::VideoMode(gamestuff::FieldSize::WIDTH + gamestuff::FieldSize::MARGIN + gamestuff::FieldSize::MARGIN_RIGHT, gamestuff::FieldSize::HEIGHT + 2 * gamestuff::FieldSize::MARGIN), "Tetris") {
        this->createField();
    }
    Game::~Game() {
        
    }
    void Game::startGame(void) {
        gamestuff::OBlock shape;
        shape.draw(this->field);
        while ((this->window).isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
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
                (this->field)[i].push_back(0);
            }
        }
    }
    void Game::redrawAndShow(void) {
        (this->window).clear(sf::Color::Black);
        this->drawFiled();
        (this->window).display();
    }
    void Game::drawFiled(void) {
        sf::RectangleShape cell(sf::Vector2f(gamestuff::FieldSize::CELL_SIZE, gamestuff::FieldSize::CELL_SIZE));
        cell.setOutlineColor(sf::Color(81, 81, 81));
        cell.setOutlineThickness(1);
        for (unsigned int i = 0; i < (this->field).size(); ++i) {
            for (unsigned int j = 0; j < (this->field)[i].size(); ++j) {
                cell.setPosition(sf::Vector2f(gamestuff::FieldSize::MARGIN + gamestuff::FieldSize::CELL_SIZE * j, gamestuff::FieldSize::MARGIN + gamestuff::FieldSize::CELL_SIZE * i));
                if ((this->field)[i][j]) {
                    cell.setFillColor(sf::Color::Cyan);
                } else {
                    cell.setFillColor(sf::Color::Transparent);
                }
                window.draw(cell);
            }
        }
    }
} // namespace gamestuff