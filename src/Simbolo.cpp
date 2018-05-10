#include "../include/Simbolo.h"

///Construtor da classe que recebe como parâmetro um operador ou operando ou delimitador.
Simbolo::Simbolo(std::string value) {
    this->value = value;
}