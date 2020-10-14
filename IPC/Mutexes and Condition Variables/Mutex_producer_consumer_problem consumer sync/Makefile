include ../../../Make.defines

EXECUTABLE = prod_consum

OBJS = prod_consum.o

LIBS += -lpthread

all: $(EXECUTABLE) 

prod_consum: $(OBJS)

$(EXECUTABLE): 
	$(LD) -o $@  $^ $(LIBS)

%.o: %.c 
	$(CC) $(CFLAGS) -c -o $@ $^ 

clean:
	rm -f $(CLEANFILES) $(EXECUTABLE)