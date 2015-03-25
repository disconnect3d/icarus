#include <stdio.h> /* puts */
#include <stdlib.h> /* strtol */
#include <errno.h> /* errno */
#include <limits.h> /* LONG_MIN, LONG_MAX */

#include "../parse.h"

/* ignore unused parameter warnings */
#pragma GCC diagnostic ignored "-Wunused-parameter"

/* ignore unused functions */
#pragma GCC diagnostic ignored "-Wunused-function"

/* ignore unused variables */
#pragma GCC diagnostic ignored "-Wunused-variable"

/* ignore unused variables that are set */
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"

#ifndef DEBUG_PARSE_EXPR
#define DEBUG_PARSE_EXPR
#endif

/* current supported expression types:
 *  func call
 *  identifier
 *  constant
 *  operator application
 */


/* consume token
 * returns ic_expr* on success
 * returns 0 on failure
 */
static struct ic_expr * ic_parse_expr_fcall(struct ic_tokens *tokens, unsigned int *i){
    /* our eventual return value */
    struct ic_expr * expr = 0;
    /* used for getting the distance of our function name */
    unsigned int dist = 0;
    /* temporary used for capturing the argument expression */
    struct ic_expr * arg = 0;

    if( ! tokens ){
        puts("ic_parse_expr_fcall: tokens was null");
        return 0;
    }
    if( ! i ){
        puts("ic_parse_expr_fcall: i was null");
        return 0;
    }

    /* build our return expr */
    expr = ic_expr_new(ic_expr_type_func_call);
    if( ! expr ){
        puts("ic_parse_expr_fcall: call to ic_expr_new failed");
        return 0;
    }

    /* find our function name */
    dist = ic_parse_token_length(tokens->tokens, *i);
    if( ! dist ){
        puts("ic_parse_expr_fcall: failed to get distance of function name");
        return 0;
    }

    /* build our function */
    if( ic_expr_func_call_init( &(expr->u.fcall), &(tokens->tokens[*i]), dist ) ){
        puts("ic_parse_expr_fcall: call to ic_expr_func_call_init failed");
        return 0;
    }

    /* skip over function name */
    ic_parse_token_advance(i, dist);

    /* skip over opening ( */
    if( ic_parse_check_token("(", 1, tokens->tokens, i) ){
        puts("ic_parse_expr_fcall: failed to find opening bracket '('");
        return 0;
    }

    /* keep consuming arguments until we see the closing ) */
    while( ic_parse_check_token(")", 1, tokens->tokens, i) ){
        /* parse this argument */
        arg = ic_parse_expr(tokens, i);
        if( ! arg ){
            puts("ic_parse_expr_fcall: parsing arg error, call to ic_parse_expr failed");
            return 0;
        }

        /* store it inside our function */
        if( ic_expr_func_call_add_arg( &(expr->u.fcall), arg ) ){
            puts("ic_parse_expr_fcall: call to ic_expr_func_call_add_arg failed");
            return 0;
        }
    }

    /* ic_parse_check_token will skip over the closing ) for us */

    /* victory */
    return expr;
}

/* consume token
 * returns ic_expr* on success
 * returns 0 on failure
 */
static struct ic_expr * ic_parse_expr_identifier(struct ic_tokens *tokens, unsigned int *i){
    /* our final return expr */
    struct ic_expr * expr = 0;
    /* pointer to our internal id */
    struct ic_expr_identifier *id = 0;
    /* dist of our identifier */
    unsigned int dist = 0;

    if( ! tokens ){
        puts("ic_parse_expr_identifier: tokens was null");
        return 0;
    }
    if( ! i ){
        puts("ic_parse_expr_identifier: i was null");
        return 0;
    }

    /* build our new expr */
    expr = ic_expr_new(ic_expr_type_identifier);
    if( ! expr ){
        puts("ic_parse_expr_identifier: call to ic_expr_new failed");
        return 0;
    }

    /* fetch our internal id */
    id = ic_expr_get_identifier(expr);
    if( ! id ){
        puts("ic_parse_expr_identifier: call to ic_expr_get_identifier failed");
        return 0;
    }

    /* find the distance for our token */
    dist = ic_parse_token_length(tokens->tokens, *i);
    if( ! dist ){
        puts("ic_parse_expr_identifier: call to ic_parse_token_length failed");
        return 0;
    }

    /* initialise our id */
    if( ic_expr_identifier_init(id, &(tokens->tokens[*i]), dist) ){
        puts("ic_parse_expr_identifier: call to ic_expr_identifier_init failed");
        return 0;
    }

