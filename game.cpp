#include "game.hpp"

namespace gamestuff {
    Game::Game(void) : scores(0), window(sf::VideoMode(FieldSize::WIDTH, FieldSize::HEIGHT), "Tetris") {

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
        }
    }
} //namespace gamestuff