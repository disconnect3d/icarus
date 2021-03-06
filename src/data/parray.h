#ifndef ICARUS_PARRAY_H
#define ICARUS_PARRAY_H

/* pointer array
 * a dynamic bounds checked array
 * stores an array of void pointers
 * caller is responsible for managing the contents
 *
 */
struct ic_parray {
    unsigned int len;
    /* note that we store an array of void*
     * so we must be careful what we actually store here
     */
    void **contents;
};

/* allocate a new array of length len
 *
 * returns array on success
 * returns 0 on failure
 */
struct ic_parray *ic_parray_new(unsigned int len);

/* initialise an existing array to the specified len
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_parray_init(struct ic_parray *arr, unsigned int len);

/* destroy this parray
 * will only free parray itself if free_parr is truthy
 *
 * this does NOT free the items in the parray as parray does not store length,
 * this is the user's responsibility
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_parray_destroy(struct ic_parray *arr, unsigned int free_parr);

/* get item at pos
 *
 * bounds checked
 *
 * returns item on success
 * returns 0 on failure
 */
void *ic_parray_get(struct ic_parray *arr, unsigned int pos);

/* set element at [pos] to val
 *
 * bounds checked
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_parray_set(struct ic_parray *arr, unsigned int pos, void *val);

/* ensure array is at least as big as `new_len`
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_parray_ensure(struct ic_parray *arr, unsigned int new_len);

#endif
