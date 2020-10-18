include ../../../Make.defines

EXECUTABLE = sigsuspend sigsuspend1

OBJS = sigsuspend.o
OBJS1 = sigsuspend1.o

all: $(EXECUTABLE) 

sigsuspend: $(OBJS)
sigsuspend1: $(OBJS1)

$(EXECUTABLE): 
	$(LD) -o $@  $^ $(LIBS)

%.o: %.c 
	$(CC) $(CFLAGS) -c -o $@ $^ 

clean:
	rm -f $(CLEANFILES) $(EXECUTABLE) 