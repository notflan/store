#include <stdio.h>
#include <stdlib.h>

#include <store.h>
#include <store/array.h>

#include <store/array_impl.h>
#include <string.h>

void _custom_array_constructor(array_t this)
{
	int *as_ints = (int*)this->data;
	for(int i=0;i<this->size;i++)
	{
		as_ints[i] = i;
	}
}

void _custom_array_set(array_t this, long i, const void* value)
{
	if(i<0 || i>= this->size) return;

	memcpy(this->data+(this->full_size- ((i+1) * this->element)), value, this->element);
}

int _custom_array_dump(const array_t this, void* buffer, size_t bs, size_t offset, size_t count)
{
	if(offset+count >= this->size) return 0;
#define min(x,y) (x<y?x:y)
	memcpy(buffer, this->data+(offset*this->element), min(bs, (this->element*count)));

	return min(bs, (this->element*count)) == (this->element*count);
#undef min
}

void* _custom_array_get(array_t this, long i)
{
	if(i<0 || i>= this->size) return NULL;
	
	return &this->data[this->full_size-((i+1)*this->element)];
}

//Create a custom array implementation that initialises to a range and then gets/sets backwards.
array_t arc_create_example(S_LEXENV, int size)
{
	struct array proto = ARRAY_PROTO;

	proto.constructor = &_custom_array_constructor;
	proto.size=size;
	proto.element=sizeof(int);
	proto.full_size = size*sizeof(int);
	proto.set = &_custom_array_set;
	proto.get = &_custom_array_get;
	proto.dump = &_custom_array_dump;
	
	return _ar_create(S_THIS, proto, size*sizeof(int), 1);
}

int main()
{
	MANAGED({
		array_t array = ar_create_memory(S_THIS, sizeof(int), 10, 1); //new int array of 10 elemnts, zeroed out.
		array_t custom = arc_create_example(S_THIS, 20); //new custom array of 20 elements.

		Smalloc(2048); //throwaway allocation.

		//set each value in int array to index*10
		for(int i=0;i<ar_size(array);i++)
		{
			ar_set_v(array, i, i*10);
		}

		//reinterpret array as bytes.
		ar_reinterpret(array, sizeof(int8_t));
		for(int i=0;i<ar_size(array);i++)
		{
			printf("%d ", (int)ar_get_v(array, i, int8_t));
		}
		printf("\n");

		//reinterpret back to ints
		ar_reinterpret(array, sizeof(int));
		for(int i=0;i<ar_size(array);i++)
		{
			printf("%d-%d ", i, ar_get_v(array, i, int));
		}
		printf("\n");

		//print whole custom
		for(int i=0;i<ar_size(custom);i++)
		{
			printf("%d ", ar_get_v(custom, i, int));
		}
		printf("\n");

		int* buffer = Smalloc(ar_full_size(custom));

		//dump custom
		ar_ndump(custom, buffer, ar_full_size(custom), 0, ar_size(custom));

		//print dump
		for(int i=0;i<ar_size(custom);i++)
		{
			printf("%d ", i);
		}
		printf("\n");

	});
	return 0;
}
