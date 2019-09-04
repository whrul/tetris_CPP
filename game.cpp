#include "game.hpp"

namespace gamestuff {
    Game::Game(void) : canNotLaunchTheGame(false),
                       scores(0),
                       highScore(0),
                       totalLinesRemoved(0),
                       speedInMilSec(SpeedInMilliSec::START_SPEED),
                       status(GameStatus::GAME_IS_ON),
                       menuPosition(GameStatus::RESTART),
                       blockMovement(false),
                       window(sf::VideoMode(FieldSize::CELLS_IN_ROW * FieldSize::CELL_SIZE + FieldSize::MARGIN + FieldSize::MARGIN_RIGHT, FieldSize::CELLS_IN_COL * FieldSize::CELL_SIZE + 2 * FieldSize::MARGIN), "Tetris"),
                       fallingShape(nullptr),
                       nextShape(nullptr) {
        srand(time(NULL));
        this->createFields();
        this->createShapes();
        if (!this->canNotLaunchTheGame) {
            this->chooseNewShape();
            (this->mainFont).loadFromFile("font.ttf");
            (this->window).setVerticalSyncEnabled(true);
            this->uploadHighScore();
        }
    }
    Game::~Game() {
        for (int i = (this->shapes).size() - 1; i >= 0; --i) {
            delete (this->shapes)[i];
        }
    }
    void Game::startGame(void) {
        if (this->canNotLaunchTheGame) {
            return;
        }
        while ((this->window).isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    std::cout << "Your scores: " << this->scores << std::endl;
                    std::cout << "Removed lines: " << this->totalLinesRemoved << std::endl;
                    window.close();
                }
                if (event.type == sf::Event::KeyPressed) {
                    if ((event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right) && this->status == GameStatus::GAME_IS_ON && !(this->blockMovement)) {
                        this->fallingShape->moveSide(this->field, 1);
                    } else if ((event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left) && this->status == GameStatus::GAME_IS_ON && !(this->blockMovement)) {
                        this->fallingShape->moveSide(this->field, -1);
                    } else if ((event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up) && this->status == GameStatus::GAME_IS_ON && !(this->blockMovement)) {
                        this->fallingShape->rotate(this->field);
                    } else if ((event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down) && this->status == GameStatus::GAME_IS_ON && !(this->blockMovement)) {
                        if (this->fallingShape->fall(this->field)){
                            ++(this->scores);
                        }
                    } else if (event.key.code == sf::Keyboard::Escape) {
                        if (this->status == GameStatus::GAME_IS_ON) {
                            this->status = GameStatus::PAUSE;
                        } else if(this->status == GameStatus::PAUSE) {
                            this->status = GameStatus::GAME_IS_ON;
                        } else if(this->status == GameStatus::GAME_OVER) {
                            std::cout << "Your scores: " << this->scores << std::endl;
                            std::cout << "Removed lines: " << this->totalLinesRemoved << std::endl;
                            window.close();
                        }
                    } else if ((event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up) && this->status == GameStatus::GAME_OVER) {
                        if (this->menuPosition == GameStatus::EXIT) {
                            this->menuPosition = GameStatus::RESTART;
                        }
                    } else if ((event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down) && this->status == GameStatus::GAME_OVER) {
                        if (this->menuPosition == GameStatus::RESTART) {
                            this->menuPosition = GameStatus::EXIT;
                        }
                    } else if (event.key.code == sf::Keyboard::Return && this->status == GameStatus::GAME_OVER) {
                        if (this->menuPosition == GameStatus::EXIT) {
                            std::cout << "Your scores: " << this->scores << std::endl;
                            std::cout << "Removed lines: " << this->totalLinesRemoved << std::endl;
                            window.close();
                        } else {
                            this->restartGame();
                        }
                    }
                }
            }
            if (this->status == GameStatus::GAME_IS_ON) {
                this->redrawAndShow();
            } else if(this->status == GameStatus::PAUSE){
                this->drawPauseImage();
            } else if(this->status == GameStatus::GAME_OVER) {
                this->drawGameOverImage();
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
    void Game::redrawAndShow(void) {
        static sf::Clock clock;
        static int maxSpeed = SpeedInMilliSec::MAX_SPEED; 
        int linesRemoved = 0;
        if (clock.getElapsedTime().asMilliseconds() > std::max(this->speedInMilSec, maxSpeed)) {
            if(this->dropAllDown()){
                if(!this->chooseNewShape()) {
                    this->status = GameStatus::GAME_OVER;
                    this->highScore = std::max(this->highScore, this->scores);
                    this->saveHighScore();
                    return;
                }
                this->blockMovement = false;
            } else if (!this->fallingShape->fall(field)) {
                linesRemoved = this->removeFullLines();
                if(!linesRemoved && !this->chooseNewShape()) {
                    this->status = GameStatus::GAME_OVER;
                    this->highScore = std::max(this->highScore, this->scores);
                    this->saveHighScore();
                    return;
                }
                if (linesRemoved) {
                    this->blockMovement = true;
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
        if (this->fallingShape != nullptr) {
            this->fallingShape->setOriginalMap();
        }
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
    int Game::removeFullLines(void) {
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
                for (unsigned int j = 0; j < (*std::next((this->field).begin(), i)).size(); ++j) {
                    (*std::next((this->field).begin(), i))[j] = sf::Color::Transparent;
                }
                ++linesRemoved;
                ++(this->totalLinesRemoved);
                this->linesForDestroy.push_back(i);
            }
        }
        return linesRemoved;
    }
    int Game::dropAllDown(void) {
        if (this->linesForDestroy.empty()) {
            return 0;
        }
        for (int i = this->linesForDestroy.size() - 1; i >= 0; --i) {
            (this->field).erase(std::next((this->field).begin(), this->linesForDestroy[i]));
        }
        for (unsigned int i = 0; i < this->linesForDestroy.size(); ++i) {
            (this->field).push_front({});
            unsigned int cellsInRow = FieldSize::CELLS_IN_ROW;
            for (unsigned int j = 0; j < cellsInRow; ++j) {
                (*(this->field).begin()).push_back(sf::Color::Transparent);
            }
        }
        static int speedStep = SpeedInMilliSec::SPEED_INCR_STEP;
        this->scores += 100 * this->linesForDestroy.size() * this->linesForDestroy.size();
        if (!(this->totalLinesRemoved % 10)) {
            this->speedInMilSec -= (this->speedInMilSec > 0) ? speedStep : 0;
        }
        int linesDroped = this->linesForDestroy.size();
        this->linesForDestroy.erase(this->linesForDestroy.begin(), this->linesForDestroy.end());
        return linesDroped;
    }
    void Game::createShapes(void) {
        try{
            (this->shapes).push_back(new gamestuff::OBlock(0, 0, sf::Color::Cyan));
            (this->shapes).push_back(new gamestuff::TBlock(0, 0, sf::Color::Red));
            (this->shapes).push_back(new gamestuff::ZBlock(0, 0, sf::Color::Yellow));
            (this->shapes).push_back(new gamestuff::SBlock(0, 0, sf::Color::Blue));
            (this->shapes).push_back(new gamestuff::JBlock(0, 0, sf::Color::Green));
            (this->shapes).push_back(new gamestuff::LBlock(0, 0, sf::Color::Magenta));
            (this->shapes).push_back(new gamestuff::IBlock(0, 0, sf::Color::White));
        } catch(std::bad_alloc& ex) {
            for (int i = (this->shapes).size() - 1; i >= 0; --i) {
                delete (this->shapes)[i];
            }
            std::cerr << "Do not enough memory.\nThe game can not be launched.\n";
            this->canNotLaunchTheGame = true;
        }
        
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
        static sf::Text pause("Pause..", this->mainFont, 65);
        pause.setFillColor(sf::Color::White);
        pause.setOrigin(pause.getLocalBounds().width / 2, pause.getLocalBounds().height / 2);
        pause.setPosition((this->window).getSize().x / 2, (this->window).getSize().y / 2);
        (this->window).clear(sf::Color::Black);
        (this->window).draw(pause);
        (this->window).display();
    }
    void Game::drawGameOverImage(void) {
        static std::vector<sf::Text> texts{sf::Text("Game over", this->mainFont, 70), 
                                           sf::Text("Restart", this->mainFont, 50),
                                           sf::Text("Exit", this->mainFont, 50)};
        for (unsigned int i = 0; i < texts.size(); ++i) {
            texts[i].setFillColor(sf::Color::Red);
            texts[i].setOrigin(texts[i].getLocalBounds().width / 2, texts[i].getLocalBounds().height / 2);
            texts[i].setPosition(sf::Vector2f((this->window).getSize().x / 2, (this->window).getSize().y / 2 + (i - texts.size() / 2) * 70));
            texts[i].setOutlineColor(sf::Color::White);
            texts[i].setOutlineThickness(0);
        }
        unsigned int actualMenuPosition = 0;
        if (this->menuPosition == GameStatus::RESTART) {
            actualMenuPosition = 1;
        } else {
            actualMenuPosition = 2;
        }
        texts[actualMenuPosition].setOutlineThickness(2);
        (this->window).clear(sf::Color::Black);
        this->drawFields();
        this->drawScoresAndLines();
        for (const auto &text : texts) {
            (this->window).draw(text);
        }
        (this->window).display();
    }
    void Game::uploadHighScore(void) {
        std::ifstream dataFile("data/data.txt");
        std::string scores = "";
        if (!dataFile.is_open()) {
            std::cout << "Can not open data file.\n";
            return;
        }
        // dataFile.seekg(0);
        std::getline(dataFile, scores);
        if (dataFile.bad() || dataFile.fail()) {
            std::cout << "Can not read from data file.\n";
            dataFile.close();
            return;
        } 
        if(isUnsignedNumber(scores)) {
            try {
                this->highScore = std::stoull(scores);
            }
            catch (std::out_of_range &ex) {
                std::cout << "Out of range.\n";
            }
        } else {
            std::cout << "Wrong data format.\n";
        }
        dataFile.close();
    }
    void Game::saveHighScore(void) {
        std::ofstream dataFile("data/data.txt");
        if (!dataFile.is_open()) {
            std::cout << "Can not open data file.\n";
            return;
        }
        dataFile << std::to_string(this->highScore);
        if (!dataFile.good()) {
            std::cout << "Can not save to data file.\n";
        }
        dataFile.close();
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
    void Game::restartGame(void) {
        this->scores = 0;
        this->totalLinesRemoved = 0;
        this->speedInMilSec = SpeedInMilliSec::START_SPEED;
        this->status = GameStatus::GAME_IS_ON;
        this->menuPosition = GameStatus::RESTART;
        this->blockMovement = false;
        this->fallingShape = nullptr;
        this->nextShape = nullptr;
        if (this->linesForDestroy.size()) {
            this->linesForDestroy.erase(linesForDestroy.begin(), linesForDestroy.end());
        }    
        for (auto &line : this->field) {
            for (auto &cell : line) {
                cell = sf::Color::Transparent;
            }
        }
        for (auto &line : this->nextShapeField) {
            for (auto &cell : line) {
                cell = sf::Color::Transparent;
            }
        }
        this->chooseNewShape();
    }
} // namespace gamestuff