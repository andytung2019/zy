

.PHONY: clean

CFLAGS := -Wall -O2
LIB = -lpthread -lcurl
OBJS = collect.o\
       queue.o

test:$(OBJS)
	gcc  $^ -o $@ $(LIB)
%.o : %.c
	gcc -c -g $^ -o $@ 
clean:
	-rm -rf $(OBJS)
	-rm -rf test

