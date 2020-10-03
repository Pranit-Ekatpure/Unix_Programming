include ../../Make.defines

EXECUTABLES = udpserver udpclient

OBJ_UDP_SERVER = udpserver.o	
OBJ_UDP_CLIENT = udpclient.o

LIBS += 

all: $(EXECUTABLES)

udpserver: $(OBJ_UDP_SERVER)
udpclient: $(OBJ_UDP_CLIENT)

$(EXECUTABLES): 
	$(LD) -o $@ $^ $(LIBS)

*.o: *.c
	$(CC) $(CFLAGS) -o $@ $^ 

clean:
	rm -f $(CLEANFILES) $(EXECUTABLES)