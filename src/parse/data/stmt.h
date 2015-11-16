#ifndef ICARUS_STATEMENT_H
#define ICARUS_STATEMENT_H

#include "body.h"
#include "expr.h"
#include "../../data/symbol.h"

/* a return statement
 *  return expr
 */
struct ic_stmt_ret {
    struct ic_expr *ret;
};

/* allocate and initialise a new return
 * does not touch init ic_expr
 *
 * returns pointers on success
 * returns 0 on failure
 */
struct ic_stmt_ret * ic_stmt_ret_new(void);

/* initialise an existing return
 * does not touch the init expression
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_stmt_ret_init(struct ic_stmt_ret *ret);

/* destroy ret
 *
 * will only free ret if `free_ret` is truthy
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_stmt_ret_destroy(struct ic_stmt_ret *ret, unsigned int free_ret);

/* get the ic_expr * contained within
 *
 * returns pointer on success
 * returns 0 on failure
 */
struct ic_expr * ic_stmt_ret_get_expr(struct ic_stmt_ret *ret);

/* print this return */
void ic_stmt_ret_print(struct ic_stmt_ret *ret, unsigned int *indent_level);


/* a let statement
 *  let identifier::type = init
 */
struct ic_stmt_let {
    unsigned int permissions;

    struct ic_symbol identifier;

    /* type is optionally declared at parse time
     * and may be set instead at analyse time
     * via inference
     *
     * if this is null then we did not have a type
     * declared at parse time
     */
    struct ic_symbol *declared_type;

    /* type we know of at analysis time
     * this could either be via the declared type
     * or inferred from the init expr.
     */
    struct ic_type *inferred_type;

    /* FIXME making this an ic_expr *
     * to simplify interface between
     * parse stmt and parse expr
     */
    struct ic_expr *init;
};

/* allocate and initialise a new let
 * does not touch init ic_expr
 *
 * returns pointers on success
 * returns 0 on failure
 */
struct ic_stmt_let * ic_stmt_let_new(char *id_src, unsigned int id_len, unsigned int permissions);

/* initialise an existing let
 * does not touch the init expression
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_stmt_let_init(struct ic_stmt_let *let, char *id_src, unsigned int id_len, unsigned int permissions);

/* destroy let
 *
 * will only free let if `free_let` is truthy
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_stmt_let_destroy(struct ic_stmt_let *let, unsigned int free_let);

/* set declared type on this let
 *
 * this sets the symbol `declared_type`
 *
 * this is used for types declared in source,
 * this is set at parse time
 *
 * this is an error if either types have already been set
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_stmt_let_set_declared_type(struct ic_stmt_let *let, char *type_src, unsigned int type_len);

/* set inferred type on this let
 *
 * this sets the type ref `inferred_type`
 *
 * this is used for the type we know of
 * at analsyis time, this could be from
 * the declared type in source or via
 * the type of the init. expr.
 *
 * this is set at analysis time
 *
 * this is an error if inferred type is already set
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_stmt_let_set_inferred_type(struct ic_stmt_let *let, struct ic_type *type);

/* get the ic_expr * contained within
 *
 * returns pointer on success
 * returns 0 on failure
 */
struct ic_expr * ic_stmt_let_get_expr(struct ic_stmt_let *let);

/* print this let */
void ic_stmt_let_print(struct ic_stmt_let *let, unsigned int *indent_level);


/* an assignment statement
 *  x = y
 *  FIXME need to have permissions on expressions
 */
struct ic_stmt_assign {
    struct ic_expr *left;
    struct ic_expr *right;
};

/* allocate and initialise a new assign
 * does not touch init ic_expr
 *
 * returns pointers on success
 * returns 0 on failure
 */
struct ic_stmt_assign * ic_stmt_assign_new(struct ic_expr *left, struct ic_expr *right);

/* initialise an existing assign
 * does not touch the init expression
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_stmt_assign_init(struct ic_stmt_assign *assign, struct ic_expr *left, struct ic_expr *right);

/* destroy assign
 *
 * will only free assign if `free_assign` is truthy
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_stmt_assign_destroy(struct ic_stmt_assign *assign, unsigned int free_assign);

/* get the left ic_expr * contained within
 *
 * returns pointer on success
 * returns 0 on failure
 */
