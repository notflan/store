/* store/array_impl - Abstractable array implementation details.
 *
 */
#ifndef _ARRAY_IMPL_H
#define _ARRAY_IMPL_H

#include "object.h"

struct array {
	object* base; //The object this array_t is assigned to.

	size_t size;	//Size of this array in elements
	size_t element;	//Size of a single element.

	size_t full_size; //Full size in bytes

	void *tl_state; //Second state variable.
	void* state;	//First state variable.

	int (*dump)(const array_t this, void* buffer, size_t buffermax, size_t offset, size_t count); //(optional) The dump function for this array_t, otherwise it is copied element by element. (NOTE: this should handle bounds checking)
	void* (*get)(struct array* this, long i); //(required) the get function for this element. Returned pointer is not guaranteed to be mutable of the underlying implementation's array, and is returned const from ar_get() (NOTE: This should handle bounds checking)
	void (*set)(struct array* this, long i, const void* item); //(required) the set function for this element. (NOTE: This should handle bounds checking)

	unsigned char data[]; //Any additional data.
}; //*array_t

#endif /* _ARRAY_IMPL_H */
