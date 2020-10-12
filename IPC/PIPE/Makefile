include ../../Make.defines

EXECUTABLE = client_server fullduplexpipe

OBJ_CLIENT_SERVER = client_server.o
OBJ_FULLDUPLEXPIPE = fullduplexpipe.o

all: $(EXECUTABLE) 

client_server: $(OBJ_CLIENT_SERVER)
fullduplexpipe: $(OBJ_FULLDUPLEXPIPE)

$(EXECUTABLE): 
	$(LD) -o $@  $^ 

%.o: %.c 
	$(CC) $(CFLAGS) -c -o $@ $^ 

clean:
	rm -f $(CLEANFILES) $(EXECUTABLE)