    /* advance past identifier */
    ic_parse_token_advance(i, dist);

    /* victory */
    return expr;
}

/* consume token and make a string
 * returns ic_expr* on success
 * returns 0 on failure
 */
static struct ic_expr * ic_parse_expr_constant_string(struct ic_tokens *tokens, unsigned int *i){
    /* our eventual return value */
    struct ic_expr * expr = 0;
    /* the i value that marks the beginning of our string
     * this is the offset *after* the opening " is read
     */
    char *start = 0;
    /* length of string */
    unsigned int length = 0;

    /* pointer to our constant */
    struct ic_expr_constant *cons = 0;
    /* pointer to our ic_string */
    struct ic_string *string = 0;

    if( ! tokens ){
        puts("ic_parse_expr_constant_string: tokens was null");
        return 0;
    }
    if( ! i ){
        puts("ic_parse_expr_constant_string: i was null");
        return 0;
    }

    /* check for opening quote */
    if( tokens->tokens[*i] != '"' ){
        puts("ic_parse_expr_constant_string: failed to find opening quote (\")");
        return 0;
    }

    /* build our return expr */
    expr = ic_expr_new(ic_expr_type_constant);
    if( ! expr ){
        puts("ic_parse_expr_constant_string: call to ic_expr_new failed");
        return 0;
    }

    /* unpack our constant */
    cons = ic_expr_get_constant(expr);
    if( ! cons ){
        puts("ic_parse_expr_constant_string: call to ic_expr_get_constant failed");
        return 0;
    }

    /* initialise our constant */
    if( ic_expr_constant_init(cons, ic_expr_constant_type_string) ){
        puts("ic_parse_expr_constant_string: call to ic_expr_constant_init failed");
        return 0;
    }

    /* get out our ic_string */
    string = ic_expr_constant_get_string(cons);
    if( ! string ){
        puts("ic_parse_expr_constant_string: call to ic_expr_constant_get_string failed");
        return 0;
    }

    /* skip over opening " */
    ++*i;

    /* record our starting value */
    start = &(tokens->tokens[*i]);

    printf("parse_string looking at '%s'\n'", &(tokens->tokens[*i + length]));

    /* find length of string
     * FIXME naive
     */
    for( length=0; tokens->tokens[*i + length] != '"'; ++length ) ;

    /* skip over string */
    *i += length;

    /* also skip over closing " */
    ++ *i;

    /* skip over inter-token delim */
    ++ *i;

    /* initialise our ic_string
     * we have the start of the string (start)
     * and the total length (dist_sum)
     */
    if( ic_string_init(string, start, length) ){
        puts("ic_parse_expr_constant_string: call to ic_string_init failed");
        return 0;
    }


    /* victory */
    return expr;
}

/* consume token and make an int
 * returns ic_expr* on success
 * returns 0 on failure
 */
static struct ic_expr * ic_parse_expr_constant_integer(struct ic_tokens *tokens, unsigned int *i){
    /* our eventual return value */
    struct ic_expr * expr = 0;
    /* iterator through string to check all characters are numeric */
    unsigned int iter = 0;
    /* distance of token */
    unsigned int dist = 0;
    /* our constant */
    struct ic_expr_constant *cons = 0;
    /* pointer to our integer value */
    long int *integer = 0;
    /* end pointer from strtol */
    char *endptr;

    if( ! tokens ){
        puts("ic_parse_expr_constant_integer: tokens was null");
        return 0;
    }
    if( ! i ){
        puts("ic_parse_expr_constant_integer: i was null");
        return 0;
    }

    /* build our return expr */
    expr = ic_expr_new(ic_expr_type_constant);
    if( ! expr ){
        puts("ic_parse_expr_constant_integer: call to ic_expr_new failed");
        return 0;
    }

    /* unpack our constant */
    cons = ic_expr_get_constant(expr);
    if( ! cons ){
        puts("ic_parse_expr_constant_integer: call to ic_expr_get_constant failed");
        return 0;
    }

    /* initialise our constant */
    if( ic_expr_constant_init(cons, ic_expr_constant_type_integer) ){
        puts("ic_parse_expr_constant_integer: call to ic_expr_constant_init failed");
        return 0;
    }

    /* get out our ic_integer */
    integer = ic_expr_constant_get_integer(cons);
    if( ! integer ){
        puts("ic_parse_expr_constant_integer: call to ic_expr_constant_get_integer failed");
        return 0;
    }

    /* get distance */
    dist = ic_parse_token_length(tokens->tokens, *i);
    if( ! dist ){
        puts("ic_parse_expr_constant_integer: call to ic_parse_token_length failed");
        return 0;
    }

