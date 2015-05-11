#ifndef ICARUS_SET_H
#define ICARUS_SET_H

#include "../../libs/linear_set/linear_set.h"

/* this setionary type maps strings * to void
 * this type is a wrapper around
 *  linear_hash.h:struct lh_table
 */
struct ic_set{
    struct ls_set lss;
};

/* allocate and init a new ic_set
 *
 * returns pointer on success
 * returns 0 on error
 */
struct ic_set * ic_set_new(void);

/* init an existing ic_set
 *
 * return 0 on success
 * return 1 on error
 */
unsigned int ic_set_init(struct ic_set *set);

/* insert new item
 *
 * returns 1 on success
 * returns 0 on error
 */
unsigned int ic_set_insert(struct ic_set *set, char *item);

/* check if key already exists in set
 *
 * returns 1 on success
 * returns 0 on error
 */
unsigned int ic_set_exists(struct ic_set *set, char *key);

/* delete element at key
 * only works if key exists
 *
 * returns 1 on success
 * returns 0 on error
 */
unsigned int ic_set_delete(struct ic_set *set, char *key);

/* free an existing ic_set
 * this will free all memory internal to the hash
 *
 * if free_set is true then it will also free the ic_set *set
 *
 * returns 1 on success
 * returns 0 on error
 */
unsigned int ic_set_destroy(struct ic_set *set, unsigned int free_set);

#endif