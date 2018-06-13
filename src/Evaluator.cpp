#include "Evaluator.h"
#include <utility>

//!< Verifica se o token é um operador
bool Evaluator::is_operator(Token t) {

    return t.type == Token::token_t::OPERATOR;
}

//!< Verifica se o token é um operando
bool Evaluator::is_operand(Token t) {
    return t.type == Token::token_t::OPERAND;
}

//!< Verifica o caractere informado é um parênteses aberto
bool Evaluator::is_opening_scope(std::string c) {
    return (c == "(");
}

//!< Verifica o caractere informado é um parênteses aberto
bool Evaluator::is_closing_scope(std::string c) {
    return (c == ")");
}

//!< Verifica se é associação a direita
bool Evaluator::is_right_association(std::string c) {
    return c == "^";
}

//!< Executa uma operação
Evaluator::EvaluatorResult Evaluator::execute_operator(std::string op1, std::string op2, Token opr) {

    std::string resultadoFinal;

    std::string::size_type sz;   // alias size_t
    auto num1 = static_cast<int>(std::stol(op1, &sz));
    auto num2 = static_cast<int>(std::stol(op2, &sz));

    value_type resultado(0);
    Evaluator::EvaluatorResult e;

    //converte o Token para const char
    const char *d = (opr.value).c_str();
    switch (*d) {
        case '^' :
            resultado = static_cast<value_type>( pow(num1, num2));
            break;
        case '*' :
            resultado = num1 * num2;
            break;
        case '/' :
            if (num2 == 0) {
                e.type_b = Evaluator::EvaluatorResult::DIVISION_BY_ZERO;
                return e;
            } else
                resultado = num1 / num2;
            break;
        case '%' :
            if (num2 == 0) {
                e.type_b = Evaluator::EvaluatorResult::DIVISION_BY_ZERO;
                return e;
            } else
                resultado = num1 % num2;
            break;
        case '+' :
            resultado = num1 + num2;
            break;
        case '-' :
            resultado = num1 - num2;
            break;
        default:
            assert(false);
    }

    if (resultado <= std::numeric_limits<Parser::required_int_type>::max()
        and resultado >= std::numeric_limits<Parser::required_int_type>::min()) {

        std::stringstream ss;
        ss << resultado;
        e.value_b = ss.str();
        e.type_b = Evaluator::EvaluatorResult::OK;

    } else
        e.type_b = Evaluator::EvaluatorResult::NUMERIC_OVERFLOW;

    return e;


}

//!< Executa uma expressão
Evaluator::EvaluatorResult Evaluator::evaluate(std::vector<Token> infix) {

    infix_to_postfix(std::move(infix));
    std::stack<std::string> s;
    Evaluator::EvaluatorResult resultado;

    for (Token ch: expression) {
        if (is_operand(ch)) s.push(ch.value);

        else if (is_operator(ch)) {
            auto op2 = s.top(); s.pop();
            auto op1 = s.top(); s.pop();

            resultado = execute_operator(op1, op2, ch);
            if (resultado.type_b != Evaluator::EvaluatorResult::OK)
                return resultado;
            else
                s.push(resultado.value_b);
        } else {
            assert(false);
        }
    }

    resultado.value_b = s.top();

    return resultado;
}

//!< Converte a expressão infixa para posfixa
void Evaluator::infix_to_postfix(std::vector<Token> infix) {
    std::string postfix;
    std::stack<std::string> s;

    for (Token c : infix) {
        if (is_operand(c)) {
            Token add;
            add.type = c.type;
            add.value = c.value;
            expression.push_back(add);;
        } else if (is_operator(c)) {
            //Remove elementos com prioridade superior
            while (not s.empty() and has_higher_precedence(s.top(), c.value)) {
                Token add;
                add.type = c.type;
                add.value = s.top(); s.pop();
                expression.push_back(add);
            }
            // Colocando o operador na fila
            s.push(c.value);
        } else if (is_opening_scope(c.value)) {
            s.push(c.value);
        } else if (is_closing_scope(c.value)) {
            //Remove todos os elementos exceto '('
            while (not is_opening_scope(s.top()) and not s.empty()) {
                Token add;
                add.type = Token::token_t::OPERATOR;
                add.value = s.top(); s.pop();
                expression.push_back(add);
            }
            s.pop();
        }
    }

    while (not s.empty()) {
        Token add;
        add.type = Token::token_t::OPERATOR;
        add.value = s.top(); s.pop();
        expression.push_back(add);
    }
}

//!< Precedências das operações
int Evaluator::get_precedence(std::string c) {
    int prioridade = 0;
    const char *d = c.c_str();
    switch (*d) {
        case '^':
            prioridade = 3;
            break;
        case '*':
            prioridade = 2;
            break;
        case '/':
            prioridade = 2;
            break;
        case '%':
            prioridade = 2;
            break;
        case '+':
            prioridade = 1;
            break;
        case '-':
            prioridade = 1;
            break;
        case '(':
            prioridade = 0;
            break;
        default:
            assert(false);
    }

    return prioridade;
}

//!< Verificar precedência dos operadores
bool Evaluator::has_higher_precedence(std::string op1, std::string op2) {

    int p1 = get_precedence(op1);
    int p2 = get_precedence(std::move(op2));

    if (p1 == p2 and is_right_association(op1))
        return false;

    return p1 >= p2;
}