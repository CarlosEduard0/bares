#ifndef _SIMBOLO_H_
#define _SIMBOLO_H_

#include <string>
#include <vector>
#include <algorithm>

typedef enum type {OPERADOR, OPERANDO, DELIMITADOR} Type;

/**
 * \brief Classe que representa os símbolos do sistema.
 */
class Simbolo {
    public:
        std::string value;

        Simbolo(std::string);
        Type getType() const;
};

#endif