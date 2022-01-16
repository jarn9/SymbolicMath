//
//  Implicit.cpp
//  SymbolicMath
//
//  Created by Arnav Joshi on 1/3/22.
//

#include "Implicit.hpp"
#include <memory>

Implicit::Implicit() : e(Expression()) {}

Implicit::Implicit(const Expression& exp) : e(exp) {}

Implicit::Implicit(Axes ax) {
    switch (ax) {
        case Axes::X:
            e = Expression("x");
            break;
        case Axes::Y:
            e = Expression("y");
            break;
        case Axes::Z:
            e = Expression("z");
            break;
    }
}

Implicit::Implicit(const Implicit& im1, const Implicit& im2, Op oper) : e(Expression(im1.e, im2.e, oper)) {}

Matrix2D<Implicit> Implicit::getGradient() {
    auto mat = Matrix2D<Implicit>(3, 1);
    mat[0] = differentiate(Axes::X);
    mat[1] = differentiate(Axes::Y);
    mat[2] = differentiate(Axes::Z);
    return mat;
}

Implicit Implicit::differentiate(Axes var) {
    Implicit newIm = Implicit();
    switch (var) {
        case Axes::X:
            return Implicit(e.differentiate("x"));
            break;
        case Axes::Y:
            return Implicit(e.differentiate("y"));
            break;
        case Axes::Z:
            return Implicit(e.differentiate("z"));
            break;
    }
    return newIm;
}

double Implicit::evaluate(double x, double y, double z) {
    e = e.substitute("x", std::to_string(x));
    e = e.substitute("y", std::to_string(y));
    e = e.substitute("z", std::to_string(z));
    return e.evaluate();
}
