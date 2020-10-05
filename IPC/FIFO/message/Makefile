include ../../../Make.defines

EXECUTABLE = main_client_server

OBJS 	   = mesg_send.o \
	   		 mesg_recv.o \
	   		 client.o \
	      	 server.o \
			 main_client_server.o \

all: $(EXECUTABLE) 

main_client_server: $(OBJS)

$(EXECUTABLE): 
	$(LD) -o $@  $^ 

%.o: %.c 
	$(CC) $(CFLAGS) -c -o $@ $^ 

clean:
	rm -f $(CLEANFILES) $(EXECUTABLE)