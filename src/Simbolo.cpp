#include "Simbolo.h"

/**
 * \brief Construtor da classe que recebe como parâmetro um operador ou operando ou delimitador.
 * \param value Valor do símbolo em formato de string;
 */
Simbolo::Simbolo(std::string value) {
    this->value = value;
}

/**
 * \brief Método que indentificado o tipo de símbolo. Tipos de símbolos: operador, operando, delimitador.
 * \return Type Tipo de símbolo que está armazenado no atributo value.
 */
Type Simbolo::getType() const {
    std::vector<std::string> operadores = {"+", "-", "*", "/", "%", "^"};
    if(std::find(operadores.begin(), operadores.end(), this->value) != operadores.end()) {
        return OPERADOR;
    } else if(this->value == "(" || this->value == ")") {
        return DELIMITADOR;
    } else {
        return OPERANDO;
    }
}