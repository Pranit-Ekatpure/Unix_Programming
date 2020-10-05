include ../../../Make.defines

OBJ_MAIN_SERVER  = main_server.o 
OBJ_MAIN_CLIENT1 = main_client1.o 
OBJ_MAIN_CLIENT2 = main_client2.o 

EXECUTABLE = main_server \
			 main_client1 \
			 main_client2

main_server: $(OBJ_MAIN_SERVER)
main_client1: $(OBJ_MAIN_CLIENT1)
main_client2: $(OBJ_MAIN_CLIENT2)

all: $(EXECUTABLE) 

$(EXECUTABLE): 
	$(LD) -o $@  $^ 

%.o: %.c 
	$(CC) $(CFLAGS) -c -o $@ $^ 

clean:
	rm -f $(CLEANFILES) $(EXECUTABLE) fifo.*