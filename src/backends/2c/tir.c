#include "tir.h"
#include "expr.h"

#include "../../analyse/data/kludge.h"
#include "../../parse/data/stmt.h"
#include "../../transform/data/tir.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

unsigned int ic_b2c_compile_stmt_ret(struct ic_kludge *input_kludge, struct ic_transform_ir_ret *ret, FILE *out);
unsigned int ic_b2c_compile_stmt_let(struct ic_kludge *input_kludge, struct ic_transform_ir_let *let, FILE *out);
unsigned int ic_b2c_compile_stmt_assign(struct ic_kludge *input_kludge, struct ic_transform_ir_assign *assign, FILE *out);
unsigned int ic_b2c_compile_stmt_expr(struct ic_kludge *input_kludge, struct ic_transform_ir_expr *expr, FILE *out);

/* compile a given stmt to specified file
 *
 * generate a c program
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_b2c_compile_stmt(struct ic_kludge *input_kludge, struct ic_transform_ir_stmt *tstmt, FILE *out) {
    unsigned int indent_level = 1;

    if (!input_kludge) {
        puts("ic_b2c_compile_stmt: input_kludge was null");
        return 0;
    }

    if (!tstmt) {
        puts("ic_b2c_compile_stmt: tstmt was null");
        return 0;
    }

    if (!out) {
        puts("ic_b2c_compile_stmt: out was null");
        return 0;
    }

    /* dispatch to appropriate sub handler based on tstmt type */
    switch (tstmt->tag) {
        case ic_transform_ir_stmt_type_expr:
            if (!ic_b2c_compile_stmt_expr(input_kludge, &(tstmt->u.expr), out)) {
                puts("ic_b2c_compile_stmt: call to ic_b2c_compile_stmt_expr failed");
                return 0;
            }
            return 1;
            break;

        case ic_transform_ir_stmt_type_let:
            return ic_b2c_compile_stmt_let(input_kludge, &(tstmt->u.let), out);
            if (!ic_b2c_compile_stmt_expr(input_kludge, &(tstmt->u.expr), out)) {
                puts("ic_b2c_compile_stmt: call to ic_b2c_compile_stmt_let failed");
                return 0;
            }
            return 1;
            break;

        case ic_transform_ir_stmt_type_ret:
            return ic_b2c_compile_stmt_ret(input_kludge, &(tstmt->u.ret), out);
            if (!ic_b2c_compile_stmt_expr(input_kludge, &(tstmt->u.expr), out)) {
                puts("ic_b2c_compile_stmt: call to ic_b2c_compile_stmt_ret failed");
                return 0;
            }
            return 1;
            break;

        case ic_transform_ir_stmt_type_assign:
            return ic_b2c_compile_stmt_assign(input_kludge, &(tstmt->u.assign), out);
            if (!ic_b2c_compile_stmt_expr(input_kludge, &(tstmt->u.expr), out)) {
                puts("ic_b2c_compile_stmt: call to ic_b2c_compile_stmt_assign failed");
                return 0;
            }
            return 1;
            break;

        default:
            puts("ic_b2c_compile_stmt: stmt had impossible tag:");
            ic_transform_ir_stmt_print(tstmt, &indent_level);

            return 0;
            break;
    }

    puts("ic_b2c_compile_stmt: impossible...");
    return 0;
}

unsigned int ic_b2c_compile_stmt_ret(struct ic_kludge *input_kludge, struct ic_transform_ir_ret *ret, FILE *out) {
    unsigned int indent_level = 1;

    if (!input_kludge) {
        puts("ic_b2c_compile_stmt_ret: input_kludge was null");
        return 0;
    }

    if (!ret) {
        puts("ic_b2c_compile_stmt_ret: ret was null");
        return 0;
    }

    if (!out) {
        puts("ic_b2c_compile_stmt_ret: out was null");
        return 0;
    }

    puts("ic_b2c_compile_stmt_ret: called on");
    ic_transform_ir_ret_print(ret, &indent_level);

    puts("ic_b2c_compile_stmt_ret: unimplemented");
    return 0;
}

