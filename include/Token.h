#ifndef BARES_TOKEN_H
#define BARES_TOKEN_H

#include <string>   // std::string
#include <iostream> // std::ostream

/// Represents a token.
struct Token {
public:
    enum class token_t : int {
        OPERAND = 0,   //!< A type representing numbers.
        OPERATOR,      //!< A type representing  "+", "-". "*", "/", "%", "^".
        OPENING_SCOPE, //!< A type representing "(".
        CLOSING_SCOPE  //!< A type representing ")".
    };

    std::string value; //!< The token value as a string.
    token_t type;      //!< The token type, which is either token_t::OPERAND or token_t::OPERATOR.

    /// Construtor default.
    explicit Token(std::string value_ = "", token_t type_ = token_t::OPERAND)
            : value(std::move(value_)), type(type_) {/* empty */}

    /// Just to help us debug the code.
    friend std::ostream &operator<<(std::ostream &os_, const Token &t_) {
        std::string types[] = {"OPERAND", "OPERATOR", "OPENING_SCOPE", "CLOSING_SCOPE"};

        os_ << "<" << t_.value << "," << types[(int) (t_.type)] << ">";

        return os_;
    }
};

#endif //BARES_TOKEN_H
