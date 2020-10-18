include ../Make.defines

EXECUTABLE = simple_thread thread_cancellation

OBJ_SIMPLE_THREAD = simple_thread.o
OBJ_THREAD_CANCEL = thread_cancellation.o

LIBS += -lpthread

all: $(EXECUTABLE) 

simple_thread: $(OBJ_SIMPLE_THREAD)
thread_cancellation: $(OBJ_THREAD_CANCEL)

$(EXECUTABLE): 
	$(LD) -o $@  $^ $(LIBS)

%.o: %.c 
	$(CC) $(CFLAGS) -c -o $@ $^ 

clean:
	rm -f $(CLEANFILES) $(EXECUTABLE) 