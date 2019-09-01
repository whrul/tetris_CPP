#include "shape.hpp"

namespace gamestuff {
    Shape::Shape(void) : leftTopCornI(0), leftTopCornJ(0) {
        for (unsigned int i = 0; i < gamestuff::ShapeSize::CELLS_IN_COL; ++i) {
            (this->shapeMap).push_back({});
            for (unsigned int j = 0; j < gamestuff::ShapeSize::CELLS_IN_ROW; ++j) {
                (this->shapeMap)[i].push_back(0);
            }
        }
    }
    Shape::~Shape() {

    }
    void Shape::draw(std::vector<std::vector<sf::Color>> &field) const {
        for (unsigned int i = 0; i < gamestuff::ShapeSize::CELLS_IN_COL; ++i) {
            for (unsigned int j = 0; j < gamestuff::ShapeSize::CELLS_IN_ROW; ++j){
                if ((this->shapeMap)[i][j]) {
                    field[this->leftTopCornI + i][this->leftTopCornJ + j] = sf::Color::Yellow;
                }
            }
        }
    }
    void Shape::hide(std::vector<std::vector<sf::Color>> &field) const {
        for (unsigned int i = 0; i < gamestuff::ShapeSize::CELLS_IN_COL; ++i) {
            for (unsigned int j = 0; j < gamestuff::ShapeSize::CELLS_IN_ROW; ++j){
                if ((this->shapeMap)[i][j]) {
                    field[this->leftTopCornI + i][this->leftTopCornJ + j] = sf::Color::Transparent;
                }
            }
        }
    }
    bool Shape::canFall(const std::vector<std::vector<sf::Color>> &field) const {
        for (unsigned int i = 0; i < gamestuff::ShapeSize::CELLS_IN_COL; ++i) {
            for (unsigned int j = 0; j < gamestuff::ShapeSize::CELLS_IN_ROW; ++j){
                if (!(this->shapeMap)[i][j]) {
                    continue;
                }
                bool isFloor = this->leftTopCornI + i + 1 >= field.size();
                if (isFloor) {
                    return false;
                }
                bool isOnBlocks = field[this->leftTopCornI + i + 1][this->leftTopCornJ + j] != sf::Color::Transparent &&
                                  (i == gamestuff::ShapeSize::CELLS_IN_COL - 1 || !((this->shapeMap)[i+1][j]));
                if (isOnBlocks) {
                    return false;
                }
            }
        }
        return true;
    }
    bool Shape::fall(std::vector<std::vector<sf::Color>> &field) {
        if (!(this->canFall(field))) {
            return false;
        }
        this->hide(field);
        ++(this->leftTopCornI);
        this->draw(field);
        return true;
    }
    bool Shape::canMoveSide(const std::vector<std::vector<sf::Color>> &field, const int direction) const {
        for (unsigned int i = 0; i < gamestuff::ShapeSize::CELLS_IN_COL; ++i) {
            for (unsigned int j = 0; j < gamestuff::ShapeSize::CELLS_IN_ROW; ++j){
                if (!(this->shapeMap)[i][j]) {
                    continue;
                }
                bool isNearWall = (this->leftTopCornJ + j + direction >= field[0].size()) || (this->leftTopCornJ + j + direction < 0);
                if (isNearWall) {
                    return false;
                }
                bool isNearBlocks = field[this->leftTopCornI + i][this->leftTopCornJ + j + direction] != sf::Color::Transparent &&
                                  (j + direction < 0 || j + direction >= gamestuff::ShapeSize::CELLS_IN_ROW || !((this->shapeMap)[i][j + direction]));
                if (isNearBlocks) {
                    return false;
                }
            }
        }
        return true;
    }
    bool Shape::moveSide(std::vector<std::vector<sf::Color>> &field, const int direction) {
        if (!(this->canMoveSide(field, direction))) {    
            return false;
        }
        this->hide(field);
        this->leftTopCornJ += direction;
        this->draw(field);
        return true;
    }
    OBlock::OBlock(void){
        if ((gamestuff::ShapeSize::CELLS_IN_COL) > 1 && gamestuff::ShapeSize::CELLS_IN_ROW > 1) {
            (this->shapeMap)[gamestuff::ShapeSize::CELLS_IN_COL - 2][0] = 1;
            (this->shapeMap)[gamestuff::ShapeSize::CELLS_IN_COL - 2][1] = 1;
            (this->shapeMap)[gamestuff::ShapeSize::CELLS_IN_COL - 1][0] = 1;
            (this->shapeMap)[gamestuff::ShapeSize::CELLS_IN_COL - 1][1] = 1;
        }
    }
    OBlock::~OBlock() {

    }
    void OBlock::rotate(void) {
        return;
    }
} // namespace gamestuff