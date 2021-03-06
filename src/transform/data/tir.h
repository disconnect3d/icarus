#ifndef ICARUS_TRANSFORM_IR_H
#define ICARUS_TRANSFORM_IR_H

#include <stdbool.h>

#include "../../data/pvector.h"
#include "../../data/symbol.h"
#include "../../parse/data/decl.h"

/* this IR is in administrative normal form
 * all arguments to functions are already bound via lets
 * all let/assignment rhs are one of
 *  literals
 *  fcall
 *  faccess
 *  var
 */

/* IR:
 *  let name type expr
 *  fcall name (names...)
 *  return name
 *  assign name expr
 *
 * expr:
 *  fcall name (names...)
 *  literal value
 *  var name
 *  faccess left . right
 */

enum ic_transform_ir_expr_tag {
    ic_transform_ir_expr_type_literal,
    ic_transform_ir_expr_type_var,
    ic_transform_ir_expr_type_faccess,
    ic_transform_ir_expr_type_fcall
};

/* an expr is only ever a function call
 * in void context we only call a function for it's side effects
 * all other things that yield a value are dealt with as let expressions
 */
struct ic_transform_ir_expr {
    enum ic_transform_ir_expr_tag tag;

    union {
        struct ic_transform_ir_expr_literal *literal;
        struct ic_transform_ir_expr_var *var;
        struct ic_transform_ir_expr_faccess *faccess;
        struct ic_transform_ir_expr_fcall *fcall;
    } u;
};

/* allocate and initialise a new expr
 *
 * TODO doesn't touch any of the contained elements
 *
 * returns pointer on success
 * returns 0 on failure
 */
struct ic_transform_ir_expr *ic_transform_ir_expr_new(enum ic_transform_ir_expr_tag tag);

/* initialise an existing expr
 *
 * TODO doesn't touch any of the contained elements
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_transform_ir_expr_init(struct ic_transform_ir_expr *expr, enum ic_transform_ir_expr_tag tag);

/* destroy expr
 *
 * TODO doesn't touch any of the contained elements
 *
 * will only free expr if `free_expr` is truthy
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_transform_ir_expr_destroy(struct ic_transform_ir_expr *expr, unsigned int free_expr);

/* print expr
 *
 * returns 1 on success
 * return 0 on failure
 */
unsigned int ic_transform_ir_expr_print(FILE *fd, struct ic_transform_ir_expr *expr, unsigned int *indent);

struct ic_transform_ir_expr_literal {
    struct ic_expr_constant *literal;
};

/* allocate and initialise a new expr_literal
 *
 * TODO doesn't touch any of the contained elements
 *
 * returns pointer on success
 * returns 0 on failure
 */
struct ic_transform_ir_expr_literal *ic_transform_ir_expr_literal_new(void);

/* initialise an existing expr_literal
 *
 * TODO doesn't touch any of the contained elements
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_transform_ir_expr_literal_init(struct ic_transform_ir_expr_literal *literal);

/* destroy expr_literal
 *
 * TODO doesn't touch any of the contained elements
 *
 * will only free let if `free_let` is truthy
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_transform_ir_expr_literal_destroy(struct ic_transform_ir_expr_literal *literal, unsigned int free_let);

/* print expr_literal
 *
 * returns 1 on success
 * return 0 on failure
 */
unsigned int ic_transform_ir_expr_literal_print(FILE *fd, struct ic_transform_ir_expr_literal *literal, unsigned int *indent);

struct ic_transform_ir_expr_var {
    struct ic_symbol *sym;
};

/* allocate and initialise a new expr_var
 *
 * TODO doesn't touch any of the contained elements
 *
 * returns pointer on success
 * returns 0 on failure
 */
struct ic_transform_ir_expr_var *ic_transform_ir_expr_var_new(void);

/* initialise an existing expr_var
 *
 * TODO doesn't touch any of the contained elements
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_transform_ir_expr_var_init(struct ic_transform_ir_expr_var *var);

/* destroy expr_var
 *
 * TODO doesn't touch any of the contained elements
 *
 * will only free let if `free_let` is truthy
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_transform_ir_expr_var_destroy(struct ic_transform_ir_expr_var *var, unsigned int free_var);

/* print expr_var
 *
 * returns 1 on success
 * return 0 on failure
 */
unsigned int ic_transform_ir_expr_var_print(FILE *fd, struct ic_transform_ir_expr_var *var, unsigned int *indent);

struct ic_transform_ir_expr_faccess {
    struct ic_symbol *left;
    struct ic_decl_type *left_type;
    struct ic_symbol *right;
};

/* allocate and initialise a new expr_faccess
 *
 * TODO doesn't touch any of the contained elements
 *
 * returns pointer on success
 * returns 0 on failure
 */
