//
//  Equation.cpp
//  SymbolicMath
//
//  Created by Arnav Joshi on 12/24/21.
//

#include <stdio.h>
#include <iostream>
#include <math.h>
#include <memory>
#include "Expression.h"

// CONSTRUCTORS
Expression::Expression() {
    root = std::shared_ptr<Node>(NULL);
}

Expression::Expression(const std::string& value_) {
    root = std::make_shared<Node>(Op::SCALAR, value_, std::vector< std::shared_ptr<Node> >(0));
}

Expression::Expression(Op opType, const std::vector<Expression>& operands) {
    auto newChildren = std::vector< std::shared_ptr<Node> >(operands.size());
    for (int i = 0; i < operands.size(); ++i) {
        newChildren[i] = operands[i].root;
    }
    root = std::make_shared<Node>(opType, std::string(), newChildren);
    simplifyHelper(root);
}

Expression::Expression(const Expression& rhs) {
    root = copyTree(rhs.root);
}

Expression Expression::operator = (const Expression& rhs) {
    if (this == &rhs) {
        return *this;
    }
    
    root = copyTree(rhs.root);
    return *this;
}

Expression::Expression(std::shared_ptr<Node> n) {
    root = n;
}

// CLASS METHODS
Expression Expression::substitute(const std::string& oldValue, const std::string& newValue) {
    return Expression(substituteHelper(copyTree(root), oldValue, newValue));
}

Expression Expression::simplify() {
    return Expression(simplifyHelper(copyTree(root)));
}

double Expression::evaluate() {
    return evaluateHelper(root);
}

Expression Expression::differentiate(const std::string& var) {
    auto newExp = Expression(differentiateHelper(copyTree(root), var));
    simplifyHelper(newExp.root);
    return newExp;
}

Expression Expression::operator + (const Expression& other) {
    std::vector<Expression> newChildren = {*this, other};
    return Expression(Op::ADD, newChildren);
}

Expression Expression::operator * (const Expression& other) {
    std::vector<Expression> newChildren = {*this, other};
    return Expression(Op::MULTIPLY, newChildren);
}

Expression Expression::operator - (const Expression& other) {
    std::vector<Expression> newChildren = {*this, other};
    return Expression(Op::SUBTRACT, newChildren);
}

Expression Expression::operator / (const Expression& other) {
    std::vector<Expression> newChildren = {*this, other};
    return Expression(Op::DIVIDE, newChildren);
}

bool Expression::operator == (const Expression& rhs) const {
    return equalityHelper(root, rhs.root);
}

// STATIC HELPER FUNCTIONS //

std::shared_ptr<Node> Expression::simplifyHelper(std::shared_ptr<Node> n) {
    if (!n) {
        return std::shared_ptr<Node>(NULL);
    }
    
    for (auto& child : n->children) {
        child = Expression::simplifyHelper(child);
    }
    
    std::shared_ptr<Node> toDelete = std::shared_ptr<Node>(NULL);
    std::shared_ptr<Node> toKeep = n;
    if (n->nodeType == Op::MULTIPLY and n->children[0]->value == "0") { // times 0
        toDelete = n->children[1];
        toKeep = n->children[0];
    } else if (n->nodeType == Op::MULTIPLY and n->children[1]->value == "0") {
        toDelete = n->children[0];
        toKeep = n->children[1];
    } else if (n->nodeType == Op::MULTIPLY and n->children[0]->value == "1") { // times 1
        toDelete = n->children[0];
        toKeep = n->children[1];
    } else if (n->nodeType == Op::MULTIPLY and n->children[1]->value == "1") {
        toDelete = n->children[1];
        toKeep = n->children[0];
    } else if (n->nodeType == Op::ADD and n->children[0]->value == "0") { // add 0
        toDelete = n->children[0];
        toKeep = n->children[1];
    } else if (n->nodeType == Op::ADD and n->children[1]->value == "0") {
        toDelete = n->children[1];
        toKeep = n->children[0];
    } else if (n->nodeType == Op::SUBTRACT and n->children[1]->value == "0") { // subtract 0
        toDelete = n->children[1];
        toKeep = n->children[0];
    } else if (n->nodeType == Op::DIVIDE and n->children[1]->value == "1") { // divide 1
        toDelete = n->children[1];
        toKeep = n->children[0];
    } else if (n->nodeType == Op::SUBTRACT and Expression(n->children[1]) == Expression(n->children[0])) { // same minus same
        n->children = std::vector< std::shared_ptr<Node> >(0);
        n->nodeType = Op::SCALAR;
        n->value = "0";
        return n;
    }
    
    n->children = toKeep->children;
    n->value = toKeep->value;
    n->nodeType = toKeep->nodeType;
    return n;
}

double Expression::evaluateHelper(std::shared_ptr<Node> n) {
    switch (n->nodeType) {
        case Op::SCALAR:
            return n->evaluate();
        case Op::ADD:
            return Expression::evaluateHelper(n->children[0]) + Expression::evaluateHelper(n->children[1]);
        case Op::SUBTRACT:
            return Expression::evaluateHelper(n->children[0]) - Expression::evaluateHelper(n->children[1]);
        case Op::MULTIPLY:
            return Expression::evaluateHelper(n->children[0]) * Expression::evaluateHelper(n->children[1]);
        case Op::DIVIDE:
            return Expression::evaluateHelper(n->children[0]) / Expression::evaluateHelper(n->children[1]);
        case Op::MIN:
            return std::min(Expression::evaluateHelper(n->children[0]), Expression::evaluateHelper(n->children[1]));
        case Op::MAX:
            return std::max(Expression::evaluateHelper(n->children[0]), Expression::evaluateHelper(n->children[1]));
        case Op::SIN:
            return sin(Expression::evaluateHelper(n->children[0]));
        case Op::COS:
           return cos(Expression::evaluateHelper(n->children[0]));
        case Op::TAN:
           return tan(Expression::evaluateHelper(n->children[0]));
    }
}

