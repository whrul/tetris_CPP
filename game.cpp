#include "game.hpp"

namespace gamestuff {
    Game::Game(void) : scores(0), 
                       window(sf::VideoMode(FieldSize::WIDTH + FieldSize::MARGIN + FieldSize::MARGIN_RIGHT, FieldSize::HEIGHT + 2 * FieldSize::MARGIN), "Tetris") {
        this->createField();
    }
    Game::~Game() {
        
    }
    void Game::startGame(void) {
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
        unsigned int cellsInCol = FieldSize::HEIGHT / FieldSize::CELL_SIZE;
        unsigned int cellsInRow = FieldSize::WIDTH / FieldSize::CELL_SIZE;
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
        sf::RectangleShape line;
        line.setFillColor(sf::Color(81, 81, 81));
        for (unsigned int i = 0; i <= FieldSize::HEIGHT / FieldSize::CELL_SIZE; ++i) {
            line.setSize(sf::Vector2f(FieldSize::WIDTH, 1));
            line.setPosition(sf::Vector2f(FieldSize::MARGIN, FieldSize::MARGIN + FieldSize::CELL_SIZE * i));
            (this->window).draw(line);
        }
        for (unsigned int i = 0; i <= FieldSize::WIDTH / FieldSize::CELL_SIZE; ++i) {
            line.setSize(sf::Vector2f(1, FieldSize::HEIGHT));
            line.setPosition(sf::Vector2f(FieldSize::MARGIN + FieldSize::CELL_SIZE * i, FieldSize::MARGIN));
            (this->window).draw(line);
        }
    }
} //namespace gamestuff