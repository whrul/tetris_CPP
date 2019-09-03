#include "shape.hpp"

namespace gamestuff {
    Shape::Shape(int leftTopCornerI, int leftTopCornerJ, sf::Color color) : leftTopCornI(leftTopCornerI), leftTopCornJ(leftTopCornerJ), mainColor(color) {
    }
    Shape::~Shape() {

    }
    void Shape::draw(std::list<std::vector<sf::Color>> &field) const {
        for (unsigned int i = 0; i < (this->shapeMap).size(); ++i) {
            for (unsigned int j = 0; j < (this->shapeMap)[i].size(); ++j){
                if ((this->shapeMap)[i][j]) {
                    (*std::next(field.begin(), this->leftTopCornI + i))[this->leftTopCornJ + j] = this->mainColor;           
                }
            }
        }
    }
    void Shape::hide(std::list<std::vector<sf::Color>> &field) const {
        for (unsigned int i = 0; i < (this->shapeMap).size(); ++i) {
            for (unsigned int j = 0; j < (this->shapeMap)[i].size(); ++j){
                if ((this->shapeMap)[i][j]) {
                    (*std::next(field.begin(), this->leftTopCornI + i))[this->leftTopCornJ + j] = sf::Color::Transparent;
                }
            }
        }
    }
    bool Shape::canFall(const std::list<std::vector<sf::Color>> &field) const {
        for (unsigned int i = 0; i < (this->shapeMap).size(); ++i) {
            for (unsigned int j = 0; j < (this->shapeMap)[i].size(); ++j){
                if (!(this->shapeMap)[i][j]) {
                    continue;
                }
                bool isFloor = this->leftTopCornI + i + 1 >= field.size();
                if (isFloor) {
                    return false;
                }
                bool isOnBlocks =  (*std::next(field.begin(), this->leftTopCornI + i + 1))[this->leftTopCornJ + j] != sf::Color::Transparent &&
                                  (i == (this->shapeMap).size() - 1 || !((this->shapeMap)[i+1][j]));
                if (isOnBlocks) {
                    return false;
                }
            }
        }
        return true;
    }
    bool Shape::fall(std::list<std::vector<sf::Color>> &field) {
        if (!(this->canFall(field))) {
            return false;
        }
        this->hide(field);
        ++(this->leftTopCornI);
        this->draw(field);
        return true;
    }
    bool Shape::canMoveSide(const std::list<std::vector<sf::Color>> &field, const int direction) const {
        for (unsigned int i = 0; i < (this->shapeMap).size(); ++i) {
            for (unsigned int j = 0; j < (this->shapeMap)[i].size(); ++j){
                if (!(this->shapeMap)[i][j]) {
                    continue;
                }
                bool isNearWall = (this->leftTopCornJ + j + direction >=  (*field.begin()).size()) || (this->leftTopCornJ + j + direction < 0);
                if (isNearWall) {
                    return false;
                }
                bool isNearBlocks =  (*std::next(field.begin(), this->leftTopCornI + i))[this->leftTopCornJ + j + direction] != sf::Color::Transparent &&
                                  (j + direction < 0 || j + direction >= (this->shapeMap)[i].size() || !((this->shapeMap)[i][j + direction]));
                if (isNearBlocks) {
                    return false;
                }
            }
        }
        return true;
    }
    bool Shape::moveSide(std::list<std::vector<sf::Color>> &field, const int direction) {
        if (!(this->canMoveSide(field, direction))) {    
            return false;
        }
        this->hide(field);
        this->leftTopCornJ += direction;
        this->draw(field);
        return true;
    }
    void Shape::setPosition(int leftTopCornerI, int leftTopCornerJ) {
        this -> leftTopCornI = leftTopCornerI;
        this -> leftTopCornJ = leftTopCornerJ;
    }
    bool Shape::canRotate(std::list<std::vector<sf::Color>> &field, std::vector<std::vector<int>> &newShapeMap) {
        this->hide(field);
        for (unsigned int i = 0; i < newShapeMap.size(); ++i) {
            for (unsigned int j = 0; j < newShapeMap[i].size(); ++j){
                if (!newShapeMap[i][j]) {
                    continue;
                }
                if (this->leftTopCornI + i < 0 ||
                    this->leftTopCornI + i >= field.size() ||
                    this->leftTopCornJ + j < 0 ||
                    this->leftTopCornJ + j >= (*std::next(field.begin(), i)).size() ||
                    (*std::next((field).begin(), this->leftTopCornI + i))[this->leftTopCornJ + j] != sf::Color::Transparent) {
                    this->draw(field);
                    return false;           
                }
            }
        }
        this->draw(field);
        return true;
    }
    bool Shape::rotate(std::list<std::vector<sf::Color>> &field) {
        // for (int i = 1; i < (this->shapeMap).size(); ++i) {
        //     if ((this->shapeMap)[i].size() != (this->shapeMap)[i-1].size()) {
        //         return;
        //     }
        // }
        std::vector<std::vector<int>> newShapeMap;
        for (unsigned int i = 0; i < (this->shapeMap)[0].size(); ++i) {
            newShapeMap.push_back({});
            for (int j = (this->shapeMap).size() - 1; j >= 0; --j) {
                newShapeMap[i].push_back((this->shapeMap)[j][i]);
            }
        }
        if (!(this->canRotate(field, newShapeMap))) {
            return false;
        }
        this->hide(field);
        this->shapeMap = newShapeMap;
        this->draw(field);
        return true;
    }
    bool Shape::canDraw(std::list<std::vector<sf::Color>> &field) const {
        for (unsigned int i = 0; i < (this->shapeMap).size(); ++i) {
            for (unsigned int j = 0; j < (this->shapeMap)[i].size(); ++j){
                if (!(this->shapeMap)[i][j]) {
                    continue;
                }
                if (this->leftTopCornI + i < 0 ||
                    this->leftTopCornI + i >= field.size() ||
                    this->leftTopCornJ + j < 0 ||
                    this->leftTopCornJ + j >= (*std::next(field.begin(), this->leftTopCornI + i)).size() ||
                    (*std::next(field.begin(), this->leftTopCornI + i))[this->leftTopCornJ + j] != sf::Color::Transparent) {
                    return false;          
                }
            }
        }
        return true;
    }
    OBlock::OBlock(int leftTopCornerI, int leftTopCornerJ, sf::Color color) : Shape(leftTopCornerI, leftTopCornerJ, color){
        if ((ShapeSize::MAX_CELLS_IN_COL) > 1 && ShapeSize::MAX_CELLS_IN_ROW > 1) {
            (this->shapeMap).push_back({1, 1});
            (this->shapeMap).push_back({1, 1});
        }
    }
    bool OBlock::rotate(std::list<std::vector<sf::Color>> &field){
        return true;
    }
    TBlock::TBlock(int leftTopCornerI, int leftTopCornerJ, sf::Color color) : Shape(leftTopCornerI, leftTopCornerJ, color){
        if ((ShapeSize::MAX_CELLS_IN_COL) > 2 && ShapeSize::MAX_CELLS_IN_ROW > 2) {
            (this->shapeMap).push_back({0, 1, 0});
            (this->shapeMap).push_back({1, 1, 1});
            (this->shapeMap).push_back({0, 0, 0});
        }
    }
    ZBlock::ZBlock(int leftTopCornerI, int leftTopCornerJ, sf::Color color) : Shape(leftTopCornerI, leftTopCornerJ, color){
        if ((ShapeSize::MAX_CELLS_IN_COL) > 2 && ShapeSize::MAX_CELLS_IN_ROW > 2) {
            (this->shapeMap).push_back({1, 1, 0});
            (this->shapeMap).push_back({0, 1, 1});
            (this->shapeMap).push_back({0, 0, 0});
        }
    }
    SBlock::SBlock(int leftTopCornerI, int leftTopCornerJ, sf::Color color) : Shape(leftTopCornerI, leftTopCornerJ, color){
        if ((ShapeSize::MAX_CELLS_IN_COL) > 2 && ShapeSize::MAX_CELLS_IN_ROW > 2) {
            (this->shapeMap).push_back({0, 1, 1});
            (this->shapeMap).push_back({1, 1, 0});
            (this->shapeMap).push_back({0, 0, 0});
        }
    }
    JBlock::JBlock(int leftTopCornerI, int leftTopCornerJ, sf::Color color) : Shape(leftTopCornerI, leftTopCornerJ, color){
        if ((ShapeSize::MAX_CELLS_IN_COL) > 2 && ShapeSize::MAX_CELLS_IN_ROW > 2) {
            (this->shapeMap).push_back({1, 0, 0});
            (this->shapeMap).push_back({1, 1, 1});
            (this->shapeMap).push_back({0, 0, 0});
        }
    }
    LBlock::LBlock(int leftTopCornerI, int leftTopCornerJ, sf::Color color) : Shape(leftTopCornerI, leftTopCornerJ, color){
        if ((ShapeSize::MAX_CELLS_IN_COL) > 2 && ShapeSize::MAX_CELLS_IN_ROW > 2) {
            (this->shapeMap).push_back({0, 0, 1});
            (this->shapeMap).push_back({1, 1, 1});
            (this->shapeMap).push_back({0, 0, 0});
        }
    }
    IBlock::IBlock(int leftTopCornerI, int leftTopCornerJ, sf::Color color) : Shape(leftTopCornerI, leftTopCornerJ, color){
        if ((ShapeSize::MAX_CELLS_IN_COL) > 3 && ShapeSize::MAX_CELLS_IN_ROW > 3) {
            (this->shapeMap).push_back({0, 0, 1, 0});
            (this->shapeMap).push_back({0, 0, 1, 0});
            (this->shapeMap).push_back({0, 0, 1, 0});
            (this->shapeMap).push_back({0, 0, 1, 0});
        }
    }
} // namespace gamestuff