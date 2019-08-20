#ifndef __STORE_IMPLEMENTATION
#error "You shouldn't include this."
#endif 

#ifndef _STORE_IMPL_H
#define _STORE_IMPL_H

#include <store.h>

struct store_link {
	void* ptr;

	int is_obj;

	struct store_link* next;
};

#ifdef __GNUC__
#define internal __attribute__((visibility ("internal")))
#else
#define internal
#endif

internal store_t* _store_ptr(store_t *store, void* heap);
internal store_t* _store_first_empty(store_t *store, int obj);

#endif /* _STORE_IMPL_H */
