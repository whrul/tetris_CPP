// Author: Walerij Hrul

#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <vector>
#include "game.hpp"

namespace gamestuff {
    enum ShapeSize {
        CELLS_IN_ROW = 4,
        CELLS_IN_COL = 4,
    };
    class Shape{
      protected:
        int leftTopCornI;
        int leftTopCornJ;
        std::vector<std::vector<unsigned char>> shapeMap;
      public:
        Shape(void);
        virtual ~Shape();
        void draw(std::vector<std::vector<unsigned char>> &field) const;
        virtual void rotate(void) = 0;
    };
    class OBlock : public Shape {
      public:
        OBlock(void);
        ~OBlock();
        void rotate(void) override;
    };
} // namespace gamestuff

#endif