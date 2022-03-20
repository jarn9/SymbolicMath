//
//  main.cpp
//  SymbolicMath
//
//  Created by Arnav Joshi on 12/24/21.
//

#include <iostream>
#include "Implicit.hpp"
#include "Matrix.hpp"
#include "Operations.h"
#include "Expression.h"
#include "ExpressionTests.t.h"

int main(int argc, const char * argv[]) {
    Implicit x = Implicit(Axes::X);
    auto x2 = Implicit(Op::MULTIPLY, std::vector<Implicit>{x, x});
    Implicit y = Implicit(Axes::Y);
    auto y2 = Implicit(Op::MULTIPLY, std::vector<Implicit>{y, y});
    auto x2py2 = Implicit(Op::ADD, std::vector<Implicit>{x2, y2});
    auto shape = Implicit(Op::SUBTRACT, {x2py2, Implicit(3.0)});
    
    std::cout << shape << std::endl;
    std::cout << shape.getZeros(Matrix2D<double>(3, 1, 1)) << std::endl;
    
//    test_operators();
//    test_simplify();
//    test_differentiate(true);
//    test_memory_leaks();
    return 0;
}
