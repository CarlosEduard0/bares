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

#include <iostream>  //std::cout, std::cin
#include <string>    // string
#include <iomanip>   // std::distance
#include <cassert>   // assert
#include <cmath>     // pow
#include <sstream> // std::stringstream
#include <utility>
#include <stack>

#include "Parser.h"

class Evaluator {

    using value_type = long int;
    public:
        struct EvaluatorResult {
            enum code {
                OK = 0,
                DIVISION_BY_ZERO,
                NUMERIC_OVERFLOW
            };

            std::string value_b;
            code type_b;

            explicit EvaluatorResult(std::string v_ = "", code t_ = code::OK)
                    : value_b(std::move(v_)), type_b(t_) {/* empty */}
        };

    private:
        std::vector<Token> expression;
        bool is_operator(Token t);
        bool is_operand(Token t);
        bool is_opening_scope(std::string c);
        bool is_closing_scope(std::string c);
        bool has_higher_precedence(std::string op1, std::string op2);
        bool is_right_association(std::string c);
        int get_precedence(std::string c);

    public:
        Evaluator() = default;
        ~Evaluator() = default;
        Evaluator(const Evaluator &) = delete;
        Evaluator &operator=(const Evaluator &) = delete;
        void infix_to_postfix(std::vector<Token> infix);
        Evaluator::EvaluatorResult execute_operator(std::string op1, std::string op2, Token opr);
        Evaluator::EvaluatorResult evaluate(std::vector<Token>);
};
#endif //BARES_BARES_H