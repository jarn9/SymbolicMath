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

#ifndef Expression_h
#define Expression_h

#include <string>
#include <vector>
#include "Operations.h"

class Node {
public:
    Node(Op nodeType, std::string val, std::vector< std::shared_ptr<Node> > children);
    
    Op nodeType;
    std::string value;
    std::vector< std::shared_ptr<Node> > children;
    
    void substitute(const std::string& oldVal, const std::string& newVal);
    double evaluate();
};

class Expression {
public:
    Expression();
    Expression(const std::string&);
    Expression(Op, const std::vector<Expression>&);
    Expression(const Expression&);
    Expression operator = (const Expression&);
    
    virtual double evaluate();
    Expression simplify();
    Expression substitute(const std::string& oldValue, const std::string& newValue);
    Expression differentiate(const std::string& variable);
    Expression operator + (const Expression& rhs);
    Expression operator * (const Expression& rhs);
    Expression operator / (const Expression& rhs);
    Expression operator - (const Expression& rhs);
    bool operator == (const Expression& rhs) const;
    
    std::shared_ptr<Node> root;
    
protected:
    Expression(std::shared_ptr<Node>);
    
    static std::shared_ptr<Node> simplifyHelper(std::shared_ptr<Node>);
    static std::shared_ptr<Node> substituteHelper(std::shared_ptr<Node>, const std::string&, const std::string&);
    static std::shared_ptr<Node> differentiateHelper(std::shared_ptr<Node>, const std::string&);
    static double evaluateHelper(std::shared_ptr<Node>);
    static bool equalityHelper(const std::shared_ptr<Node> a, const std::shared_ptr<Node> b);
    static std::shared_ptr<Node> copyTree(const std::shared_ptr<Node>);
};

std::ostream& operator<<(std::ostream&, const Expression&);
std::ostream& operator<<(std::ostream&, const std::shared_ptr<Node>);

#endif /* Expression_h */