struct ic_expr * ic_stmt_assign_get_left(struct ic_stmt_assign *assign);

/* get the right ic_expr * contained within
 *
 * returns pointer on success
 * returns 0 on failure
 */
struct ic_expr * ic_stmt_assign_get_right(struct ic_stmt_assign *assign);

/* print this assign */
void ic_stmt_assign_print(struct ic_stmt_assign *assign, unsigned int *indent_level);


/* an if statement
 *  if expr
 *      body
 *  end
 */
struct ic_stmt_if {
    struct ic_expr *expr;
    struct ic_body *body;
};

/* allocate and initialise a new ic_stmt_if
 * this will initialise the body
 * but will NOT initialise the expression
 *
 * returns pointers on success
 * returns 0 on failure
 */
struct ic_stmt_if * ic_stmt_if_new(void);

/* initialise an existing new ic_stmt_if
 * this will initialise the body
 * but will NOT initialise the expression
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_stmt_if_init(struct ic_stmt_if *sif);

/* destroy if
 *
 * only frees stmt_if if `free_if` is truthy
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_stmt_if_destroy(struct ic_stmt_if *sif, unsigned int free_if);

/* returns pointer on success
 * returns 0 on failure
 */
struct ic_expr * ic_stmt_if_get_expr(struct ic_stmt_if *sif);

/* get statement of offset i within the body
 *
 * returns pointer to element on success
 * returns 0 on failure
 */
struct ic_stmt * ic_stmt_if_get_stmt(struct ic_stmt_if *sif, unsigned int i);

/* get length of body
 *
 * returns length on success
 * returns 0 on failure
 */
unsigned int ic_stmt_if_length(struct ic_stmt_if *sif);

/* print this if */
void ic_stmt_if_print(struct ic_stmt_if *sif, unsigned int *indent_level);


/* a for statement
 *  for expr in iterator
 *      body
 *  end
 */
struct ic_stmt_for {
    struct ic_expr *expr;
    struct ic_expr *iterator;
    struct ic_body *body;
};

/* allocate and initialise a new ic_stmt_for
 * this will initialise the body
 * but will NOT initialise the expression OR the iterator
 *
 * returns pointers on success
 * returns 0 on failure
 */
struct ic_stmt_for * ic_stmt_for_new(void);

/* initialise an existing new ic_stmt_for
 * this will initialise the body
 * but will NOT initialise the expression
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_stmt_for_init(struct ic_stmt_for *sfor);

/* destroy for
 *
 * only frees stmt_for if `free_for` is truthy
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_stmt_for_destroy(struct ic_stmt_for *sfor, unsigned int free_for);

/* returns pointer on success
 * returns 0 on failure
 */
struct ic_expr * ic_stmt_for_get_expr(struct ic_stmt_for *sfor);

/* returns pointer on success
 * returns 0 on failure
 */
struct ic_expr * ic_stmt_for_get_iterator(struct ic_stmt_for *sfor);

/* get statement of offset i within the body
 *
 * returns pointer to element on success
 * returns 0 on failure
 */
struct ic_stmt * ic_stmt_for_get_stmt(struct ic_stmt_for *sfor, unsigned int i);

/* get length of body
 *
 * returns length on success
 * returns 0 on failure
 */
unsigned int ic_stmt_for_length(struct ic_stmt_for *sfor);

/* print this for */
void ic_stmt_for_print(struct ic_stmt_for *sfor, unsigned int *indent_level);


/* a while statement
 *  while expr
 *      body
 *  end
 */
struct ic_stmt_while {
    struct ic_expr *expr;
    struct ic_body *body;
};

/* allocate and initialise a new ic_stmt_while
 * this will initialise the body
 * but will NOT initialise the expression
 *
 * returns pointers on success
 * returns 0 on failure
 */
struct ic_stmt_while * ic_stmt_while_new(void);

