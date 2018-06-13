#include <iostream>  // cout, endl
#include <sstream>   // getline
#include <stack>     // stack
#include <string>    // string
#include <iomanip>   //setfill, setw
#include <fstream>

#include "Parser.h"
#include "Evaluator.h"

using value_type = long int;

//!< Imprime na tela mensagens auxiliares de erro do bares
void print_msg_bares(const Evaluator::EvaluatorResult &result) {
    switch (result.type_b) {
        case Evaluator::EvaluatorResult::DIVISION_BY_ZERO:
            std::cout << "Division by zero!\n";
            break;
        case Evaluator::EvaluatorResult::NUMERIC_OVERFLOW:
            std::cout << "Numeric overflow error!\n";
            break;
        default:
            break;
    }
}

//!< Imprime mensagens de erro de sintaxe da expressão
void print_msg(const Parser::ResultType &result) {
    switch (result.type) {
        case Parser::ResultType::UNEXPECTED_END_OF_EXPRESSION:
            std::cout << "Unexpected end of expression at column (" << result.at_col << ")!\n";
            break;
        case Parser::ResultType::ILL_FORMED_INTEGER:
            std::cout << "Ill formed integer at column (" << result.at_col << ")!\n";
            break;
        case Parser::ResultType::MISSING_TERM:
            std::cout << "Missing <term> at column (" << result.at_col << ")!\n";
            break;
        case Parser::ResultType::EXTRANEOUS_SYMBOL:
            std::cout << "Extraneous symbol after valid expression found at column (" << result.at_col << ")!\n";
            break;
        case Parser::ResultType::MISSING_CLOSING:
            std::cout << "Missing closing \")\" at column (" << result.at_col << ")!\n";
            break;
        case Parser::ResultType::INTEGER_OUT_OF_RANGE:
            std::cout << "Integer constant out of range beginning at column (" << result.at_col << ")!\n";
            break;
        default:
            break;
    }
}

//!< Leitura do arquivo
std::vector<std::string> read_file(const std::string &fileName) {
    std::vector<std::string> expressions;
    std::fstream file(fileName);
    if (file.is_open()) {
        while (!file.eof()) {
            std::string line;
            getline(file, line);
            expressions.push_back(line);
        }
        file.close();
    } else {
        std::cerr << "Não foi possível lê o arquivo.\n";
        exit(1);
    }
    return expressions;
}

//!< Método principal
int main(int argc, char *argv[]) {
    if(argc < 2) {
        std::cerr << "Use: ./bares <entrada>\n";
        return EXIT_FAILURE;
    }

    std::vector<std::string> expressions = read_file(argv[1]);

    Parser my_parser;

    for (std::string &expr : expressions) {
        Parser::ResultType result = my_parser.parse(expr);
        if (result.type != Parser::ResultType::OK)
            print_msg(result);
        else {
            std::vector<Token> lista = my_parser.get_tokens();

            Evaluator eval;
            auto resultado = eval.evaluate(lista);

            if (resultado.type_b != Evaluator::EvaluatorResult::OK)
                print_msg_bares(resultado);
            else
                std::cout << resultado.value_b << std::endl;
        }

    }
    return EXIT_SUCCESS;
}
