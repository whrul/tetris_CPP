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
    void Shape::draw(std::vector<std::vector<unsigned char>> &field) const {
        for (unsigned int i = 0; i < gamestuff::ShapeSize::CELLS_IN_COL; ++i) {
            for (unsigned int j = 0; j < gamestuff::ShapeSize::CELLS_IN_ROW; ++j){
                if ((this->shapeMap)[i][j]) {
                    field[this->leftTopCornI + i][this->leftTopCornJ + j] = 't';
                }
            }
        }
    }
    OBlock::OBlock(void){
        if ((gamestuff::ShapeSize::CELLS_IN_COL) > 1 && gamestuff::ShapeSize::CELLS_IN_ROW > 1) {
            (this->shapeMap)[0][0] = 1;
            (this->shapeMap)[0][1] = 1;
            (this->shapeMap)[1][0] = 1;
            (this->shapeMap)[1][1] = 1;
        }
    }
    OBlock::~OBlock() {

    }
    void OBlock::rotate(void) {
        return;
    }
} // namespace gamestuff