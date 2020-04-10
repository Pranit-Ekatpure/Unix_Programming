
OBJS = main_client_server.o \
	   mesg_send.o \
	   mesg_recv.o \
	   client.o \
	   server.o 

all: main_client_server clean

main_client_server: $(OBJS) mesg.h
	gcc -o main_client_server $(OBJS)

%.o:%.c mesg.h
	gcc -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS)