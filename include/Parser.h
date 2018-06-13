#ifndef BARES_PARSER_H
#define BARES_PARSER_H

#include <iostream> // cout, cin
#include <iterator> // std::distance()
#include <vector>   // std::vector
#include <sstream>  // std::istringstream
#include <stdexcept>
#include <string>
#include <limits>   // std::numeric_limits, para validar a faixa de um inteiro.
#include <algorithm>// std::copy, para copiar substrings.
#include <utility>

#include "Token.h"  // struct Token.

/*!
 * Implements a recursive descendent parser for a EBNF grammar.
 *
 * This class also tokenizes the input expression into its components, creating a list of tokens.
 *
 * The grammar is:
 * ```
 *   <expr>            := <term>,{ ("+"|"-"),<term> };
 *   <term>            := <integer>;
 *   <integer>         := 0 | ["-"],<natural_number>;
 *   <natural_number>  := <digit_excl_zero>,{<digit>};
 *   <digit_excl_zero> := "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9";
 *   <digit>           := "0"| <digit_excl_zero>;
 * ```
 */

class Parser {
    public:

        /// This struct represents the result of the parsing operation.
        struct ResultType {
            //=== Alias
            typedef size_t size_type; /// Used for column location.

            /// List of possible syntax errors.
            enum code_t {
                OK = 0,
                UNEXPECTED_END_OF_EXPRESSION,
                ILL_FORMED_INTEGER,
                MISSING_TERM,
                EXTRANEOUS_SYMBOL,
                MISSING_CLOSING,
                INTEGER_OUT_OF_RANGE
            };

            //=== Members (public).
            code_t type;      //!< Error code.
            size_type at_col; //!< Stores the column number where the error happened.

            /// Default constructor
            explicit ResultType(code_t type_ = OK, size_type col_ = 0u)
                    : type{type_}, at_col{col_} { /* empty */ }
        };

        //==== Aliases
        typedef short int required_int_type;
        typedef long long int input_int_type;

        //==== Public interface
        // Parses and tokenizes an input source expression.  Return the result as a struct.
        ResultType parse(std::string e_);

        /// Retrieves the list of tokens created during the partins process.
        std::vector<Token> get_tokens() const;

        //==== Special methods
        /// Default constructor
        Parser() = default;
        /// Default destructor
        ~Parser() = default;
        /// Turn off copy constructor. We do not need it.
        Parser(const Parser &) = delete;
        /// Turn off assignment operator.
        Parser &operator=(const Parser &) = delete;

    private:

    // Terminal symbols table
        enum class terminal_symbol_t {  // The symbols:-
            TS_PLUS,            //!< code for "+"
            TS_MINUS,           //!< code for "-"
            TS_TIMES,           //!< code for "*"
            TS_SLASH,           //!< code for "/"
            TS_MOD,             //!< code for "%"
            TS_CIRCUMFLEX,      //!< code for "^"
            TS_NON_ZERO_DIGIT,  //!< code for digits "1"->"9"
            TS_ZERO,            //!< code for "0"
            TS_WS,              //!< code for a white-space
            TS_EOS,             //!< code for "End Of String"
            TS_TAB,             //!< code for tab
            TS_INVALID,         //!< invalid token
            TS_OPENING_SCOPE,   //!< code for "("
            TS_CLOSING_SCOPE   //!< code for ")"
        };

        //==== Private members.
        std::string expr;                   //!< The source expression to be parsed
        std::string::iterator it_curr_symb; //!< Pointer to the current char inside the expression.
        std::vector<Token> token_list;      //!< Resulting list of tokens extracted from the expression.

        terminal_symbol_t lexer(char) const;

    //=== Support methods.
        void next_symbol();
        bool peek(terminal_symbol_t s_) const;
        bool accept(terminal_symbol_t s_);
        bool expect(terminal_symbol_t);
        void skip_ws();
        bool end_input() const;

    //=== NTS methods.
        ResultType expression();
        ResultType term();
        ResultType integer();
        ResultType natural_number();
        bool digit_excl_zero();
        bool digit();
};

#endif //BARES_PARSER_H
