#include "Parser.h"

/// Converts the input character c_ into its corresponding terminal symbol code.
Parser::terminal_symbol_t Parser::lexer(char c_) const {
    switch (c_) {
        case '+':
            return terminal_symbol_t::TS_PLUS;
        case '-':
            return terminal_symbol_t::TS_MINUS;
        case '%':
            return terminal_symbol_t::TS_MOD;
        case '/':
            return terminal_symbol_t::TS_SLASH;
        case '*':
            return terminal_symbol_t::TS_TIMES;
        case '^':
            return terminal_symbol_t::TS_CIRCUMFLEX;
        case ')':
            return terminal_symbol_t::TS_CLOSING_SCOPE;
        case '(':
            return terminal_symbol_t::TS_OPENING_SCOPE;
        case ' ':
            return terminal_symbol_t::TS_WS;
        case 9:
            return terminal_symbol_t::TS_TAB;
        case '0':
            return terminal_symbol_t::TS_ZERO;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return terminal_symbol_t::TS_NON_ZERO_DIGIT;
        case '\0':
            return terminal_symbol_t::TS_EOS; // end of string: the $ terminal symbol
        default:
            break;
    }
    return terminal_symbol_t::TS_INVALID;
}

/// Consumes a valid character from the input source expression.
void Parser::next_symbol() {
    // Advances iterator to the next valid symbol for processing
    std::advance(it_curr_symb, 1);
}

/// Checks whether we reached the end of the input expression string.
bool Parser::end_input() const {
    // "Fim de entrada" ocorre quando o iterador chega ao
    // fim da string que guarda a expressão.
    return it_curr_symb == expr.end(); // Stub
}

/// Returns the result of trying to match the current character with c_, **without** consuming the current character from the input expression.
bool Parser::peek(terminal_symbol_t c_) const {
    // Checks whether the input symbol is equal to the argument symbol.
    return (not end_input()) and lexer(*it_curr_symb) == c_;
}

/// Returns the result of trying to match (peek()) and consume the current character with c_.
/*!
 * If the match is not successful, the current character (from the input) **is not consumed**.
 * @param c_ the current character from the input source expression we wish to parse.
 * @see peek().
 * @return true if we got a successful match; false otherwise.
 */
bool Parser::accept(terminal_symbol_t c_) {
    // If we have a match, we consume the character from the input source expression.
    // caractere da entrada.
    if (peek(c_)) {
        next_symbol();
        return true;
    }
    return false;
}

/// Skips all white spaces and tries to accept() the next valid character. @see accept().
bool Parser::expect(terminal_symbol_t c_) {
    // Skip all white spaces first.
    skip_ws();
    return accept(c_); // Stub
}

/// Ignores any white space or tabs in the expression until reach a valid character or end of input.
void Parser::skip_ws() {
    // Skip white spaces, while at the same time, check for end of string.
    while ((!end_input()) &&
           ((lexer(*it_curr_symb) == Parser::terminal_symbol_t::TS_WS) ||
            (lexer(*it_curr_symb) == Parser::terminal_symbol_t::TS_TAB))) {
        next_symbol();
    }
}

//=== Non Terminal Symbols (NTS) methods.

/// Validates (i.e. returns true or false) and consumes an expression from the input string.
/*! This method parses a valid expression from the input and, at the same time, it tokenizes its components.
 *
 * Production rule is:
 * ```
 *  <expr> := <term>,{ ("+"|"-"),<term> };
 * ```
 * An expression might be just a term or one or more terms with '+'/'-' between them.
 */
Parser::ResultType Parser::expression() {
    skip_ws();
    auto result = term();
    while (result.type == ResultType::OK) {

        if (expect(terminal_symbol_t::TS_PLUS)) {
            token_list.emplace_back("+", Token::token_t::OPERATOR);
        } else if (expect(terminal_symbol_t::TS_MINUS)) {
            token_list.emplace_back("-", Token::token_t::OPERATOR);
        } else if (expect(terminal_symbol_t::TS_TIMES)) {
            token_list.emplace_back("*", Token::token_t::OPERATOR);
        } else if (expect(terminal_symbol_t::TS_SLASH)) {
            token_list.emplace_back("/", Token::token_t::OPERATOR);
        } else if (expect(terminal_symbol_t::TS_MOD)) {
            token_list.emplace_back("%", Token::token_t::OPERATOR);
        } else if (expect(terminal_symbol_t::TS_CIRCUMFLEX)) {
            token_list.emplace_back("^", Token::token_t::OPERATOR);
        } else {
            return result;
        }

        result = term();
        if (result.type != ResultType::OK and result.type != ResultType::INTEGER_OUT_OF_RANGE and end_input()) {
            result.type = ResultType::MISSING_TERM;
            return result;
        }
    }

    return result;
}

/// Validates (i.e. returns true or false) and consumes a term from the input string.
/*! This method parses a valid term from the input.
 *
 * Production rule is:
 * ```
 *  <term> := <integer>;
 * ```
 * A term is made of a single integer.
 *
 * @return true if a term has been successfuly parsed from the input; false otherwise.
 */
