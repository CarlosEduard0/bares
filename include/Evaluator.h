#ifndef BARES_BARES_H
#define BARES_BARES_H

/*!
 * Converts an arithmetic expression in infix notation into postfix
 * representation. After the convertion is successfyly done, the
 * program evaluates the expression based on stacks and queues.
 *
 * Input Postfix expression must be in a desired format.
 * Operands and operator, **both must be single character**.
 * Only '+', '-', '*', '%', '/', and '^' (for exponentiation) operators are expected;
 * Any other character is just ignored.
 */

#include <iostream>  // cout, endl
#include <stack>     // stack
#include <string>    // string
#include <cassert>   // assert
#include <cmath>     // pow
#include <stdexcept> // std::runtime_error
#include <vector>
#include "Token.h"

class Evaluator {
    //=== Aliases
    using value_type = long int; //!< Type we operate on.

public:
    Token evaluate_postfix(std::vector<Token>) const;
private:
    bool is_operator(Token t) const;
    bool is_operand(Token t) const;
    bool is_opening_scope(Token t) const;
    bool is_closing_scope(Token t) const;
    std::vector<Token> infix_to_postfix(std::vector<Token>) const;
    bool is_right_association(Token t) const;
    short get_precedence(Token t) const;
    bool has_higher_or_eq_precedence(Token op1, Token op2) const;
    Token execute_operator(Token v1, Token v2, Token t) const;
};


#endif //BARES_BARES_H
