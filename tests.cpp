#include "tests.hpp"

namespace gamestuff {
    void Tests::completeAllTests(void) {
        Tests::checkFieldAndShapesSizes();
    }
    void Tests::checkFieldAndShapesSizes(void) {
        assert(FieldSize::CELL_SIZE > 0 &&
               ShapeSize::MAX_CELLS_IN_COL > 0 &&
               ShapeSize::MAX_CELLS_IN_ROW > 0 &&
               ShapeSize::MAX_CELLS_IN_COL < FieldSize::CELLS_IN_COL &&
               ShapeSize::MAX_CELLS_IN_ROW < FieldSize::CELLS_IN_ROW);
        // std::cout << "test checkFieldAndShapesSizes() passed.\n";
    }
} // namespace gamestuff