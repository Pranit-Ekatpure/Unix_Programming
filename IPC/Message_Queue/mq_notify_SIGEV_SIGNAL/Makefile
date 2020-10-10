include ../../../Make.defines

EXECUTABLE = mq_notify_SIGEV_SIGNAL

OBJS 	   = mqnotify.o

all: $(EXECUTABLE) 

mq_notify_SIGEV_SIGNAL: $(OBJS)

LIBS += -lrt

$(EXECUTABLE): 
	$(LD) -o $@  $^ $(LIBS)

%.o: %.c 
	$(CC) $(CFLAGS) -c -o $@ $^ 

clean:
	rm -f $(CLEANFILES) $(EXECUTABLE)