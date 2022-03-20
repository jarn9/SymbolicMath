//
//  Node.cpp
//  SymbolicMath
//
//  Created by Arnav Joshi on 1/1/22.
//

#include <stdio.h>
#include <iostream>
#include "Expression.h"

Node::Node(Op nodeType_, std::string val, std::vector< std::shared_ptr<Node> > children_):
nodeType(nodeType_), value(val), children(children_){}

void Node::substitute(const std::string& oldVal, const std::string& newVal) {
    if (value == oldVal) {
        value = newVal;
    }
}

double Node::evaluate() {
    return stod(value);
}
