#ifndef ICARUS_TABLE_H
#define ICARUS_TABLE_H

#include "data/token.h"

#define IC_LEX_TABLE_LEN() (sizeof table / sizeof table[0])

struct ic_table_entry {
    char *str;
    unsigned int len;
    enum ic_token_id id;
};

/* a map of tokens to their ids */
struct ic_table_entry table[] = {
    /* no entry for identifier */

    /* str,         len,    id */
    {"\n", 1, IC_NEWLINE},
    {"\r", 1, IC_WHITESPACE},
    {"\t", 1, IC_WHITESPACE},
    {" ", 1, IC_WHITESPACE},

    {"True", 4, IC_TRUE},
    {"False", 5, IC_FALSE},

    {"end", 3, IC_END},

    {"begin", 5, IC_BEGIN},
    {"if", 2, IC_IF},
    {"elsif", 5, IC_ELSIF},
    {"else", 4, IC_ELSE},
    {"return", 6, IC_RETURN},
    {"let", 3, IC_LET},
    {"for", 3, IC_FOR},
    {"in", 2, IC_IN},
    {"while", 5, IC_WHILE},
    {"match", 5, IC_MATCH},
    {"case", 4, IC_CASE},

    {"import", 6, IC_IMPORT},
    {"builtin", 7, IC_BUILTIN},
    {"op", 2, IC_OP},
    {"fn", 2, IC_FUNC},
    {"type", 4, IC_TYPE},
    {"union", 5, IC_UNION},
    {"enum", 4, IC_ENUM},

    {"->", 2, IC_ARROW},
    {"==", 2, IC_EQUAL},
    {"=", 1, IC_ASSIGN},
    {"::", 2, IC_DOUBLECOLON},
    {".", 1, IC_PERIOD},
    {",", 1, IC_COMMA},

    /* TODO FIXME
     * here we require a space after the `and` and `or` operators
     * to distinguish from their functions `and(` and `or(`
     */
    {"and ", 4, IC_AND},
    {"or ", 3, IC_OR},

    {"/", 1, IC_DIVIDE},

    {"$", 1, IC_DOLLAR},
    {"%", 1, IC_PERCENT},
    {"&", 1, IC_AMPERSAND},
    {"@", 1, IC_AT},
    {"*", 1, IC_ASTERISK},
    {"^", 1, IC_CARET},
    {"+", 1, IC_PLUS},
    {"-", 1, IC_MINUS},

    {"(", 1, IC_LRBRACKET},
    {")", 1, IC_RRBRACKET},

    {"[", 1, IC_LSBRACKET},
    {"]", 1, IC_RSBRACKET},

    {"<=", 2, IC_LESSTHAN_EQUAL},
    {">=", 2, IC_GREATERTHAN_EQUAL},
    {"<", 1, IC_LESSTHAN},
    {">", 1, IC_GREATERTHAN},

    {"", 1, IC_TOKEN_LEN},

};

#endif