    /* iterate through and check all characters are numeric */
    for( iter=0; iter<dist; ++iter ){
        switch( tokens->tokens[*i + iter] ){
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                /* valid integer, continue */
                break;
            default:
                /* invalid character found, bail */
                printf("ic_parse_expr_constant_integer: invalid integer found '%c'\n",
                       tokens->tokens[*i + iter]);
                return 0;
                break;
        }
    }

    /* set error value to clear */
    errno = 0;

    /* strtol */
    *integer = strtol(&(tokens->tokens[*i]), &endptr, 10);

    /* check the strtol was a success */
    if( errno == ERANGE ){
        puts("ic_parse_expr_constant_integer: call to strtol failed");
        if( *integer == LONG_MIN ){
            puts("-> underflow occured");
        } else if( *integer == LONG_MAX){
            puts("-> overflow occured");
        } else {
            puts("-> unknown ERANGE occured");
        }
        perror("strtol");
        return 0;
    }

    if( errno != 0 && *integer == 0 ){
        puts("ic_parse_expr_constant_integer: call to strtol failed, unknown error");
        perror("strtol");
        return 0;
    }

    /* check endptr */
    if( &(tokens->tokens[*i + dist]) != endptr ){
        printf("ic_parse_expr_constant_integer: endptr ('%c') was not as expected ('%c')\n",
               *endptr,
               tokens->tokens[*i + dist]);
        return 0;
    }

    /* skip over token */
    ic_parse_token_advance(i, dist);

    /* victory */
    return expr;
}


/* consume token
 * returns ic_expr* on success
 * returns 0 on failure
 */
static struct ic_expr * ic_parse_expr_operator(struct ic_tokens *tokens, unsigned int *i){
    puts("ic_parse_expr_operator: unimplemented");
    return 0;
}

struct ic_expr * ic_parse_expr(struct ic_tokens *tokens, unsigned int *i){
    /* pointer used to peek at start of next token */
    char *next = 0;

#ifdef DEBUG_PARSE_EXPR
    /* used for debugging */
    unsigned int dist = 0;
#endif

    if( ! tokens ){
        puts("ic_parse_expr: tokens was null");
        return 0;
    }
    if( ! i ){
        puts("ic_parse_expr: i was null");
        return 0;
    }

#ifdef DEBUG_PARSE_EXPR
    dist = ic_parse_token_length(tokens->tokens, *i);
    printf("\nic_parse_expr: considering token '%.*s'\n", dist, &(tokens->tokens[*i]));
    printf("context : '%s'\n", &(tokens->tokens[*i]));
#endif

    /* rules
     *
     * we see a " token -> string value
     * we see a number as the first char -> int value
     * we see a symbol, need to inspect next token
     *      ( -> func call
     *      {+ - * ...} -> operator
     *      else -> identifier
     */
    if( ic_parse_stringish(tokens, i) ){
        puts("ic_parse_expr: calling string");
        return ic_parse_expr_constant_string(tokens, i);
    }

    if( ic_parse_numberish(tokens, i) ){
        puts("ic_parse_expr: calling number");
        return ic_parse_expr_constant_integer(tokens, i);
    }

    next = ic_parse_peek_next(tokens, i);
    if( ! next ){
        /* this in theory could mean out final token is an identifier
         * however this is illegal in practice as all expr/stmt must
         * appears inside a body ending with `end`
         * so this is indeed an error
         */
        puts("ic_parse_expr: call to ic_parse_peek_next failed");
        return 0;
    }

#ifdef DEBUG_PARSE_EXPR
    printf("ic_parse_expr: peeking at next token '%c'\n", *next);
    printf("context : '%s'\n", next);
#endif

    /* basic support for operators
     * this will only support:
     *  identifier operator ...
     * as any numbers or strings are caught above
     *
     * FIXME reconsider this
     */
    if( ic_parse_operatorish(next) ){
#ifdef DEBUG_PARSE_EXPR
        puts("ic_parse_expr: calling operator");
#endif
        return ic_parse_expr_operator(tokens, i);
    }

    /* if we see an open bracket this is a function call */
    if( *next == '(' ){
#ifdef DEBUG_PARSE_EXPR
        puts("ic_parse_expr: calling fcall");
#endif
        return ic_parse_expr_fcall(tokens, i);
    }

    /* otherwise assume this is just an identifier */
#ifdef DEBUG_PARSE_EXPR
    puts("ic_parse_expr: calling identifier");
#endif
    return ic_parse_expr_identifier(tokens, i);
}

