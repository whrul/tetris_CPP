#include "tests.hpp"

namespace gamestuff {
    void Tests::completeAllTests(void) {
        Tests::checkFieldAndShapesSizes();
    }
    void Tests::checkFieldAndShapesSizes(void) {
        assert(gamestuff::FieldSize::CELL_SIZE > 0 &&
               gamestuff::ShapeSize::MAX_CELLS_IN_COL > 0 &&
               gamestuff::ShapeSize::MAX_CELLS_IN_ROW > 0 &&
               gamestuff::ShapeSize::MAX_CELLS_IN_COL < gamestuff::FieldSize::HEIGHT / gamestuff::FieldSize::CELL_SIZE &&
               gamestuff::ShapeSize::MAX_CELLS_IN_ROW < gamestuff::FieldSize::WIDTH / gamestuff::FieldSize::CELL_SIZE);
        std::cout << "checkFieldAndShapesSizes() passed.\n";
    }
} // namespace gamestuff