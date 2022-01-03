//
//  main.cpp
//  SymbolicMath
//
//  Created by Arnav Joshi on 12/24/21.
//

#include <iostream>
#include "Expression.h"
#include "Matrix.hpp"

int main(int argc, const char * argv[]) {
    Expression x = Expression("x");
    auto y = Expression("y");
    
    auto x2 = x*x;
    auto y2 = y*y;
    auto F = x2 + y2;

    auto Fx = F.differentiate("x");
    auto Fy = F.differentiate("y");

    auto grad = Matrix2D<Expression>(2, 1);
    grad(0,0) = Fx;
    grad(1,0) = Fy;

    std::cout << grad << std::endl;
    return 0;
}
