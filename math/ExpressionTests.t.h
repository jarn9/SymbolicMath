//
//  ExpressionTests.t.cpp
//  SymbolicMath
//
//  Created by Arnav Joshi on 1/8/22.
//

#ifndef ExpressionTests_h
#define ExpressionTests_h

#include <vector>
#include <stdio.h>
#include "Expression.h"

void test_simplify() {
    auto zero = Expression("0");
    auto one = Expression("1");
    auto x = Expression("x");
    auto y = Expression("y");
    
    auto f1 = x - x;
    assert(f1 == zero);
    
    auto f2 = x*zero;
    assert(f2 == zero);
    
    auto f3 = x + zero;
    assert(f3 == x);
    
    assert(zero + zero == zero);
    assert(zero - zero == zero);
    assert(one - one == zero);
    assert(zero*zero == zero);
}

void test_differentiate(bool verbose=false) {
    auto zero = Expression("0");
    auto one = Expression("1");
    auto x = Expression("x");
    auto y = Expression("y");
    
    
    std::vector<Expression> expressions;
    expressions.push_back(Expression("1"));
    expressions.push_back(Expression("0"));
    expressions.push_back(Expression("y"));
    expressions.push_back(x * x);
    expressions.push_back(x + x);
    expressions.push_back(x / x);
    expressions.push_back(x / y);
    
    std::vector<std::string> variables;
    variables.push_back("x");
    variables.push_back("x");
    variables.push_back("y");
    variables.push_back("x");
    variables.push_back("x");
    variables.push_back("x");
    variables.push_back("x");
    
    std::vector<Expression> expected_results;
    expected_results.push_back(zero);
    expected_results.push_back(zero);
    expected_results.push_back(one);
    expected_results.push_back(x + x);
    expected_results.push_back(one + one);
    expected_results.push_back(zero/(x*x));
    expected_results.push_back(y/(y*y));
    
    // Equation
    for (int i = 0; i < expressions.size(); ++i) {
        auto diff = expressions[i].differentiate(variables[i]);
        
        if (verbose) {
            std::cout << "Expression: " << expressions[i] << std::endl;
            std::cout << "Expected der: " << expected_results[i] << std::endl;
            std::cout << "Actual der: " << diff << std::endl;
            std::cout << std::endl;
        }
        
        assert(diff == expected_results[i]);
    }
}

void test_operators() {
    auto zero = Expression("0");
    assert(zero == Expression("0"));
    
    // ==
    // =
    // +, -, /, *
    // operate()
}

void test_memory_leaks() {
    auto x = Expression("x");
    auto F1 = x/x + x*x - x;
    int vecSize = 100;
    std::vector<Expression> ins(vecSize);
    for (int j = 0; j < 10000; ++j) {
        for (int i = 0; i < vecSize; ++i) {
            ins[i] = F1.differentiate("x");
        }
    }
}

#endif
