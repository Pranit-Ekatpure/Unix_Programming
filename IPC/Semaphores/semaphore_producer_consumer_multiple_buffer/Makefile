include ../../../Make.defines

EXECUTABLE = sem_prod_consum

OBJS = sem_prod_consum.o

LIBS += -lpthread

all: $(EXECUTABLE) 

sem_prod_consum: $(OBJS)

$(EXECUTABLE): 
	$(LD) -o $@  $^ $(LIBS)

%.o: %.c 
	$(CC) $(CFLAGS) -c -o $@ $^ 

clean:
	rm -f $(CLEANFILES) $(EXECUTABLE)