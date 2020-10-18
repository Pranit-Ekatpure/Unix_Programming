include ../../../../Make.defines

EXECUTABLE = shm_client_server_server \
			 shm_client_server_client1 \
			 shm_client_server_client2 \
			 shm_client_server_client3

OBJS_SERVER  = shm_client_server_server.o
OBJS_CLIENT1 = shm_client_server_client1.o
OBJS_CLIENT2 = shm_client_server_client2.o
OBJS_CLIENT3 = shm_client_server_client3.o

LIBS += -lpthread -lrt

all: $(EXECUTABLE) 

shm_client_server_server: $(OBJS_SERVER)
shm_client_server_client1: $(OBJS_CLIENT1)
shm_client_server_client2: $(OBJS_CLIENT2)
shm_client_server_client3: $(OBJS_CLIENT3)

$(EXECUTABLE): 
	$(LD) -o $@  $^ $(LIBS)

%.o: %.c 
	$(CC) $(CFLAGS) -c -o $@ $^ 

clean:
	rm -f $(CLEANFILES) $(EXECUTABLE) 