std::shared_ptr<Node> Expression::substituteHelper(std::shared_ptr<Node> n, const std::string& oldVal, const std::string& newVal) {
    if (!n) {
        return std::shared_ptr<Node>(NULL);
    }
    
    for (auto& child : n->children) {
        child = substituteHelper(child, oldVal, newVal);
    }
    
    n->substitute(oldVal, newVal);
    return n;
}

std::shared_ptr<Node> Expression::differentiateHelper(std::shared_ptr<Node> n, const std::string& var) {
    if (!n) {
        return std::shared_ptr<Node>(NULL);
    }

    if (n->nodeType == Op::SCALAR) {
        if (n->value == var) {
            n->value = "1";
        } else {
            n->value = "0";
        }
        assert(n->children.size() == 0);
        return n;
    } else if (n->nodeType == Op::ADD) {
        n->children[0] = differentiateHelper(n->children[0], var);
        n->children[1] = differentiateHelper(n->children[1], var);
        return n;
    } else if (n->nodeType == Op::SUBTRACT) {
        n->children[0] = differentiateHelper(n->children[0], var);
        n->children[1] = differentiateHelper(n->children[1], var);
        return n;
    } else if (n->nodeType == Op::MULTIPLY) {
        auto fprime = differentiateHelper(copyTree(n->children[0]), var);
        auto gprime = differentiateHelper(copyTree(n->children[1]), var);
        auto v1 = {fprime, copyTree(n->children[1])};
        auto fprimeg = std::make_shared<Node>(Op::MULTIPLY, "", v1);
        auto v2 = {gprime, copyTree(n->children[0])};
        auto gprimef = std::make_shared<Node>(Op::MULTIPLY, "", v2);
        auto v3 = {fprimeg, gprimef};
        auto newNode = std::make_shared<Node>(Op::ADD, "", v3);
        return newNode;
    } else if (n->nodeType == Op::DIVIDE) {
        auto fprime = differentiateHelper(copyTree(n->children[0]), var);
        auto gprime = differentiateHelper(copyTree(n->children[1]), var);
        auto v1 = {fprime, copyTree(n->children[1])};
        auto fprimeg = std::make_shared<Node>(Op::MULTIPLY, "", v1);
        auto v2 = {gprime, copyTree(n->children[0])};
        auto gprimef = std::make_shared<Node>(Op::MULTIPLY, "", v2);
        auto v3 = {fprimeg, gprimef};
        auto numerator = std::make_shared<Node>(Op::SUBTRACT, "", v3);
        auto v4 = {copyTree(n->children[1]), copyTree(n->children[1])};
        auto gsquared = std::make_shared<Node>(Op::MULTIPLY, "", v4);
        auto v5 = {numerator, gsquared};
        return std::make_shared<Node>(Op::DIVIDE, "", v5);
    }
//    else if (n->nodeType == Op::MAX) {
//        (fprime + gprime + (fprime - gprime)*sign(f - g))/2
//    }
    std::cout << "Error: operator derivative not implemented" << std::endl;
    assert(false);
    return std::shared_ptr<Node>(NULL);
}

bool Expression::equalityHelper(const std::shared_ptr<Node> a, const std::shared_ptr<Node> b) {
    if (!a and !b) {
        return true;
    }
    
    if (a->value != b->value) {
        return false;
    }
    
    if (a->nodeType != b->nodeType) {
        return false;
    }
    
    if (a->children.size() != b->children.size()) {
        return false;
    }
    
    bool result = true;
    for (int i = 0; i < a->children.size(); ++i) {
        result = result and equalityHelper(a->children[i], b->children[i]);
    }
    
    return result;
}

std::shared_ptr<Node> Expression::copyTree(const std::shared_ptr<Node> node) {
    if (!node) {
        return std::shared_ptr<Node>(NULL);
    }
    
    std::vector< std::shared_ptr<Node> > newChildren(node->children.size());
    for (int i = 0; i < node->children.size(); ++i) {
        newChildren[i] = copyTree(node->children[i]);
    }
    
    if (node->nodeType == Op::SCALAR) {
        assert(newChildren.size() == 0);
    }
    
    return std::make_shared<Node>(node->nodeType, node->value, newChildren);
}

// OTHER METHODS
std::ostream& operator<<(std::ostream& os, const Expression& exp){
    os << exp.root;
    return os;
}

std::ostream& operator<<(std::ostream& os, const std::shared_ptr<Node> n) {
    switch (n->nodeType) {
        case Op::SCALAR:
            os << n->value;
            break;
        case Op::ADD:
            os << "(" << n->children[0] << "+" << n->children[1] << ")";
            break;
        case Op::MULTIPLY:
            os << "(" << n->children[0] << "*" << n->children[1] << ")";
            break;
        case Op::DIVIDE:
            os << "(" << n->children[0] << "/" << n->children[1] << ")";
            break;
        case Op::SUBTRACT:
            os << "(" << n->children[0] << "-" << n->children[1] << ")";
            break;
        case Op::MIN:
            os << "min(" << n->children[0] << ", " << n->children[1] << ")";
            break;
        case Op::MAX:
            os << "max(" << n->children[0] << ", " << n->children[1] << ")";
            break;
        case Op::SIN:
            os << "sin(" << n->children[0] << ")";
            break;
        case Op::COS:
            os << "cos(" << n->children[0] << ")";
            break;
        case Op::TAN:
            os << "tan(" << n->children[0] << ")";
            break;
    }
    return os;
}