Parser::ResultType Parser::term() {
    skip_ws();
    std::string::iterator it_begin = it_curr_symb;

    ResultType resultado;
    if (expect(terminal_symbol_t::TS_OPENING_SCOPE)) {
        token_list.emplace_back("(", Token::token_t::OPENING_SCOPE);
        resultado = expression();

        if (resultado.type == ResultType::OK) {
            if (not expect(terminal_symbol_t::TS_CLOSING_SCOPE))
                return ResultType(ResultType::MISSING_CLOSING,
                                  static_cast<ResultType::size_type>(std::distance(expr.begin(), it_curr_symb) + 1));

            token_list.emplace_back(")", Token::token_t::CLOSING_SCOPE);
        }
    } else {
        resultado = integer();
        if (resultado.type == ResultType::OK) {
            std::string num;
            num.insert(num.begin(), it_begin + resultado.at_col, it_curr_symb);

            std::string::iterator end_pos = std::remove(num.begin(), num.end(), ' ');
            num.erase(end_pos, num.end());

            input_int_type value = std::stoll(num);
            if (value <= std::numeric_limits<Parser::required_int_type>::max()
                && value >= std::numeric_limits<Parser::required_int_type>::min()) {
                token_list.emplace_back(num, Token::token_t::OPERAND);
            } else {
                resultado.type = ResultType::INTEGER_OUT_OF_RANGE;
                resultado.at_col = static_cast<ResultType::size_type>(std::distance(expr.begin(), it_begin) + 1);
            }
        }
    }

    return resultado;
}

/// Validates (i.e. returns true or false) and consumes an integer from the input string.
/*! This method parses a valid integer from the input and, at the same time, add the integer to the token list.
 *
 * Production rule is:
 * ```
 * <integer> := 0 | ["-"],<natural_number>;
 * ```
 * A integer might be a zero or a natural number, which, in turn, might begin with an unary minus.
 *
 * @return true if an integer has been successfuly parsed from the input; false otherwise.
 */
Parser::ResultType Parser::integer() {
    if (accept(terminal_symbol_t::TS_ZERO)) {
        return ResultType(ResultType::OK);
    }

    int cont = 0;
    while (expect(terminal_symbol_t::TS_MINUS)) {
        cont++;
    }

    if (cont % 2 == 1)
        cont -= 1;

    auto resultado = natural_number();

    if (resultado.type == ResultType::OK)
        resultado.at_col = static_cast<ResultType::size_type>(cont);

    return resultado;

}

/// Validates (i.e. returns true or false) and consumes a natural number from the input string.
/*! This method parses a valid natural number from the input.
 *
 * Production rule is:
 * ```
 * <natural_number> := <digit_excl_zero>,{<digit>};
 * ```
 *
 * @return true if a natural number has been successfuly parsed from the input; false otherwise.
 */
Parser::ResultType Parser::natural_number() {
    bool resultado = digit_excl_zero();
    if (!resultado)
        return ResultType(ResultType::ILL_FORMED_INTEGER,
                          static_cast<ResultType::size_type>(std::distance(expr.begin(), it_curr_symb) + 1));
    while (resultado) {
        resultado = digit();
    }
    return ResultType(ResultType::OK);
}

/// Validates (i.e. returns true or false) and consumes a non-zero digit from the input string.
/*! This method parses a single valid non-zero digit from the input.
 *
 * Production rule is:
 * ```
 * <digit_excl_zero> := "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9";
 * ```
 *
 * @return true if a non-zero digit has been successfuly parsed from the input; false otherwise.
 */
bool Parser::digit_excl_zero() {
    return accept(terminal_symbol_t::TS_NON_ZERO_DIGIT);
}

/// Validates (i.e. returns true or false) and consumes a zero digit from the input string.
/*! This method parses a single valid digit from the input.
 *
 * Production rule is:
 * ```
 * <digit> := "0"| <digit_excl_zero>;
 * ```
 *
 * @return true if a digit has been successfuly parsed from the input; false otherwise.
 */
bool Parser::digit() {
    return (accept(terminal_symbol_t::TS_ZERO) or digit_excl_zero());
}

/*!
 * This is the parser's entry point.
 * This method tries to (recursivelly) validate an expression.
 * During this process, we also store the tokens into a container.
 *
 * \param e_ The string with the expression to parse.
 * \return The parsing result.
 *
 * @see ResultType
 */
Parser::ResultType Parser::parse(std::string e_) {
    expr = std::move(e_);
    it_curr_symb = expr.begin();
    token_list.clear();
    ResultType resultado(ResultType::OK);
    skip_ws();
    if (end_input()) {
        return ResultType(ResultType::UNEXPECTED_END_OF_EXPRESSION,
                          static_cast<ResultType::size_type>(std::distance(expr.begin(), it_curr_symb) + 1));
    }
    resultado = expression();
    if (resultado.type == ResultType::OK) {
        skip_ws();
        if (!end_input()) {
            token_list.clear();
            return ResultType(ResultType::EXTRANEOUS_SYMBOL,
                              static_cast<ResultType::size_type>(std::distance(expr.begin(), it_curr_symb) + 1));
        }

    }
    return resultado;
}

/// Return the list of tokens, which is the by-product created during the syntax analysis.
std::vector<Token> Parser::get_tokens() const {
    return token_list;
}



//==========================[ Fim do parse.cpp ]==========================//
