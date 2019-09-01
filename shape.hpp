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
        std::vector<std::vector<int>> shapeMap;
        bool canMoveSide(const std::list<std::vector<sf::Color>> &field, const int direction) const;
        bool canFall(const std::list<std::vector<sf::Color>> &field) const;
        bool canRotate(std::list<std::vector<sf::Color>> &field, std::vector<std::vector<int>> &newShapeMap);
      public:
        Shape(int leftTopCornerI, int leftTopCornerJ);
        virtual ~Shape();
        void draw(std::list<std::vector<sf::Color>> &field) const;
        void hide(std::list<std::vector<sf::Color>> &field) const;
        bool fall(std::list<std::vector<sf::Color>> &field);
        bool moveSide(std::list<std::vector<sf::Color>> &field, const int direction);
        virtual bool rotate(std::list<std::vector<sf::Color>> &field);
        void setPosition(int leftTopCornerI, int leftTopCornerJ);
        virtual void justForMakingShapeClassAbstract(void) = 0;
    };
    class OBlock : public Shape {
        void justForMakingShapeClassAbstract(void) override{};
      public:
        OBlock(int leftTopCornerI = 0, int leftTopCornerJ = 0);
        ~OBlock();
        bool rotate(std::list<std::vector<sf::Color>> &field) override;
    };
    class TBlock : public Shape {
        void justForMakingShapeClassAbstract(void) override{};
      public:
        TBlock(int leftTopCornerI = 0, int leftTopCornerJ = 0);
        ~TBlock();
        
    };
    class ZBlock : public Shape {
        void justForMakingShapeClassAbstract(void) override{};
      public:
        ZBlock(int leftTopCornerI = 0, int leftTopCornerJ = 0);
        ~ZBlock();
         
    };
} // namespace gamestuff

#endif