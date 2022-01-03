//
//  Node.cpp
//  SymbolicMath
//
//  Created by Arnav Joshi on 1/1/22.
//

#include <stdio.h>
#include <iostream>
#include "Expression.h"

Node::Node(Op nodeType_, std::string val, Node* left_, Node* right_):
nodeType(nodeType_), value(val), left(copyNode(left_)), right(copyNode(right_)){}

Node::Node(const Node* n) {
    value = n->value;
    nodeType = n->nodeType;
    left = copyNode(n->left);
    right = copyNode(n->right);
}

Node* Node::copyNode(const Node* n) {
    if (!n) {
        return NULL;
    }
    
    return new Node(n->nodeType, n->value, copyNode(n->left), copyNode(n->right));;
}

Node* Node::substitute(Node* n, std::string oldVal, std::string newVal) {
    if (!n) {
        return NULL;
    }
    
    if (n->value == oldVal) {
        n->value = newVal;
    }
    
    n->left = Node::substitute(n->left, oldVal, newVal);
    n->right = Node::substitute(n->right, oldVal, newVal);
    
    return n;
}

Node* Node::simplify(Node* n) {
    if (!n) {
        return NULL;
    }
    
    n->left = Node::simplify(n->left);
    n->right = Node::simplify(n->right);
    
    if (n->nodeType == Op::SCALAR) {
        return n;
    }
    
    if (n->nodeType == Op::MULTIPLY and n->left->value == "0") { // times 0
        n->right = n->left->right;
        n->nodeType = n->left->nodeType;
        n->value = n->left->value;
        n->left = n->left->left;
    } else if (n->nodeType == Op::MULTIPLY and n->right->value == "0") {
        n->left = n->right->left;
        n->nodeType = n->right->nodeType;
        n->value = n->right->value;
        n->right = n->right->right;
    } else if (n->nodeType == Op::MULTIPLY and n->left->value == "1") { // times 1
        n->left = n->right->left;
        n->nodeType = n->right->nodeType;
        n->value = n->right->value;
        n->right = n->right->right;
    } else if (n->nodeType == Op::MULTIPLY and n->right->value == "1") {
        n->right = n->left->right;
        n->nodeType = n->left->nodeType;
        n->value = n->left->value;
        n->left = n->left->left;
    } else if (n->nodeType == Op::ADD and n->left->value == "0") { // add 0
        n->left = n->right->left;
        n->nodeType = n->right->nodeType;
        n->value = n->right->value;
        n->right = n->right->right;
    } else if (n->nodeType == Op::ADD and n->right->value == "0") {
        n->right = n->left->right;
        n->nodeType = n->left->nodeType;
        n->value = n->left->value;
        n->left = n->left->left;
    }
    return n;
}

Node* Node::differentiate(Node* n, const std::string var) {
    if (n->nodeType == Op::SCALAR) {
        auto newNode = new Node(n);
        if (n->value == var) {
            newNode->value = "1";
        } else {
            newNode->value = "0";
        }
        return newNode;
    }
    
    auto fprime = differentiate(n->left, var);
    auto gprime = differentiate(n->right, var);
    auto fprimeg = operate(fprime, n->right, Op::MULTIPLY);
    auto gprimef = operate(n->left, gprime, Op::MULTIPLY);
    switch (n->nodeType) {
        case Op::SCALAR:
            std::cout << "Not possible" << std::endl;
            return n;
        case Op::ADD:
            return operate(differentiate(n->left, var), differentiate(n->right, var), Op::ADD);
        case Op::SUBTRACT:
            return operate(differentiate(n->left, var), differentiate(n->right, var), Op::SUBTRACT);
        case Op::MULTIPLY:
            return operate(fprimeg, gprimef, Op::ADD);
        case Op::DIVIDE:
            auto numerator = operate(fprimeg, gprimef, Op::SUBTRACT);
            auto gsquared = operate(n->right, n->right, Op::MULTIPLY);
            return operate(numerator, gsquared, Op::DIVIDE);
    }
}

Node* Node::operate(Node* lhs, Node* rhs, Op opType){
    return new Node(opType, "", Node::copyNode(lhs), Node::copyNode(rhs));
}

double Node::evaluate(Node* n) {
    switch (n->nodeType) {
        case Op::SCALAR:
            return stod(n->value);
        case Op::ADD:
            return Node::evaluate(n->left) + Node::evaluate(n->right);
        case Op::SUBTRACT:
            return Node::evaluate(n->left) - Node::evaluate(n->right);
        case Op::MULTIPLY:
            return Node::evaluate(n->left) * Node::evaluate(n->right);
        case Op::DIVIDE:
            return Node::evaluate(n->left) / Node::evaluate(n->right);
    }
}

std::ostream& operator<<(std::ostream& os, const Node& n){
    if (n.left) {
        os << "(";
        os << *n.left;
    }
    
    switch (n.nodeType) {
        case Op::SCALAR:
            os << n.value;
            break;
        case Op::ADD:
            os << " + ";
            break;
        case Op::MULTIPLY:
            os << "*";
            break;
        case Op::DIVIDE:
            os << "/";
            break;
        case Op::SUBTRACT:
            os << " - ";
            break;
    }
    
    if (n.right) {
        os << *n.right;
        os << ")";
    }
    
    return os;
}
