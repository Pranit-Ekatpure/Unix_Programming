include ../../Make.defines

EXECUTABLES = tcpserver tcpclient

OBJ_TCP_SERVER = tcpserver.o	
OBJ_TCP_CLIENT = tcpclient.o

LIBS += 

all: $(EXECUTABLES)

tcpserver: $(OBJ_TCP_SERVER)
tcpclient: $(OBJ_TCP_CLIENT)

$(EXECUTABLES): 
	$(LD) -o $@ $^ $(LIBS)

*.o: *.c
	$(CC) $(CFLAGS) -o $@ $^ 

clean:
	rm -f $(CLEANFILES) $(EXECUTABLES)