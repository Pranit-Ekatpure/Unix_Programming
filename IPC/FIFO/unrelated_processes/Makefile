include ../../../Make.defines

EXECUTABLE = main_client main_server

OBJ_MAIN_CLIENT	= main_client.o client.o 
OBJ_MAIN_SERVER =main_server.o server.o 

all: $(EXECUTABLE) 

main_client: $(OBJ_MAIN_CLIENT)
main_server: $(OBJ_MAIN_SERVER)

$(EXECUTABLE): 
	$(LD) -o $@  $^ 

%.o: %.c 
	$(CC) $(CFLAGS) -c -o $@ $^ 

clean:
	rm -f $(CLEANFILES) $(EXECUTABLE)