/* initialise an existing new ic_stmt_while
 * this will initialise the body
 * but will NOT initialise the expression
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_stmt_while_init(struct ic_stmt_while *swhile);

/* destroy while
 *
 * only frees stmt_while if `free_while` is truthy
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_stmt_while_destroy(struct ic_stmt_while *swhile, unsigned int free_while);

/* returns pointer on success
 * returns 0 on failure
 */
struct ic_expr * ic_stmt_while_get_expr(struct ic_stmt_while *swhile);

/* get statement of offset i within the body
 *
 * returns pointer to element on success
 * returns 0 on failure
 */
struct ic_stmt * ic_stmt_while_get_stmt(struct ic_stmt_while *swhile, unsigned int i);

/* get length of body
 *
 * returns length on success
 * returns 0 on failure
 */
unsigned int ic_stmt_while_length(struct ic_stmt_while *swhile);

/* print this if */
void ic_stmt_while_print(struct ic_stmt_while *swhile, unsigned int *indent_level);


enum ic_stmt_tag {
    ic_stmt_type_ret,
    ic_stmt_type_let,
    ic_stmt_type_assign,
    ic_stmt_type_if,
    ic_stmt_type_for,
    ic_stmt_type_while,
    ic_stmt_type_expr
};

struct ic_stmt {
    enum ic_stmt_tag tag;
    union {
        struct ic_stmt_ret    ret;
        struct ic_stmt_let    let;
        struct ic_stmt_assign assign;
        struct ic_stmt_if     sif;
        struct ic_stmt_for    sfor;
        struct ic_stmt_while  swhile;
        /* a statement can just be an expression in
         * void context
         *  foo(bar)
         *
         * FIXME making this an ic_expr *
         * to simplify interface between
         * parse stmt and parse expr
         */
        struct ic_expr *expr;
    } u;
};

/* allocate and initialise anew ic_stmt
 * will not initialise union members
 *
 * returns pointer on success
 * returns 0 on failure
 */
struct ic_stmt * ic_stmt_new(enum ic_stmt_tag tag);

/* initialise an existing ic_stmt
 *
 * returns 1 on success
 * returns 0 on failure
 */
int ic_stmt_init(struct ic_stmt *stmt, enum ic_stmt_tag tag);

/* destroy stmt
 *
 * will only free stmt if `free_stmt` is truthy
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_stmt_destroy(struct ic_stmt *stmt, unsigned int free_stmt);

/* get a pointer to the return within
 * will only succeed if ic_stmt is of the correct type
 *
 * returns pointer on success
 * returns 0 on failure
 */
struct ic_stmt_ret * ic_stmt_get_ret(struct ic_stmt *stmt);

/* get a pointer to the let within
 * will only succeed if ic_stmt is of the correct type
 *
 * returns pointer on success
 * returns 0 on failure
 */
struct ic_stmt_let * ic_stmt_get_let(struct ic_stmt *stmt);

/* get a pointer to the assign within
 * will only succeed if ic_stmt is of the correct type
 *
 * returns pointer on success
 * returns 0 on failure
 */
struct ic_stmt_assign * ic_stmt_get_assign(struct ic_stmt *stmt);

/* get a pointer to the sif within
 * will only succeed if ic_stmt is of the correct type
 *
 * returns pointer on success
 * returns 0 on failure
 */
struct ic_stmt_if * ic_stmt_get_sif(struct ic_stmt *stmt);

/* get a pointer to the sfor within
 * will only succeed if ic_stmt is of the correct type
 *
 * returns pointer on success
 * returns 0 on failure
 */
struct ic_stmt_for * ic_stmt_get_sfor(struct ic_stmt *stmt);

/* get a pointer to the swhile within
 * will only succeed if ic_stmt is of the correct type
 *
 * returns pointer on success
 * returns 0 on failure
 */
struct ic_stmt_while * ic_stmt_get_swhile(struct ic_stmt *stmt);

/* get a pointer to the expr within
 * will only succeed if ic_stmt is of the correct type
 *
 * returns pointer on success
 * returns 0 on failure
 */
struct ic_expr * ic_stmt_get_expr(struct ic_stmt *stmt);

/* print this stmt */
void ic_stmt_print(struct ic_stmt *stmt, unsigned int *indent_level);

#endif
