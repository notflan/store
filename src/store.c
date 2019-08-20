#include <store.h>
#include <store/object.h>
#include "_impl.h"
#include <string.h>
#include <stdlib.h>

store_t *store_new()
{
	store_t *s = malloc(sizeof(store_t));
	memset(s,0,sizeof(store_t));
	return s;
}

void* store_malloc(store_t *store, size_t sz)
{
	void ** sptr;
	if(store->ptr == NULL)
		sptr = &store->ptr;
	else if (store->next == NULL) {
		store->next = store_new();
		return store_malloc(store->next, sz);
	} else {
		return store_malloc(store->next, sz);
	}

	*sptr = malloc(sz);
	memset(*sptr, 0, sz);

	return *sptr;
}

void store_free(store_t* store, void* ptr)
{
	if(store->ptr == ptr)
	{
		if(store->is_obj)
		{
			object* obj = store->ptr;
			if(obj->destructor!=NULL)
				obj->destructor(obj);
		}
		free(ptr);
		store->ptr = NULL;
		store->is_obj=0;
	} else if (store->next) {
		store_free(store->next,ptr);
	}
}

internal store_t* _store_first_empty(store_t *store, int obj)
{
	if(!store->ptr) { store->is_obj=obj;  return store; }
	else if(store->next) return _store_first_empty(store->next, obj);
	else {
		store->next = store_new();
		store->next->is_obj = obj;
		return store->next;
	}
}

internal store_t* _store_ptr(store_t *store, void* heap)
{
	if(!store->ptr)
		store->ptr = heap;
	else if(store->next)
		return _store_ptr(store->next, heap);
	else {
		store->next = store_new();
		return _store_ptr(store->next, heap);
	}
	return store;

}

void* store_ptr(store_t *store, void *heap)
{
	if(!store->ptr)
		store->ptr = heap;
	else if(store->next)
		return store_ptr(store->next, heap);
	else {
		store->next = store_new();
		return store_ptr(store->next, heap);
	}
	return store->ptr;
}

void store_destroy(store_t *store)
{
	while(store)
	{
		if(store->ptr)
		{
			if(store->is_obj)
			{
				object *obj = store->ptr;
				if(obj->destructor!=NULL)
					obj->destructor(obj);
			}
			free(store->ptr);

		}
		store_t *next = store->next;
		
		free(store);
		store = next;
	}
}

void store_collect(store_t *store)
{
	int first=1;
	while(store)
	{
		if(store->ptr)
		{
			if(store->is_obj)
			{
				object *obj = store->ptr;
				if(obj->destructor!=NULL)
					obj->destructor(obj);
			}

			free(store->ptr);
		}
		store_t *next = store->next;
		store->is_obj=0;
		if(!first) free(store);
		first=0;
		store=next;
	}
}

