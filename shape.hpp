// Author: Walerij Hrul

#ifndef SHAPE_HPP
#define SHAPE_HPP

#include<list>
#include <vector>
#include <cstdbool>
#include <SFML/Graphics.hpp>
#include <iostream>

namespace gamestuff {
    enum ShapeSize {
        MAX_CELLS_IN_ROW = 4,
        MAX_CELLS_IN_COL = 4,
    };
    class Shape{
      protected:
        int leftTopCornI;
        int leftTopCornJ;
        std::vector<std::vector<unsigned int>> shapeMap;
        bool canMoveSide(const std::list<std::vector<sf::Color>> &field, const int direction) const;
        bool canFall(const std::list<std::vector<sf::Color>> &field) const;
      public:
        Shape(int leftTopCornerI, int leftTopCornerJ);
        virtual ~Shape();
        void draw(std::list<std::vector<sf::Color>> &field) const;
        void hide(std::list<std::vector<sf::Color>> &field) const;
        virtual void rotate(void) = 0;
        bool fall(std::list<std::vector<sf::Color>> &field);
        bool moveSide(std::list<std::vector<sf::Color>> &field, const int direction);
        void setPosition(int leftTopCornerI, int leftTopCornerJ);
    };
    class OBlock : public Shape {
      public:
        OBlock(int leftTopCornerI = 0, int leftTopCornerJ = 0);
        ~OBlock();
        void rotate(void) override;
    };
    class TBlock : public Shape {
      public:
        TBlock(int leftTopCornerI = 0, int leftTopCornerJ = 0);
        ~TBlock();
        void rotate(void) override;
    };
    class ZBlock : public Shape {
      public:
        ZBlock(int leftTopCornerI = 0, int leftTopCornerJ = 0);
        ~ZBlock();
        void rotate(void) override;
    };
} // namespace gamestuff

#endif