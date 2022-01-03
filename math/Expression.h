//
//  Expression.h
//  SymbolicMath
//
//  Created by Arnav Joshi on 12/24/21.
//
// The Expression is a tree-based class that contains nodes to represent mathematical operations (+-/*, etc)
// The root nodes (leaves) are the variables/scalars
//
// The following is an example of x*y + 3
//
//      plus
//       /\
//  times  3
//   /\
//  x  y

#ifndef Equation_h
#define Equation_h

#include <string>
#include <vector>

enum class Op {
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    SCALAR // leaf type
};

class Node {
public:
    Node(Op nodeType, std::string val, Node* left, Node* right);
    Node(const Node*);
    
    Op nodeType;
    std::string value;
    Node* left;
    Node* right;
    
    static Node* copyNode(const Node*);
    static Node* substitute(Node*, std::string oldVal, std::string newVal);
    static Node* simplify(Node*);
    static Node* differentiate(Node*, const std::string var);
    static Node* operate(Node* lhs, Node* rhs, Op opType);
    static double evaluate(Node*);
};

class Expression {
public:
    Node* root;
    
    Expression();
    Expression(std::string value);
    Expression(const Expression&, const Expression&, Op);
    
    double evaluate();
    Expression simplify();
    Expression substitute(std::string oldValue, std::string newValue);
    Expression differentiate(const std::string variable);
    Expression operator + (const Expression& rhs);
    Expression operator * (const Expression& rhs);
    Expression operator / (const Expression& rhs);
    Expression operator - (const Expression& rhs);
    
private:
    Expression(Node*);
};

std::ostream& operator<<(std::ostream&, const Expression&);
std::ostream& operator<<(std::ostream&, const Node&);

#endif /* Expression_h */