unsigned int ic_b2c_compile_stmt_let(struct ic_kludge *input_kludge, struct ic_transform_ir_let *let, FILE *out) {
    struct ic_type *let_type = 0;
    char *let_name = 0;
    struct ic_symbol *let_sym = 0;
    char *let_str = 0;

    struct ic_expr_constant *literal = 0;

    struct ic_transform_ir_let_expr *let_expr = 0;
    struct ic_transform_ir_expr *expr = 0;

    if (!input_kludge) {
        puts("ic_b2c_compile_stmt_let: input_kludge was null");
        return 0;
    }

    if (!let) {
        puts("ic_b2c_compile_stmt_let: let was null");
        return 0;
    }

    if (!out) {
        puts("ic_b2c_compile_stmt_let: out was null");
        return 0;
    }

    switch (let->tag) {
        case ic_transform_ir_let_type_literal:
            /* let name::type = literal */
            /* goes to */
            /* c_type_str name = cons(literal) */

            let_type = let->u.lit.type;

            let_sym = ic_type_name(let_type);
            if (!let_sym) {
                puts("ic_b2c_compile_stmt_let: call to ic_type_name failed");
                return 0;
            }

            let_str = ic_symbol_contents(let_sym);
            if (!let_str) {
                puts("ic_b2c_compile_stmt_let: call to ic_symbol_contents failed for let type");
                return 0;
            }

            let_name = ic_symbol_contents(let->u.lit.name);
            if (!let_name) {
                puts("ic_b2c_compile_stmt_let: call to ic_symbol_contents failed for let name");
                return 0;
            }

            /* print "type *name = " */
            fprintf(out, "%s *%s = ", let_str, let_name);

            /* literal */
            literal = let->u.lit.literal;

            if (!ic_b2c_compile_expr_constant(input_kludge, literal, out)) {
                puts("ic_b2c_compile_stmt_let: call to ic_b2c_compile_expr_constant failed");
                return 0;
            }

            /* closing semicolon and trailing \n */
            fputs(";\n", out);

            return 1;
            break;

        case ic_transform_ir_let_type_expr:
            /* let name::type = fcall(args...) */
            /* goes to */
            /* c_type_str name = fcall(args...) */

            let_expr = &(let->u.expr);

            let_type = let_expr->type;

            let_sym = ic_type_name(let_type);
            if (!let_sym) {
                puts("ic_b2c_compile_stmt_let: call to ic_type_name failed");
                return 0;
            }

            let_str = ic_symbol_contents(let_sym);
            if (!let_str) {
                puts("ic_b2c_compile_stmt_let: call to ic_symbol_contents failed for let type");
                return 0;
            }

            let_name = ic_symbol_contents(let_expr->name);
            if (!let_name) {
                puts("ic_b2c_compile_stmt_let: call to ic_symbol_contents failed for let name");
                return 0;
            }

            /* print "type *name = " */
            fprintf(out, "%s *%s = ", let_str, let_name);

            /* expression */
            expr = let_expr->expr;

            if (!ic_b2c_compile_expr(input_kludge, expr, out)) {
                puts("ic_b2c_compile_stmt_let: call to ic_b2c_compile_expr failed");
                return 0;
            }

            /* closing semicolon and trailing \n */
            fputs(";\n", out);

            return 1;
            break;

        default:
            puts("ic_b2c_compile_stmt_let: impossible let_type");
            return 0;
            break;
    }

    return 0;
}

unsigned int ic_b2c_compile_stmt_assign(struct ic_kludge *input_kludge, struct ic_transform_ir_assign *assign, FILE *out) {
    unsigned int indent_level = 1;

    if (!input_kludge) {
        puts("ic_b2c_compile_stmt_assign: input_kludge was null");
        return 0;
    }

    if (!assign) {
        puts("ic_b2c_compile_stmt_assign: assign was null");
        return 0;
    }

    if (!out) {
        puts("ic_b2c_compile_stmt_assign: out was null");
        return 0;
    }

    puts("ic_b2c_compile_stmt_assign: called on");
    ic_transform_ir_assign_print(assign, &indent_level);

    puts("ic_b2c_compile_stmt_assign: unimplemented");
    return 0;
}

unsigned int ic_b2c_compile_stmt_expr(struct ic_kludge *input_kludge, struct ic_transform_ir_expr *expr, FILE *out) {
    if (!input_kludge) {
        puts("ic_b2c_compile_stmt_expr: input_kludge was null");
        return 0;
    }

    if (!expr) {
        puts("ic_b2c_compile_stmt_expr: expr was null");
        return 0;
    }

    if (!out) {
        puts("ic_b2c_compile_stmt_expr: out was null");
        return 0;
    }

    if (!ic_b2c_compile_expr(input_kludge, expr, out)) {
        puts("ic_b2c_compile_stmt_expr: call to ic_b2c_compile_expr failed");
        return 0;
    }

    /* must close off expr */
    fputs(";\n", out);

    return 1;
}
