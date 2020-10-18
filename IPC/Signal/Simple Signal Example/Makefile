include ../../../Make.defines

EXECUTABLE = signal

OBJS = signal.o

all: $(EXECUTABLE) 

signal: $(OBJS)

$(EXECUTABLE): 
	$(LD) -o $@  $^ $(LIBS)

%.o: %.c 
	$(CC) $(CFLAGS) -c -o $@ $^ 

clean:
	rm -f $(CLEANFILES) $(EXECUTABLE) 