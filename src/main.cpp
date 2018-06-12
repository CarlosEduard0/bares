#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>

#include "../include/Parser.h"
#include "../include/Evaluator.h"

void print_error_msg(const Parser::ResultType &result, const std::string &str) {
    std::string error_indicator(str.size() + 1, ' ');

    // Have we got a parsing error?
    error_indicator[result.at_col] = '^';
    switch (result.type) {
        case Parser::ResultType::UNEXPECTED_END_OF_EXPRESSION:
            std::cout << "Unexpected end of input at column (" << result.at_col + 1 << ")!\n";
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
        case Parser::ResultType::INTEGER_OUT_OF_RANGE:
            std::cout << "Integer constant out of range beginning at column (" << result.at_col + 1 << ")!\n";
            break;
        case Parser::ResultType::MISSING_CLOSING:
            std::cout << "Mising closing \")\" at column (" << result.at_col + 1 << ")!\n";
            break;
        case Parser::ResultType::DIVISION_ZERO:
            std::cout << "Division by zero!\n";
            break;
        case Parser::ResultType::NUMERIC_OVERFLOW:
            std::cout << "Numeric overflow error!\n";
            break;
        default:
            std::cout << "Unhandled error found!\n";
            break;
    }

    /*std::cout << "\"" << str << "\"\n";
    std::cout << " " << error_indicator << std::endl;*/
}

std::vector<std::string> read_file(const std::string &fileName) {
    std::vector<std::string> expressions;
    std::fstream file(fileName);
    if(file.is_open()) {
        while(!file.eof()) {
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

int main(int argc, char *argv[]) {
    if(argc < 2) {
        std::cerr << "Use: ./bares <entrada>\n";
        return EXIT_FAILURE;
    }
    std::vector<std::string> expressions = read_file(argv[1]);

    Parser my_parser; // Instancia um parser.
    // Tentar analisar cada expressão da lista.
    for (const auto &expr : expressions) {
        // Fazer o parsing desta expressão.
        auto result = my_parser.parse(expr);
        // Preparar cabeçalho da saida.
        /*std::cout << std::setfill('=') << std::setw(80) << "\n";
        std::cout << std::setfill(' ') << ">>> Parsing \"" << expr << "\"\n";*/
        // Se deu pau, imprimir a mensagem adequada.
        if (result.type != Parser::ResultType::OK) {
            print_error_msg(result, expr);
            continue;
        } /*else {
            std::cout << ">>> Expression SUCCESSFULLY parsed!\n";
        }*/
        // Recuperar a lista de tokens.
        auto lista = my_parser.get_tokens();
        /*std::cout << ">>> Tokens: { ";
        std::copy(lista.begin(), lista.end(), std::ostream_iterator<Token>(std::cout, " "));
        std::cout << "}\n";*/

        Evaluator bares;
        Token resultado = bares.evaluate_postfix(lista);
        std::cout /*<< ">>> Result is: "*/ << resultado.value << std::endl;
    }

    std::cout << "\n>>> Normal exiting...\n";

    return EXIT_SUCCESS;
}