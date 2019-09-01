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
        CELLS_IN_ROW = 4,
        CELLS_IN_COL = 4,
    };
    class Shape{
      protected:
        int leftTopCornI;
        int leftTopCornJ;
        std::vector<std::vector<unsigned int>> shapeMap;
        bool canMoveSide(const std::list<std::vector<sf::Color>> &field, const int direction) const;
        bool canFall(const std::list<std::vector<sf::Color>> &field) const;
      public:
        Shape(void);
        virtual ~Shape();
        void draw(std::list<std::vector<sf::Color>> &field) const;
        void hide(std::list<std::vector<sf::Color>> &field) const;
        virtual void rotate(void) = 0;
        
        bool fall(std::list<std::vector<sf::Color>> &field);
        bool moveSide(std::list<std::vector<sf::Color>> &field, const int direction);
    };
    class OBlock : public Shape {
      public:
        OBlock(void);
        ~OBlock();
        void rotate(void) override;
    };
} // namespace gamestuff

#endif