struct ic_transform_ir_expr_faccess *ic_transform_ir_expr_faccess_new(void);

/* initialise an existing expr_faccess
 *
 * TODO doesn't touch any of the contained elements
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_transform_ir_expr_faccess_init(struct ic_transform_ir_expr_faccess *faccess);

/* destroy expr_faccess
 *
 * TODO doesn't touch any of the contained elements
 *
 * will only free let if `free_faccess` is truthy
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_transform_ir_expr_faccess_destroy(struct ic_transform_ir_expr_faccess *faccess, unsigned int free_faccess);

/* print expr_faccess
 *
 * returns 1 on success
 * return 0 on failure
 */
unsigned int ic_transform_ir_expr_faccess_print(FILE *fd, struct ic_transform_ir_expr_faccess *faccess, unsigned int *indent);

struct ic_transform_ir_let {
    struct ic_symbol *name;
    struct ic_decl_type *type;
    struct ic_transform_ir_expr *expr;
    /* if this tlet is ever assigned to
     *
     * set during transform phase from ic_stmt_let->assigned_to
     *
     * this is used by some backends to perform optimisations around variable allocation
     */
    bool assigned_to;
};

/* allocate and initialise a new let
 *
 * TODO doesn't touch any of the contained elements
 *
 * returns pointer on success
 * returns 0 on failure
 */
struct ic_transform_ir_let *ic_transform_ir_let_new(void);

/* initialise an existing let
 *
 * TODO doesn't touch any of the contained elements
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_transform_ir_let_init(struct ic_transform_ir_let *let);

/* destroy let
 *
 * TODO doesn't touch any of the contained elements
 *
 * will only free let if `free_let` is truthy
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_transform_ir_let_destroy(struct ic_transform_ir_let *let, unsigned int free_let);

/* print let
 *
 * returns 1 on success
 * return 0 on failure
 */
unsigned int ic_transform_ir_let_print(FILE *fd, struct ic_transform_ir_let *let, unsigned int *indent);

/* get pointer to internal expr
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_transform_ir_expr *ic_transform_ir_let_get_expr(struct ic_transform_ir_let *let);

/* the left hand-size here is fine
 * as we only need to support
 *   foo = expr
 *
 * we want to transform things of the form
 *  a[b] = c
 *  a.b = c
 * into
 *  set(a, b, c)
 */
struct ic_transform_ir_assign {
    struct ic_symbol *left;
    struct ic_transform_ir_expr *right;
};

/* allocate and initialise a new assign
 *
 * TODO doesn't touch any of the contained elements
 *
 * returns pointer on success
 * returns 0 on failure
 */
struct ic_transform_ir_assign *ic_transform_ir_assign_new(void);

/* initialise an existing assign
 *
 * TODO doesn't touch any of the contained elements
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_transform_ir_assign_init(struct ic_transform_ir_assign *assign);

/* destroy assign
 *
 * TODO doesn't touch any of the contained elements
 *
 * will only free assign if `free_assign` is truthy
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_transform_ir_assign_destroy(struct ic_transform_ir_assign *assign, unsigned int free_assign);

/* print assign
 *
 * returns 1 on success
 * return 0 on failure
 */
unsigned int ic_transform_ir_assign_print(FILE *fd, struct ic_transform_ir_assign *assign, unsigned int *indent);

struct ic_transform_ir_begin {
    struct ic_transform_body *tbody;
};

/* allocate and initialise a new begin
 *
 * TODO doesn't touch any of the contained elements
 *
 * returns pointer on success
 * returns 0 on failure
 */
struct ic_transform_ir_begin *ic_transform_ir_begin_new(void);

/* initialise an existing begin
 *
 * TODO doesn't touch any of the contained elements
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_transform_ir_begin_init(struct ic_transform_ir_begin *begin);

/* destroy begin
 *
 * TODO doesn't touch any of the contained elements
 *
 * will only free assign begin `free_begin` is truthy
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_transform_ir_begin_destroy(struct ic_transform_ir_begin *begin, unsigned int free_begin);

/* print begin
 *
 * returns 1 on success
 * return 0 on failure
 */
unsigned int ic_transform_ir_begin_print(FILE *fd, struct ic_transform_ir_begin *begin, unsigned int *indent);

struct ic_transform_ir_if {
    /* symbol pointing to let variables holding if-condition value */
    struct ic_symbol *cond;
    /* mandatory then tbody */
    struct ic_transform_body *then_tbody;
    /* optional else tbody */
    struct ic_transform_body *else_tbody;
};

/* allocate and initialise a new if
 *
 * TODO doesn't touch any of the contained elements
 *
 * returns pointer on success
 * returns 0 on failure
 */
