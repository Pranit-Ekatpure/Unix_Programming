include ../../Make.defines

EXECUTABLES = sctpserver sctpclient

OBJ_SCTP_SERVER = sctpserver.o	
OBJ_SCTP_CLIENT = sctpclient.o

LIBS += -lsctp

all: $(EXECUTABLES)

sctpserver: $(OBJ_SCTP_SERVER)
sctpclient: $(OBJ_SCTP_CLIENT)

$(EXECUTABLES): 
	$(LD) -o $@ $^ $(LIBS)

*.o: *.c
	$(CC) $(CFLAGS) -o $@ $^ 

clean:
	rm -f $(CLEANFILES) $(EXECUTABLES)