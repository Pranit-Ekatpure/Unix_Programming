include ../../../Make.defines

EXECUTABLE = mq_notify_SIGEV_THREAD

OBJS 	   = mqnotify.o

all: $(EXECUTABLE) 

mq_notify_SIGEV_THREAD: $(OBJS)

LIBS += -lrt

$(EXECUTABLE): 
	$(LD) -o $@  $^ $(LIBS)

%.o: %.c 
	$(CC) $(CFLAGS) -c -o $@ $^ 

clean:
	rm -f $(CLEANFILES) $(EXECUTABLE)