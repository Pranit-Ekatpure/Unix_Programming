include ../../../Make.defines

EXECUTABLE = client_server

OBJS 	   = client_server.o 

all: $(EXECUTABLE) 

client_server: $(OBJS)

$(EXECUTABLE): 
	$(LD) -o $@  $^ 

%.o: %.c 
	$(CC) $(CFLAGS) -c -o $@ $^ 

clean:
	rm -f $(CLEANFILES) $(EXECUTABLE)