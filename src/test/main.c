#include <stdio.h>
#include <stdlib.h>

#include <store.h>
#include <store/array.h>

int main()
{
	MANAGED({
		array_t array = ar_create_memory(S_THIS, sizeof(int), 10, 1);

		Smalloc(2048);

		for(int i=0;i<ar_size(array);i++)
		{
			ar_set_v(array, i, i*10);
		}

		ar_reinterpret(array, sizeof(int8_t));
		for(int i=0;i<ar_size(array);i++)
		{
			printf("%d ", (int)ar_get_v(array, i, int8_t));
		}
		printf("\n");
		ar_reinterpret(array, sizeof(int));
		for(int i=0;i<ar_size(array);i++)
		{
			printf("%d-%d ", i, ar_get_v(array, i, int));
		}
		printf("\n");
	});
	return 0;
}
