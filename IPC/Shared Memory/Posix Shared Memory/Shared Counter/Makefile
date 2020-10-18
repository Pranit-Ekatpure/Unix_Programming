include ../../../../Make.defines

EXECUTABLE = server client1 client2 client3

OBJS_SERVER  = server.o
OBJS_CLIENT1 = client1.o
OBJS_CLIENT2 = client2.o
OBJS_CLIENT3 = client3.o

LIBS += -lpthread -lrt

all: $(EXECUTABLE) 

server: $(OBJS_SERVER)
client1: $(OBJS_CLIENT1)
client2: $(OBJS_CLIENT2)
client3: $(OBJS_CLIENT3)

$(EXECUTABLE): 
	$(LD) -o $@  $^ $(LIBS)

%.o: %.c 
	$(CC) $(CFLAGS) -c -o $@ $^ 

clean:
	rm -f $(CLEANFILES) $(EXECUTABLE) 