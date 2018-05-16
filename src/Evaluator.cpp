#include <utility>
#include "../include/Evaluator.h"

bool Evaluator::is_operator(Token t) const {
    return t.type == Token::token_t::OPERATOR;
}

bool Evaluator::is_operand(Token t) const {
    return t.type == Token::token_t::OPERAND;
}

bool Evaluator::is_opening_scope(Token t) const {
    return t.type == Token::token_t::OPENING_SCOPE;
}

bool Evaluator::is_closing_scope(Token t) const {
    return t.type == Token::token_t::CLOSING_SCOPE;
}

std::vector<Token> Evaluator::infix_to_postfix(std::vector<Token> infix) const {
    std::stack<Token> s;
    std::vector<Token> postfix;

    for(const auto &t : infix) {
        if(is_operand(t)) {
            postfix.emplace_back(t);
        } else if(is_opening_scope(t)) {
            s.push(t);
        } else if(is_closing_scope(t)) {
            while(!is_opening_scope(s.top())) {
                postfix.emplace_back(s.top());
                s.pop();
            }
            s.pop();
        } else if(is_operator(t)) {
            while(!s.empty() && has_higher_or_eq_precedence(s.top(), t)) {
                postfix.emplace_back(s.top());
                s.pop();
            }
            s.push(t);
        } else {

        }
    }
    while(!s.empty()) {
        postfix.emplace_back(s.top());
        s.pop();
    }
    return postfix;
}

bool Evaluator::is_right_association(Token t) const {
    return t.value == "^";
}

short Evaluator::get_precedence(Token t) const {
    switch (t.value[0]) {
        case '^':
            return 3;
        case '*':
        case '/':
        case '%':
            return 2;
        case '+':
        case '-':
            return 1;
        case '(':
            return 0;
        default:
            assert(false);
    }
    return -1;
}

bool Evaluator::has_higher_or_eq_precedence(Token op1, Token op2) const {
    return (get_precedence(op1) >= get_precedence(std::move(op2))) ? !is_right_association(op1) : false;
}

Token Evaluator::execute_operator(Token v1, Token v2, Token t) const {

    std::string::size_type sz;              // alias of size_t
    auto valor1 = static_cast<int>(std::stol(v1.value, &sz));
    auto valor2 = static_cast<int>(std::stol(v2.value, &sz));

    switch(t.value[0]) {
        case '^':
            return Token(std::to_string(pow(valor1, valor2)), Token::token_t::OPERAND);
        case '*':
            return Token(std::to_string(valor1 * valor2), Token::token_t::OPERAND);
        case '/':
            if (valor2 == 0) throw std::runtime_error("Division by zero!");
            return Token(std::to_string(valor1 / valor2), Token::token_t::OPERAND);
        case '%':
            if (valor2 == 0) throw std::runtime_error("Division by zero!");
            return Token(std::to_string(valor1 % valor2), Token::token_t::OPERAND);
        case '+':
            return Token(std::to_string(valor1 + valor2), Token::token_t::OPERAND);
        case '-':
            return Token(std::to_string(valor1 - valor2), Token::token_t::OPERAND);
        default:break;
    }

    throw std::runtime_error("Invalid operator!");
}

Token Evaluator::evaluate_postfix(std::vector<Token> postfix) const {
    postfix = infix_to_postfix(postfix);
    std::stack<Token> s;
    for(const Token &t : postfix) {
        if(is_operand(t)) {
            s.push(t);
        } else if(is_operator(t)) {
            auto op2 = s.top(); s.pop();
            auto op1 = s.top(); s.pop();
            s.push(execute_operator(op1, op2, t));
        } else {
            assert(false);
        }
    }
    return s.top();
}