include ../../../Make.defines

EXECUTABLE = alarm

OBJS = alarm.o

all: $(EXECUTABLE) 

alarm: $(OBJS)

$(EXECUTABLE): 
	$(LD) -o $@  $^ $(LIBS)

%.o: %.c 
	$(CC) $(CFLAGS) -c -o $@ $^ 

clean:
	rm -f $(CLEANFILES) $(EXECUTABLE) 