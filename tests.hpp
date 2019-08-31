// Author: Walerij Hrul

#ifndef TESTS_HPP
#define TESTS_HPP

#include "game.hpp"
#include "shape.hpp"
#include <cassert>

namespace gamestuff{
    class Tests {
      public:
        static void completeAllTests(void);
        static void checkFieldAndShapesSizes(void);
    };
} // namespace gamestuff

#endif