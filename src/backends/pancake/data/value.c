#include <stdio.h>
#include <stdlib.h>

#include "value.h"

/* allocate a  blank value */
struct ic_backend_pancake_value *ic_backend_pancake_value_new(void) {
    struct ic_backend_pancake_value *value = 0;

    value = calloc(1, sizeof(struct ic_backend_pancake_value));
    if (!value) {
        puts("ic_backend_pancake_value_new: call to calloc failed");
        return 0;
    }

    return value;
}

/* copy all data from `value_from` into `value_to`
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_backend_pancake_value_copy(struct ic_backend_pancake_value *value_from, struct ic_backend_pancake_value *value_to) {
    if (!value_from) {
        puts("ic_backend_pancake_value_copy: value_from was null");
        return 0;
    }

    if (!value_to) {
        puts("ic_backend_pancake_value_copy: value_to was null");
        return 0;
    }

    value_to->tag = value_from->tag;

    switch (value_from->tag) {
        case ic_backend_pancake_value_type_boolean:
            value_to->u.boolean = value_from->u.boolean;
            break;

        case ic_backend_pancake_value_type_uint:
            value_to->u.uint = value_from->u.uint;
            break;

        case ic_backend_pancake_value_type_sint:
            value_to->u.sint = value_from->u.sint;
            break;

        case ic_backend_pancake_value_type_string:
            value_to->u.string = value_from->u.string;
            break;

        case ic_backend_pancake_value_type_ref:
            value_to->u.ref = value_from->u.ref;
            break;

        default:
            puts("ic_backend_pancake_value_copy: impossible value_from->tag");
            return 0;
    }

    return 1;
}

/* print value to provided file
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_backend_pancake_value_print(FILE *fd, struct ic_backend_pancake_value *value) {
    if (!value) {
        puts("ic_backend_pancake_value_print: value was null");
        return 0;
    }

    switch (value->tag) {
        case ic_backend_pancake_value_type_boolean:
            fprintf(fd, "bool: %u\n", value->u.boolean);
            break;

        case ic_backend_pancake_value_type_uint:
            fprintf(fd, "uint: %u\n", value->u.uint);
            break;

        case ic_backend_pancake_value_type_sint:
            fprintf(fd, "sint: %d\n", value->u.sint);
            break;

        case ic_backend_pancake_value_type_string:
            fprintf(fd, "string: %s\n", value->u.string);
            break;

        case ic_backend_pancake_value_type_ref:
            fprintf(fd, "ref: %p\n", value->u.ref);
            break;

        default:
            puts("ic_backend_pancake_value_print: impossible value->tag");
            return 0;
    }

    return 1;
}
