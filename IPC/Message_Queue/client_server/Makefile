include ../../../Make.defines

EXECUTABLE = client server

OBJS_CLIENT = client.o 
OBJS_SERVER = server.o 

LIBS += -lrt

all: $(EXECUTABLE) 

client: $(OBJS_CLIENT)
server: $(OBJS_SERVER)

$(EXECUTABLE): 
	$(LD) -o $@  $^ $(LIBS)

%.o: %.c 
	$(CC) $(CFLAGS) -c -o $@ $^ 

clean:
	rm -f $(CLEANFILES) $(EXECUTABLE)