
all: test

clean:
	rm -f *.a

store: clean
	gcc -c src/*.c -g -Wall -Iinclude/ -std=gnu11 -D__STORE_IMPLEMENTATION
	ar rc lib$@.a *.o
	ranlib lib$@.a
	rm -f *.o

test: store
	gcc src/test/*.c -g -Wall -Iinclude/ -std=gnu11 -o test -L. -lstore
	valgrind ./test
	rm test
