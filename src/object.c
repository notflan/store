#include <store.h>
#include <store/object.h>
#include <stdlib.h>
#include <string.h>
#include "_impl.h"

static object *_ob_new_unmanaged(object prototype)
{
	object* ret = malloc(sizeof(object));
	memcpy(ret, &prototype, sizeof(object));

	if(ret->constructor!=NULL)
		ret->constructor(ret);

	return ret;
}

/*static void _ob_free_unmanaged(object* obj)
{
	if(obj->destructor!=NULL)
		obj->destructor(obj);
	free(obj);
}*/

object* obj_new(store_t* store, object prototype)
{
	object* obj = _ob_new_unmanaged(prototype);

	store_t* s = _store_ptr(store, obj);
	s->is_obj = 1;

	return obj;
}

object* obj_copy(object* from, object* to)
{
	to->constructor = from->constructor;
	to->destructor = from->destructor;

	to->copy_to(from, to);
	return to;
}

object* obj_clone(store_t* store, object *from)
{
	object* new = malloc(sizeof(object));

	from->constructor(new);
	obj_copy(from, new);

	if(store)
	{
		store_t* s = _store_ptr(store, new);
		s->is_obj = 1;
	}

	return new;
}

void obj_free(store_t *store,object *obj)
{
	if(store->ptr == obj)
	{
		if(obj->destructor!=NULL)
			obj->destructor(obj);
		
		free(obj);
		store->ptr = NULL;
		store->is_obj=0;
	} else if (store->next) {
		obj_free(store->next,obj);
	}

}

static void _obj_copy_default(object* f, object* t)
{
	t->state = f->state;
}

const object OBJ_PROTO = {NULL,NULL,NULL, &_obj_copy_default};
