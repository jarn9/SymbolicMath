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
Expression::Expression(std::string value_):
nodeType(Type::SCALAR), value(value_), left(nullptr), right(nullptr) {}

Expression::Expression():
nodeType(Type::SCALAR), value(""), left(nullptr), right(nullptr) {}

Expression::Expression(Expression::Type opType):
nodeType(opType), value(""), left(nullptr), right(nullptr) {}

Expression::Expression(const Expression& lhs, const Expression& rhs, Expression::Type opType):
nodeType(opType), value(""){
    left = Expression::copyExp(&lhs);
    right = Expression::copyExp(&rhs);
    
    auto simplified = Expression::simplify(this);
    nodeType = simplified->nodeType;
    value = simplified->value;
    left = simplified->left;
    right = simplified->right;
}

Expression::Expression(const Expression& oldExp):
nodeType(oldExp.nodeType), value(oldExp.value){
    left = Expression::copyExp(oldExp.left);
    right = Expression::copyExp(oldExp.right);
}

Expression* Expression::copyExp(const Expression* oldExp){
    if (!oldExp) {
        return NULL;
    }
    
    Expression* newExp = new Expression(oldExp->nodeType);
    newExp->value = oldExp->value;
    newExp->left = copyExp(oldExp->left);
    newExp->right = copyExp(oldExp->right);
    
    return newExp;
}

// CLASS METHODS
Expression Expression::substitute(std::string oldValue, std::string newValue){
    Expression newExp = Expression(*this);
    subHelper(&newExp, oldValue, newValue);
    return newExp;
}

void Expression::subHelper(Expression* expression, std::string oldValue, std::string newValue){
    if (!expression) {
        return;
    }
    
    if (expression->value == oldValue) {
        expression->value = newValue;
    }
    
    expression->subHelper(expression->left, oldValue, newValue);
    expression->subHelper(expression->right, oldValue, newValue);
}

Expression* Expression::simplify(Expression* exp){
    if (!exp) {
        return NULL;
    }
    
    exp->left = simplify(exp->left);
    exp->right = simplify(exp->right);
    
    if (!exp->left or !exp->right){
        return exp;
    }
    
    // TIMES 0
    if (exp->right->value == "0" and exp->nodeType == Expression::Type::MULTIPLY){
        return exp->right;
    }

    if (exp->left->value == "0" and exp->nodeType == Expression::Type::MULTIPLY){
        return exp->left;
    }
    
    // TIMES 1
    if (exp->right->value == "1" and exp->nodeType == Expression::Type::MULTIPLY){
        return exp->left;
    }

    if (exp->left->value == "1" and exp->nodeType == Expression::Type::MULTIPLY){
        return exp->right;
    }
    
    // ADD 0
    if (exp->left->value == "0" and exp->nodeType == Expression::Type::ADD){
        return exp->right;
    }
    
    if (exp->right->value == "0" and exp->nodeType == Expression::Type::ADD){
        return exp->left;
    }
    
    return exp;
}

double Expression::evaluate(){
    switch (nodeType){
        case Type::MULTIPLY:
            return left->evaluate()*right->evaluate();
        case Type::DIVIDE:
            return left->evaluate()/right->evaluate();
        case Type::ADD:
            return left->evaluate() + right->evaluate();
        case Type::SUBTRACT:
            return left->evaluate() - right->evaluate();
        case Type::SCALAR:
            return stof(value);
    }
}

Expression Expression::differentiate(std::string variable){
    if (nodeType == Type::SCALAR) {
        if (value == variable) {
            return Expression("1");
        }
        return Expression("0");
    }
    
    Expression dleft = left->differentiate(variable);
    Expression dright = right->differentiate(variable);
    Expression fpg = Expression(dleft, *right, Type::MULTIPLY);
    Expression fgp = Expression(*left, dright, Type::MULTIPLY);

    if (nodeType == Type::MULTIPLY) {
        return Expression(fpg, fgp, Type::ADD);
    }
    
    if (nodeType == Type::DIVIDE) {
        return Expression(fpg, fgp, Type::ADD);
    }
    
    if (nodeType == Type::ADD) {
        return Expression(dleft, dright, Type::ADD);
    }
    
    // if (nodeType == Type::SUBTRACT)
    return Expression(dleft, dright, Type::SUBTRACT);
}

Expression Expression::operator+ (const Expression& other){
    return Expression(*this, other, Type::ADD);
}

Expression Expression::operator* (const Expression& other){
    return Expression(*this, other, Type::MULTIPLY);
}

// OTHER METHODS
std::ostream& operator<<(std::ostream& os, const Expression& exp){
    if (exp.left){
        os << "(";
        os << *exp.left;
    }
    
    switch (exp.nodeType) {
        case Expression::Type::MULTIPLY:
            os << "*";
            break;
        case Expression::Type::DIVIDE:
            os << "/";
            break;
        case Expression::Type::ADD:
            os << " + ";
            break;
        case Expression::Type::SUBTRACT:
            os << " - ";
            break;
        case Expression::Type::SCALAR:
            os << exp.value;
            break;
    }
    
    if (exp.right) {
        os << *exp.right;
        os << ")";
    }
    
    return os;
}
