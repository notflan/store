/* store/array - Abstratable array.
 * (include store/array_impl.h to implement your own)
 */

#ifndef _ARRAY_H
#define _ARRAY_H
#include "../store.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct array* array_t; //Managed array abstraction

array_t ar_create_file(S_LEXENV, FILE* from, size_t element_size, int owns_stream);		//Create an array_t abstraction from a FILE* stream
array_t ar_create_memory_from(S_LEXENV, const void* from, size_t element_size, size_t count);	//Create an in-memory array_t abstraction copying values from a buffer
array_t ar_create_memory(S_LEXENV, size_t element_size, size_t count, int init0);			//Create an in-memory array_t abstraction

const void* ar_get(const array_t this, long i);							//Get a pointer to a value from array_t
void ar_set(array_t this, long i, const void* obj);						//Set a value from obj to array_t
long ar_size(const array_t this);								//Get the size (in elements) of array_t
long ar_full_size(const array_t this);								//Get the full size (in bytes) of array_t (note: this is not neccisarliy the same as ar_size()*ar_type())
long ar_type(const array_t this);								//Get the size of the element type of array_t
int ar_get_into(const array_t this, long i, void* buffer);					//Copy value i from array_t into buffer
void ar_swap(array_t from, long i, long j);							//Swap 2 values in array_t
void ar_reinterpret(array_t from, size_t element_size);						//Reinterpret array_t as a different type
int ar_dump(const array_t from, void* buffer, size_t offset, size_t count);			//Dump an array_t to a buffer, starting at offset, and ending after count elements have been written or array would overflow. Returns 1 if completed entirely, 0 if failed (due to either being out of buffer bounds or array bounds).
int ar_ndump(const array_t from, void* buffer, size_t bufsize, size_t offset, size_t count);	//Dump no more than bufsize bytes to buffer, starting at offset, and ending after count elements have been written or the buffer would overflow. Returns 1 if completed entirely, 0 if failed (due to either being out of buffer bounds or array bounds).

//Get a value from array_t of spcified type
#define ar_get_v(this, i, type) (*((const type*)ar_get(this, i)))

#ifndef __GNUC__
//Set a value to array_t (NOTE: must be variable)
#define ar_set_v(this, i, value) ar_set(this, i, &value)
#else
//Set a value to array_t (NOTE: can be expression or variable)
#define ar_set_v(this, i, value) { __auto_type __internal_value = (value); ar_set(this, i, &__internal_value); }
#endif

#endif /* _ARRAY_H */