struct ic_transform_ir_if *ic_transform_ir_if_new(struct ic_symbol *cond_sym);

/* initialise an existing if
 *
 * TODO doesn't touch any of the contained elements
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_transform_ir_if_init(struct ic_transform_ir_if *tif, struct ic_symbol *cond_sym);

/* destroy if
 *
 * TODO doesn't touch any of the contained elements
 *
 * will only free assign if `free_if` is truthy
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_transform_ir_if_destroy(struct ic_transform_ir_if *tif, unsigned int free_if);

/* print if
 *
 * returns 1 on success
 * return 0 on failure
 */
unsigned int ic_transform_ir_if_print(FILE *fd, struct ic_transform_ir_if *tif, unsigned int *indent);

struct ic_transform_ir_ret {
    struct ic_symbol *var;
};

/* allocate and initialise a new ret
 *
 * TODO doesn't touch any of the contained elements
 *
 * returns pointer on success
 * returns 0 on failure
 */
struct ic_transform_ir_ret *ic_transform_ir_ret_new(void);

/* initialise an existing ret
 *
 * TODO doesn't touch any of the contained elements
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_transform_ir_ret_init(struct ic_transform_ir_ret *ret);

/* destroy ret
 *
 * TODO doesn't touch any of the contained elements
 *
 * will only free ret if `free_ret` is truthy
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_transform_ir_ret_destroy(struct ic_transform_ir_ret *ret, unsigned int free_ret);

/* print ret
 *
 * returns 1 on success
 * return 0 on failure
 */
unsigned int ic_transform_ir_ret_print(FILE *fd, struct ic_transform_ir_ret *ret, unsigned int *indent);

struct ic_transform_ir_expr_fcall {
    struct ic_expr_func_call *fcall;
    /* pvector of symbols */
    struct ic_pvector *args;
};

/* allocate and initialise a new fcall
 *
 * TODO doesn't touch any of the contained elements
 *
 * returns pointer on success
 * returns 0 on failure
 */
struct ic_transform_ir_expr_fcall *ic_transform_ir_expr_fcall_new(struct ic_expr_func_call *fcall, struct ic_pvector *args);

/* initialise an existing fcall
 *
 * TODO doesn't touch any of the contained elements
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_transform_ir_expr_fcall_init(struct ic_transform_ir_expr_fcall *tir_expr_fcall, struct ic_expr_func_call *fcall, struct ic_pvector *args);

/* destroy fcall
 *
 * TODO doesn't touch any of the contained elements
 *
 * will only free fcall if `free_fcall` is truthy
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_transform_ir_expr_fcall_destroy(struct ic_transform_ir_expr_fcall *fcall, unsigned int free_fcall);

/* print fcall
 *
 * returns 1 on success
 * return 0 on failure
 */
unsigned int ic_transform_ir_expr_fcall_print(FILE *fd, struct ic_transform_ir_expr_fcall *fcall, unsigned int *indent);

/* get number of args
 *
 * returns number on success
 * returns 0 on failure
 */
unsigned int ic_transform_ir_expr_fcall_length(struct ic_transform_ir_expr_fcall *fcall);

/* get arg at offset i
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_symbol *ic_transform_ir_expr_fcall_get_arg(struct ic_transform_ir_expr_fcall *fcall, unsigned int i);

struct ic_transform_ir_match_case {
    struct ic_field *field;
    struct ic_transform_body *tbody;
};

/* allocate and initialise a new case
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_transform_ir_match_case *ic_transform_ir_match_case_new(struct ic_field *field);

/* initialise an existing case
 *
 * returns 1 on success
 * return 0 on failure
 */
unsigned int ic_transform_ir_match_case_init(struct ic_transform_ir_match_case *scase, struct ic_field *field);

/* destroy case
 *
 * TODO doesn't touch any fields
 *
 * will only free case if `free_case` is truthy
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_transform_ir_match_case_destroy(struct ic_transform_ir_match_case *scase, unsigned int free_case);

/* print case
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_transform_ir_match_case_print(FILE *fd, struct ic_transform_ir_match_case *scase, unsigned int *indent);

struct ic_transform_ir_match {
    struct ic_decl_type *tdecl;
    struct ic_symbol *match_symbol;
    struct ic_pvector cases;
    struct ic_transform_body *else_body;
};

/* allocate and initialise a new match
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_transform_ir_match *ic_transform_ir_match_new(struct ic_decl_type *tdecl, struct ic_symbol *match_symbol);

/* initialise an existing match
 *
 * returns 1 on success
 * return 0 on failure
 */
unsigned int ic_transform_ir_match_init(struct ic_transform_ir_match *match, struct ic_decl_type *tdecl, struct ic_symbol *match_symbol);

