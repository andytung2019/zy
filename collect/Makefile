

.PHONY: clean

CFLAGS := -Wall -O2
LIB = -lpthread -lcurl -lmodbus
OBJS = collect.o\
       queue.o\
	   server_cmd.o\
	   mod_cmd.o\
	   cJSON.o


	 
test:$(OBJS)
	gcc  $^ -o $@ $(LIB)
%.o : %.c
	gcc -c -g $^ -o $@  
clean:
	-rm -rf $(OBJS)
	-rm -rf test

