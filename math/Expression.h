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

class Expression {
public:
    enum class Type {
        ADD,
        SUBTRACT,
        MULTIPLY,
        DIVIDE,
        SCALAR // leaf type
    };
    Type nodeType;
    std::string value;
    Expression* left;
    Expression* right;
    
    Expression();
    Expression(std::string);
    Expression(Type opType);
    Expression(const Expression&, const Expression&, Type);
    Expression(const Expression&);
    double evaluate();
    
    Expression substitute(std::string oldValue, std::string newValue);
    Expression differentiate(std::string variable);
    Expression operator + (const Expression& rhs);
    Expression operator * (const Expression& rhs);
    
private:
    Expression* copyExp(const Expression*);
    static void subHelper(Expression*, std::string oldValue, std::string newValue);
    Expression* simplify(Expression*);
};

std::ostream& operator<<(std::ostream& os, const Expression& exp);

#endif /* Expression_h */
