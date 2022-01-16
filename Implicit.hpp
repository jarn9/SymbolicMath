//
//  Implicit.hpp
//  SymbolicMath
//
//  Created by Arnav Joshi on 1/3/22.
//

#ifndef Implicit_hpp
#define Implicit_hpp

#include <stdio.h>
#include "Expression.h"
#include "Matrix.hpp"

enum class Axes {
    X,
    Y,
    Z
};

class Implicit {
public:
    Implicit();
    Implicit(Axes);
    Implicit(const Implicit&, const Implicit&, Op);
    
    Matrix2D<Implicit> getGradient();
    Implicit differentiate(Axes var);
    double evaluate(double x, double y, double z);
private:
    Expression e;
    Implicit(const Expression& exp);
};


#endif /* Implicit_hpp */
