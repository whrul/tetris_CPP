#include "game.hpp"

namespace gamestuff {
    Game::Game(void) : canAllocMemory(true),
                       scores(0),
                       highScore(0),
                       totalLinesRemoved(0),
                       speedInMilSec(SpeedInMilliSec::START_SPEED),
                       gameStatus(GameStatus::GAME_IS_ON),
                       menuPosition(GameStatus::RESTART),
                       blockMovement(false),
                       window(sf::VideoMode(FieldSize::CELLS_IN_ROW * FieldSize::CELL_SIZE + FieldSize::MARGIN + FieldSize::MARGIN_RIGHT, FieldSize::CELLS_IN_COL * FieldSize::CELL_SIZE + 2 * FieldSize::MARGIN), "Tetris"),
                       fallingShape(nullptr),
                       nextShape(nullptr),
                       showShadow(true),
                       emptyField(FieldSize::CELLS_IN_COL, std::vector<sf::Color>(FieldSize::CELLS_IN_ROW, sf::Color::Transparent)) {
        this->createShapes();
        if (!this->canAllocMemory) {
            return;
        }
        srand(time(NULL));
        this->createFields();
        this->chooseNewShape();
        (this->window).setVerticalSyncEnabled(true);
        this->uploadHighScore();
        if (!(this->mainFont).loadFromFile("font.ttf")) {
            std::cerr << "Can not upload font from font.tff.\n";
        }
    }
    Game::~Game() {
        for (int i = (this->shapes).size() - 1; i >= 0; --i) {
            delete (this->shapes)[i];
        }
    }
    void Game::startGame(void) {
        if (!this->canAllocMemory) {
            return;
        }
        while ((this->window).isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    this->writeScoresInfoInTerminal();
                    window.close();
                }
                if (event.type == sf::Event::KeyPressed) {
                   this->keyPressed(event.key.code);
                }
            }
            if (this->gameStatus == GameStatus::GAME_IS_ON) {
                this->drawGameOnImage();
            } else if(this->gameStatus == GameStatus::PAUSE){
                this->drawPauseImage();
            } else {
                this->drawGameOverImage();
            }
        }
    }
    void Game::createFields(void) {
        for (unsigned int i = 0; i < FieldSize::CELLS_IN_COL; ++i) {
            (this->field).push_back(std::vector<sf::Color>(FieldSize::CELLS_IN_ROW, sf::Color::Transparent));
        }
        for (unsigned int i = 0; i < ShapeSize::MAX_CELLS_IN_COL; ++i) {
            (this->nextShapeField).push_back(std::vector<sf::Color>(ShapeSize::MAX_CELLS_IN_ROW, sf::Color::Transparent));
        }
    }
    void Game::drawGameOnImage(void) {
        static sf::Clock clock;
        static int maxSpeed = SpeedInMilliSec::MAX_SPEED; 
        int linesRemoved = 0;
        if (clock.getElapsedTime().asMilliseconds() > std::max(this->speedInMilSec, maxSpeed)) {
            if(this->dropAllDown()){
                if(!this->chooseNewShape()) {
                    this->gameStatus = GameStatus::GAME_OVER;
                    this->highScore = std::max(this->highScore, this->scores);
                    this->saveHighScore();
                    return;
                }
                this->blockMovement = false;
            } else if (!this->fallingShape->fall(field)) {
                linesRemoved = this->removeFullLines();
                if(!linesRemoved && !this->chooseNewShape()) {
                    this->gameStatus = GameStatus::GAME_OVER;
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
        if (this->showShadow) {
            this->drawShadow();
        }
        this->drawScoresAndLines();
        (this->window).display();
    }
    void Game::drawShadow(void) {
        static sf::RectangleShape cell(sf::Vector2f(FieldSize::CELL_SIZE, FieldSize::CELL_SIZE));
        cell.setFillColor(sf::Color::Transparent);
        cell.setOutlineThickness(1);
        this->fallingShape->markShadowLocation(this->field, this->emptyField);
        for (unsigned int i = 0; i < (this->emptyField).size(); ++i) {
            for (unsigned int j = 0; j < (*std::next((this->emptyField).begin(), i)).size(); ++j) {
                if ((*std::next((this->emptyField).begin(), i))[j] != sf::Color::Transparent) {
                    if ((*std::next((this->field).begin(), i))[j] == sf::Color::Transparent) {
                        cell.setPosition(sf::Vector2f(FieldSize::MARGIN + FieldSize::CELL_SIZE * j, FieldSize::MARGIN + FieldSize::CELL_SIZE * i)); 
                        cell.setOutlineColor((*std::next((this->emptyField).begin(), i))[j]);
                        window.draw(cell);
                    }
                    (*std::next((this->emptyField).begin(), i))[j] = sf::Color::Transparent;
                }
            }
        }
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
    unsigned int Game::removeFullLines(void) {
        bool shouldRemove = true;
        unsigned int linesRemoved = 0;
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
            (this->field).push_front(std::vector<sf::Color>(FieldSize::CELLS_IN_ROW, sf::Color::Transparent));
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
            (this->shapes).push_back(new gamestuff::OBlock(0, 0, sf::Color::Yellow));
            (this->shapes).push_back(new gamestuff::TBlock(0, 0, sf::Color::Magenta));
            (this->shapes).push_back(new gamestuff::ZBlock(0, 0, sf::Color::Red));
            (this->shapes).push_back(new gamestuff::SBlock(0, 0, sf::Color::Green));
            (this->shapes).push_back(new gamestuff::JBlock(0, 0, sf::Color::Blue));
            (this->shapes).push_back(new gamestuff::LBlock(0, 0, sf::Color(255, 165, 0)));
            (this->shapes).push_back(new gamestuff::IBlock(0, 0, sf::Color::Cyan));
        } catch(std::bad_alloc& ex) {
            std::cerr << "Do not enough memory.\nThe game can not be launched.\n";
            this->canAllocMemory = false;
        }
        
    }
    void Game::drawScoresAndLines(void) {
        static int POS_X = FieldSize::MARGIN * 2 + FieldSize::CELLS_IN_ROW * FieldSize::CELL_SIZE;
        //scores achieved:
        sf::Text text("Scores: " + std::to_string(this->scores), this->mainFont, Fonts::SCORES_F);
        text.setFillColor(sf::Color::White);
        text.setPosition(sf::Vector2f(POS_X, Fonts::SCORES_Y_POS));
        (this->window).draw(text);
        //lines removed:
        text.setString("Lines: " + std::to_string(this->totalLinesRemoved));
        text.setPosition(sf::Vector2f(POS_X, Fonts::LINES_Y_POS));
        (this->window).draw(text);
        //highscore:
        text.setFillColor(sf::Color::Green);
        text.setString("High score: " + std::to_string(std::max(this->highScore, this->scores)));
        text.setPosition(sf::Vector2f(POS_X, Fonts::BEST_SCORE_Y_POS));
        (this->window).draw(text);
    }
    void Game::drawPauseImage(void) {
        static sf::Text pause("Pause..", this->mainFont, Fonts::PAUSE_F);
        pause.setFillColor(sf::Color::White);
        pause.setOrigin(sf::Vector2f(pause.getLocalBounds().width / 2, pause.getLocalBounds().height / 2));
        pause.setPosition(sf::Vector2f((this->window).getSize().x / 2, (this->window).getSize().y / 2));
        (this->window).clear(sf::Color::Black);
        (this->window).draw(pause);
        (this->window).display();
    }
    void Game::drawGameOverImage(void) {
        static std::vector<sf::Text> texts{sf::Text("Game over", this->mainFont, Fonts::GAME_OVER_F), 
                                           sf::Text("Restart", this->mainFont, Fonts::MENU_F),
                                           sf::Text("Exit", this->mainFont, Fonts::MENU_F)};
        for (unsigned int i = 0; i < texts.size(); ++i) {
            texts[i].setFillColor(sf::Color(255, 165, 0));
            texts[i].setOrigin(sf::Vector2f(texts[i].getLocalBounds().width / 2, texts[i].getLocalBounds().height / 2));
            texts[i].setPosition(sf::Vector2f((this->window).getSize().x / 2, (this->window).getSize().y / 2 + (i - texts.size() / 2) * 70));
            texts[i].setOutlineColor(sf::Color::White);
            texts[i].setOutlineThickness(0);
        }
        texts[0].setFillColor(sf::Color(255, 15, 15));
        texts[0].setOutlineThickness(2);
        unsigned int actualMenuPosition = 0;
        if (this->menuPosition == GameStatus::RESTART) {
            actualMenuPosition = 1;
        } else {
            actualMenuPosition = 2;
        }
        texts[actualMenuPosition].setOutlineThickness(1);
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
        this->gameStatus = GameStatus::GAME_IS_ON;
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
    inline void Game::writeScoresInfoInTerminal(void) const {
        std::cout << "Your scores: " << this->scores << std::endl;
        std::cout << "Removed lines: " << this->totalLinesRemoved << std::endl;
    }
    void Game::keyPressed(const sf::Keyboard::Key &key) {
        if (this->gameStatus == GameStatus::GAME_IS_ON && !this->blockMovement) {
            if (key == sf::Keyboard::D || key == sf::Keyboard::Right) {
                this->fallingShape->moveSide(this->field, 1);
            } else if (key == sf::Keyboard::A || key == sf::Keyboard::Left) {
                this->fallingShape->moveSide(this->field, -1);
            } else if (key == sf::Keyboard::W || key == sf::Keyboard::Up) {
                this->fallingShape->rotate(this->field);
            } else if (key == sf::Keyboard::S || key == sf::Keyboard::Down) {
                if (this->fallingShape->fall(this->field)){
                    // ++(this->scores);
                }
            } else if (key == sf::Keyboard::Space) {
                while (this->fallingShape->fall(this->field)){
                    // ++(this->scores);
                }
            } else if (key == sf::Keyboard::M) {
                this->showShadow = !this->showShadow;
            }
        }
        if (key == sf::Keyboard::Escape) {
            if (this->gameStatus == GameStatus::GAME_IS_ON) {
                this->gameStatus = GameStatus::PAUSE;
            } else if(this->gameStatus == GameStatus::PAUSE) {
                this->gameStatus = GameStatus::GAME_IS_ON;
            } else if(this->gameStatus == GameStatus::GAME_OVER) {
                this->writeScoresInfoInTerminal();
                window.close();
            }
        }
        if (this->gameStatus == GameStatus::GAME_OVER) {
            if (key == sf::Keyboard::W || key == sf::Keyboard::Up) {
                this->menuPosition = GameStatus::RESTART;
            } else if (key == sf::Keyboard::S || key == sf::Keyboard::Down) {
                this->menuPosition = GameStatus::EXIT;
            } else if(key == sf::Keyboard::Return) {
                if (this->menuPosition == GameStatus::EXIT) {
                    this->writeScoresInfoInTerminal();
                    window.close();
                } else {
                   this->restartGame();
                }
            }
        }
    }
} // namespace gamestuff