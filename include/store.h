#ifndef _PTR_STORE_H
#define _PTR_STORE_H

#include <stddef.h>

typedef struct store_link store_t;

store_t *store_new(); 				// Create a new empty store
void* store_malloc(store_t *store, size_t sz);	//malloc() sz bytes and add to store
void store_free(store_t* store, void* ptr);	//Free a single pointer from store
void store_destroy(store_t *store);		//Free all pointers in store and free the store itself
void* store_ptr(store_t *store, void *heap);	//Add a pointer to the store
void store_collect(store_t *store);		//Free all pointers in the store and empty it

//MANAGED's environment with name specified as function argument
#define S_LEXENV_NAMED(n) store_t* n
//MANAGED's environment as function argument
#define S_LEXENV store_t* __internal_store
//MANAGED's current environment
#define S_THIS __internal_store

//Collect from MANAGED's environment
#define Scollect() store_collect(__internal_store)
//malloc() to MANAGED's environment
#define Smalloc(sz) store_malloc(__internal_store, sz)
//New object added to MANAGED's environment
#define Snew_t(type) ((type*)Smalloc(sizeof(type)))
//Create an internal store environment, and allow access to this with S* macros
#define MANAGED(var) { \
				store_t * __internal_store = store_new(); \
				var; \
				goto __internal_breakpoint; /*to suppress unused warning*/ \
				__internal_breakpoint: \
				store_destroy(__internal_store); \
			}
//MANAGED but able to safely return from the function
#define MANAGED_RETURNABLE(type, var) { \
						type __internal_retval; \
						int __internal_has_set_return =0; \
						store_t * __internal_store = store_new(); \
						var; \
						goto __internal_breakpoint; /*to suppress unused warning*/ \
						__internal_breakpoint: \
						store_destroy(__internal_store); \
						if(__internal_has_set_return) return __internal_retval; \
					}
//Safely return from a MANAGED_RETURNABLE block
#define MANAGED_RETURN(value) __internal_retval = value; \
				__internal_has_set_return = 1; \
				MANAGED_BREAK

//Safely exit a MANAGED block
#define MANAGED_BREAK goto __internal_breakpoint

#endif /* _PTR_STORE_H */
