#CROSS_COMPILE 	?= arm-linux-gnueabihf-
CROSS_COMPILE 	?= 

CC 			:= $(CROSS_COMPILE)gcc
LD			:= $(CROSS_COMPILE)ld
OBJCOPY 		:= $(CROSS_COMPILE)objcopy
OBJDUMP 		:= $(CROSS_COMPILE)objdump

TARGET		  	:= EMS_Local

LDFLAGS := -lpthread -lsqlite3 -lcurl -lssl -lm -g -rdynamic
#-lsqlite3

#SRC = keyApp.c
#SRC += net_deal.c
SRC         += $(wildcard *.c)


OBJS = $(SRC:%.c=%.o)

$(EXEC): $(OBJS)

all :
	$(CC) $(CFLAGS)  $(SRC) -o $(TARGET) $(LDFLAGS)


.PHONY:clean
clean:
	rm *.o run
