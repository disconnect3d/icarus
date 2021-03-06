#ifndef IC_BACKEND_PANCAKE_LOCAL_H
#define IC_BACKEND_PANCAKE_LOCAL_H

#include <stdbool.h>

enum ic_backend_pancake_local_type {
    /* literal means we have a literal value stored here */
    icpl_literal,
    /* offset means we have the value stored on the stack
     * at the given offset within our fcall's value_stack
     */
    icpl_offset,
    /* runtime means the value is dependent on a runtime expression
     * and that the value is stored on our call_frame->local_vars
     */
    icpl_runtime,
};

/* a local 'variable' used during bytecode compilation */
struct ic_backend_pancake_local {
    /* whether or not this local variable was accessed
     * used to throw warnings for unused variables
     */
    bool accessed;

    /* local variable name */
    struct ic_symbol *name;

    enum ic_backend_pancake_local_type tag;

    /* from here we can have one of two cases
     *  literal - value
     *  runtime - offset into this function
     */
    union {
        struct ic_expr_constant *literal;
        unsigned int offset;
    } u;
};

/* allocate and init a new local
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_backend_pancake_local *ic_backend_pancake_local_new(struct ic_symbol *name, enum ic_backend_pancake_local_type tag);

/* init an existing local
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_backend_pancake_local_init(struct ic_backend_pancake_local *local, struct ic_symbol *name, enum ic_backend_pancake_local_type tag);

/* destroy a local
 *
 * will only free local if `free_local` is truthy
 *
 * FIXME doesn't free any elements within
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_backend_pancake_local_destroy(struct ic_backend_pancake_local *local, unsigned int free_local);

/* set literal on this local
 *
 * must be a literal
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_backend_pancake_local_set_literal(struct ic_backend_pancake_local *local, struct ic_expr_constant *literal);

/* set offset on this local
 *
 * must be an offset
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_backend_pancake_local_set_offset(struct ic_backend_pancake_local *local, unsigned int offset);

/* get literal from this local
 *
 * must be a literal
 *
 * returns 1 on success
 * returns 0 on failure
 */
struct ic_expr_constant *ic_backend_pancake_local_get_literal(struct ic_backend_pancake_local *local);

/* get offset from this local
 *
 * must be an offset
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_backend_pancake_local_get_offset(struct ic_backend_pancake_local *local);

/* get the accessed field on this local
 *
 * has no way of signalling error
 *
 * returns boolean on success
 * returns 0 on failure
 */
bool ic_backend_pancake_local_accessed(struct ic_backend_pancake_local *local);

#endif /* IC_BACKEND_PANCAKE_LOCAL_H */
