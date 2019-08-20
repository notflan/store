/* store/array_impl - Abstractable array implementation details.
 *
 */
#ifndef _ARRAY_IMPL_H
#define _ARRAY_IMPL_H

#include "object.h"

struct array {

	object* base; //(readonly) The object this array_t is assigned to.

	size_t size;	//(required) Size of this array in elements
	size_t element;	//(required) Size of a single element.

	size_t full_size; //(required) Full size in bytes

	void *tl_state; //(optional) Second state variable.
	void* state;	//(optional) First state variable.

	void (*constructor)(array_t this); //(optional) custom constructor for this array.
	void (*destructor)(array_t this); //(optional) custom destructor for this array. (Do not call free(this) here, that is handled elsewhere)

	int (*dump)(const array_t this, void* buffer, size_t buffermax, size_t offset, size_t count); //(optional) The dump function for this array_t, otherwise it is copied element by element. (NOTE: this should handle bounds checking)
	void* (*get)(struct array* this, long i); //(required) the get function for this element. Returned pointer is not guaranteed to be mutable of the underlying implementation's array, and is returned const from ar_get() (NOTE: This should handle bounds checking)
	void (*set)(struct array* this, long i, const void* item); //(required) the set function for this element. (NOTE: This should handle bounds checking)


	unsigned char data[]; //(optional, assigned by _ar_create()) Any additional data.
}; //*array_t

const extern struct array ARRAY_PROTO; //Prototype for array decleration. (memset()ing a struct array* to 0 is fine too).

array_t _ar_create(S_LEXENV, struct array proto, int extra_bytes, int init_extra_bytes_to_zero); //Create a new array_t abstraction from prototype.

#endif /* _ARRAY_IMPL_H */
