// Author: Walerij Hrul

#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <list>
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
        std::vector<std::vector<int>> originalShapeMap;
        bool canMoveSide(const std::list<std::vector<sf::Color>> &field, const int direction) const;
        bool canFall(const std::list<std::vector<sf::Color>> &field) const;
        bool canRotate(std::list<std::vector<sf::Color>> &field, std::vector<std::vector<int>> &newShapeMap) const;
        sf::Color mainColor;
      public:
        Shape(int leftTopCornerI, int leftTopCornerJ, sf::Color color);
        virtual ~Shape();
        void draw(std::list<std::vector<sf::Color>> &field) const;
        void hide(std::list<std::vector<sf::Color>> &field) const;
        bool fall(std::list<std::vector<sf::Color>> &field);
        bool moveSide(std::list<std::vector<sf::Color>> &field, const int direction);
        virtual bool rotate(std::list<std::vector<sf::Color>> &field);
        void setPosition(int leftTopCornerI, int leftTopCornerJ);
        virtual void justForMakingShapeClassAbstract(void) = 0;
        bool canDraw(std::list<std::vector<sf::Color>> &field) const;
        void setOriginalMap(void);
    };
    class OBlock : public Shape {
        void justForMakingShapeClassAbstract(void) override{};
      public:
        OBlock(int leftTopCornerI = 0, int leftTopCornerJ = 0, sf::Color color = sf::Color::Yellow);
        bool rotate(std::list<std::vector<sf::Color>> &field) override;
    };
    class TBlock : public Shape {
        void justForMakingShapeClassAbstract(void) override{};
      public:
        TBlock(int leftTopCornerI = 0, int leftTopCornerJ = 0, sf::Color color = sf::Color::Yellow);
    };
    class ZBlock : public Shape {
        void justForMakingShapeClassAbstract(void) override{};
      public:
        ZBlock(int leftTopCornerI = 0, int leftTopCornerJ = 0, sf::Color color = sf::Color::Yellow);  
    };
    class SBlock : public Shape {
        void justForMakingShapeClassAbstract(void) override{};
      public:
        SBlock(int leftTopCornerI = 0, int leftTopCornerJ = 0, sf::Color color = sf::Color::Yellow);  
    };
    class LBlock : public Shape {
        void justForMakingShapeClassAbstract(void) override{};
      public:
        LBlock(int leftTopCornerI = 0, int leftTopCornerJ = 0, sf::Color color = sf::Color::Yellow);  
    };
    class JBlock : public Shape {
        void justForMakingShapeClassAbstract(void) override{};
      public:
        JBlock(int leftTopCornerI = 0, int leftTopCornerJ = 0, sf::Color color = sf::Color::Yellow);  
    };
    class IBlock : public Shape {
        void justForMakingShapeClassAbstract(void) override{};
      public:
        IBlock(int leftTopCornerI = 0, int leftTopCornerJ = 0, sf::Color color = sf::Color::Yellow);  
    };
} // namespace gamestuff

#endif