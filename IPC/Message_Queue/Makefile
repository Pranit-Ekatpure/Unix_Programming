include ../../Make.defines

EXECUTABLE = mqcreate \
			 mqgetattr \
   			 mqsend \
			 mqreceive \
			 mqunlink
 
SRC_DIR_MQ_CREA	:= mq_create
SRC_DIR_MQ_GATR	:= mq_getattr
SRC_DIR_MQ_SEND := mq_send
SRC_DIR_MQ_RECV	:= mq_receive
SRC_DIR_MQ_UNLK	:= mq_unlink

SRC_MQ_CREA := $(wildcard $(SRC_DIR_MQ_CREA)/*.c)
SRC_MQ_GATR := $(wildcard $(SRC_DIR_MQ_GATR)/*.c)
SRC_MQ_SEND := $(wildcard $(SRC_DIR_MQ_SEND)/*.c)
SRC_MQ_RECV := $(wildcard $(SRC_DIR_MQ_RECV)/*.c)
SRC_MQ_UNLK := $(wildcard $(SRC_DIR_MQ_UNLK)/*.c)

OBJ_MQ_CREA := $(SRC_MQ_CREA:$(SRC_DIR_MQ_CREA)%.c = %.o)
OBJ_MQ_GATR := $(SRC_MQ_GATR:$(SRC_DIR_MQ_GATR)%.c = %.o)
OBJ_MQ_SEND := $(SRC_MQ_SEND:$(SRC_DIR_MQ_SEND)%.c = %.o)
OBJ_MQ_RECV := $(SRC_MQ_RECV:$(SRC_DIR_MQ_RECV)%.c = %.o)
OBJ_MQ_UNLK := $(SRC_MQ_UNLK:$(SRC_DIR_MQ_UNLK)%.c = %.o)

mqcreate:		$(OBJ_MQ_CREA)
mqgetattr:		$(OBJ_MQ_GATR)
mqsend: 		$(OBJ_MQ_SEND)
mqreceive: 		$(OBJ_MQ_RECV)
mqunlink:		$(OBJ_MQ_UNLK)

LIBS += -lrt

all: $(EXECUTABLE) 

$(EXECUTABLE): 
	$(LD) -o $@  $^ $(LIBS)

%.o: %.c 
	$(CC) $(CFLAGS) -c -o $@ $^ 

clean:
	rm -f $(CLEANFILES) $(EXECUTABLE)