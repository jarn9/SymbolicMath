//
//  Equation.cpp
//  SymbolicMath
//
//  Created by Arnav Joshi on 12/24/21.
//

#include <stdio.h>
#include <iostream>
#include "Expression.h"

// CONSTRUCTORS
Expression::Expression(std::string value_) {
    root = new Node(Op::SCALAR, value_, NULL, NULL);
}

Expression::Expression() {
    root = new Node(Op::SCALAR, "", NULL, NULL);
}

Expression::Expression(const Expression& lhs, const Expression& rhs, Op opType) {
    root = new Node(opType, "", lhs.root, rhs.root);
    root = Node::simplify(root);
}

Expression::Expression(Node* n) {
    root = new Node(n);
}

// CLASS METHODS
Expression Expression::substitute(std::string oldValue, std::string newValue){
    return Expression(Node::substitute(root, oldValue, newValue));
}

Expression Expression::simplify(){
    root = Node::simplify(root);
    auto newExp = Expression(root);
    return newExp;
}

double Expression::evaluate(){
    return Node::evaluate(root);
}

Expression Expression::differentiate(const std::string var){
    Node* newRoot = Node::differentiate(root, var);
    Expression newExp = Expression(newRoot);
        return newExp.simplify();
}

Expression Expression::operator + (const Expression& other){
    return Expression(*this, other, Op::ADD);
}

Expression Expression::operator * (const Expression& other){
    return Expression(*this, other, Op::MULTIPLY);
}

Expression Expression::operator - (const Expression& other){
    return Expression(*this, other, Op::SUBTRACT);
}

Expression Expression::operator / (const Expression& other){
    return Expression(*this, other, Op::DIVIDE);
}

// OTHER METHODS
std::ostream& operator<<(std::ostream& os, const Expression& exp){
    os << *exp.root;
    return os;
}
