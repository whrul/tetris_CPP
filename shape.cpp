#include "shape.hpp"

namespace gamestuff {
    Shape::Shape(int leftTopCornerI, int leftTopCornerJ) : leftTopCornI(leftTopCornerI), leftTopCornJ(leftTopCornerJ) {
        // for (unsigned int i = 0; i < gamestuff::ShapeSize::MAX_CELLS_IN_COL; ++i) {
        //     (this->shapeMap).push_back({});
        //     for (unsigned int j = 0; j < gamestuff::ShapeSize::MAX_CELLS_IN_ROW; ++j) {
        //         (this->shapeMap)[i].push_back(0);
        //     }
        // }
    }
    Shape::~Shape() {

    }
    void Shape::draw(std::list<std::vector<sf::Color>> &field) const {
        for (unsigned int i = 0; i < (this->shapeMap).size(); ++i) {
            for (unsigned int j = 0; j < (this->shapeMap)[i].size(); ++j){
                if ((this->shapeMap)[i][j]) {
                    (*std::next(field.begin(), this->leftTopCornI + i))[this->leftTopCornJ + j] = sf::Color::Yellow;           
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
    OBlock::OBlock(int leftTopCornerI, int leftTopCornerJ) : Shape(leftTopCornerI, leftTopCornerJ){
        if ((gamestuff::ShapeSize::MAX_CELLS_IN_COL) > 1 && gamestuff::ShapeSize::MAX_CELLS_IN_ROW > 1) {
            for (unsigned int i = 0; i < 2; ++i) {
                (this->shapeMap).push_back({});
                for (unsigned int j = 0; j < 2; ++j) {
                    (this->shapeMap)[i].push_back(1);
                }
            }
        }
    }
    OBlock::~OBlock() {

    }
    void OBlock::rotate(void) {
        return;
    }
} // namespace gamestuff