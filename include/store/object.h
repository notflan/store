/* store/object - Simple object model with con/destructors called by store
 *
 */

#ifndef _OBJECT_H
#define _OBJECT_H
#include "../store.h"

typedef struct store_obj {
	void* state; //This object's state information.

	void (*constructor)(struct store_obj* this);	//(optional) Constructor for this object. Called when added to store.
	void (*destructor)(struct store_obj* this);	//(optional) Destructor for this object. Called when freed from store.
	
	void (*copy_to)(struct store_obj* this, struct store_obj* other);	//(optional) Object's copy function, the default copies state.
} object;

object* obj_new(store_t* store, object prototype);	//Create a new object in store from prototype
object* obj_copy(object* from, object* to);		//Copy an object to another
object* obj_clone(store_t* store, object *from);	//Clone an object
void obj_free(store_t *store,object *obj);		//Free an object from store

const extern object OBJ_PROTO;	//Base object prototype, you should build yours from this.

//Delete a pointer, or an object from MANAGED's environment
#define Sdelete(ptr) _Generic((ptr), object*: obj_free, default: store_free)(__internal_store, ptr)
//Create a new object in MANAGED's environment
#define Snew_obj(proto) obj_new(__internal_store, proto)

//Create a new object from proto, or a new pointer if proto is not an object prototype
#define Snew(proto) _Generic((proto), object: Snew_obj, default: Snew_t)(proto)

#endif /* _OBJECT_H */
