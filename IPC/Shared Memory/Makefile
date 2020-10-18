include ../../Make.defines

EXECUTABLE = shm_parent_child shm_parent_child1

OBJS = shm_parent_child.o
OBJS1 = shm_parent_child1.o

LIBS += -lpthread

all: $(EXECUTABLE) 

shm_parent_child: $(OBJS)
shm_parent_child1: $(OBJS1)

$(EXECUTABLE): 
	$(LD) -o $@  $^ $(LIBS)

%.o: %.c 
	$(CC) $(CFLAGS) -c -o $@ $^ 

clean:
	rm -f $(CLEANFILES) $(EXECUTABLE) temp