/* destroy match
 *
 * TODO doesn't touch any fields
 *
 * will only free match if `free_match` is truthy
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_transform_ir_match_destroy(struct ic_transform_ir_match *match, unsigned int free_match);

/* print match
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_transform_ir_match_print(FILE *fd, struct ic_transform_ir_match *match, unsigned int *indent);

enum ic_transform_ir_stmt_tag {
    ic_transform_ir_stmt_type_fcall,
    ic_transform_ir_stmt_type_let,
    ic_transform_ir_stmt_type_ret,
    ic_transform_ir_stmt_type_assign,
    ic_transform_ir_stmt_type_begin,
    ic_transform_ir_stmt_type_if,
    ic_transform_ir_stmt_type_match,
};

struct ic_transform_ir_stmt {
    enum ic_transform_ir_stmt_tag tag;
    union {
        struct ic_transform_ir_expr_fcall *fcall;
        struct ic_transform_ir_let let;
        struct ic_transform_ir_ret ret;
        struct ic_transform_ir_assign assign;
        struct ic_transform_ir_begin begin;
        struct ic_transform_ir_if sif;
        struct ic_transform_ir_match match;
    } u;
};

/* allocate and initialise a new stmt
 *
 * TODO doesn't touch any of the contained elements
 *
 * returns pointer on success
 * returns 0 on failure
 */
struct ic_transform_ir_stmt *ic_transform_ir_stmt_new(enum ic_transform_ir_stmt_tag tag);

/* initialise an existing stmt
 *
 * TODO doesn't touch any of the contained elements
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_transform_ir_stmt_init(struct ic_transform_ir_stmt *stmt, enum ic_transform_ir_stmt_tag tag);

/* destroy stmt
 *
 * TODO doesn't touch any of the contained elements
 *
 * will only free stmt if `free_stmt` is truthy
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_transform_ir_stmt_destroy(struct ic_transform_ir_stmt *stmt, unsigned int free_stmt);

/* print stmt
 *
 * returns 1 on success
 * return 0 on failure
 */
unsigned int ic_transform_ir_stmt_print(FILE *fd, struct ic_transform_ir_stmt *stmt, unsigned int *indent);

/* get pointer to internal expr_fcall
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_transform_ir_expr_fcall *ic_transform_ir_stmt_get_fcall(struct ic_transform_ir_stmt *stmt);

/* get pointer to internal let
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_transform_ir_let *ic_transform_ir_stmt_get_let(struct ic_transform_ir_stmt *stmt);

/* get pointer to internal ret
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_transform_ir_ret *ic_transform_ir_stmt_get_ret(struct ic_transform_ir_stmt *stmt);

/* get pointer to internal begin
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_transform_ir_begin *ic_transform_ir_stmt_get_begin(struct ic_transform_ir_stmt *stmt);

/* get pointer to internal if
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_transform_ir_if *ic_transform_ir_stmt_get_if(struct ic_transform_ir_stmt *stmt);

/* get pointer to internal match
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_transform_ir_match *ic_transform_ir_stmt_get_match(struct ic_transform_ir_stmt *stmt);

/* get pointer to internal assign
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_transform_ir_assign *ic_transform_ir_stmt_get_assign(struct ic_transform_ir_stmt *stmt);

/* allocate and initialise a new stmt->let->literal
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_transform_ir_stmt *ic_transform_ir_stmt_let_literal_new(struct ic_symbol *name, struct ic_decl_type *type, struct ic_expr_constant *literal);

/* allocate and initialise a new stmt->let->expr
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_transform_ir_stmt *ic_transform_ir_stmt_let_expr_new(struct ic_symbol *name, struct ic_decl_type *type, struct ic_transform_ir_expr *expr);

/* allocate and initialise a new stmt->let->faccess
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_transform_ir_stmt *ic_transform_ir_stmt_let_faccess_new(struct ic_symbol *name, struct ic_decl_type *type, struct ic_symbol *left, struct ic_decl_type *left_type, struct ic_symbol *right);

/* allocate and initialise a new stmt->ret
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_transform_ir_stmt *ic_transform_ir_stmt_ret_new(struct ic_symbol *var);

/* allocate and initialise a new stmt->begin
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_transform_ir_stmt *ic_transform_ir_stmt_begin_new(void);

/* allocate and initialise a new stmt->if
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_transform_ir_stmt *ic_transform_ir_stmt_if_new(struct ic_symbol *cond_sym);

/* allocate and initialise a new stmt->match
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_transform_ir_stmt *ic_transform_ir_stmt_match_new(struct ic_decl_type *tdecl, struct ic_symbol *match_symbol);

#endif /* ifndef ICARUS_TRANSFORM_IR_H */
