.DEFAULT_GOAL := build
DLL_TEST = dll
CFLAGS += -g -Wall -std=c11
leak : CFLAGS += -fsanitize=address -ggdb 
leak : build

build: $(DLL_TEST)

dll : dll.o 
	$(CC) $(CFLAGS) -o dll dll.o 

dll.o : dll.c 
	$(CC) $(CFLAGS) -c dll.c

clean :
	rm $(DLL_TEST) dll.o 