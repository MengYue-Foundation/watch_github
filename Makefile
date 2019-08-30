
CROSS_COMPILE = 
AS		= $(CROSS_COMPILE)as
LD		= $(CROSS_COMPILE)ld
CC		= $(CROSS_COMPILE)gcc
CPP		= $(CC) -E
AR		= $(CROSS_COMPILE)ar
NM		= $(CROSS_COMPILE)nm

STRIP		= $(CROSS_COMPILE)strip
OBJCOPY		= $(CROSS_COMPILE)objcopy
OBJDUMP		= $(CROSS_COMPILE)objdump

export AS LD CC CPP AR NM
export STRIP OBJCOPY OBJDUMP

CFLAGS := -Wall -O2 -ggdb3
CFLAGS += -I $(shell pwd)/include  -I $(shell pwd)/screen -I $(shell pwd)/Fonts  -I $(shell pwd)/key/ -I $(shell pwd)/time -I $(shell pwd)/watch -I $(shell pwd)/common -I $(shell pwd)/ATserial -I$(shell pwd)/taptic_engine

LDFLAGS := -lm -lwiringPi -lpthread -lcurl -lcrypto

export CFLAGS LDFLAGS

TOPDIR := $(shell pwd)
export TOPDIR

TARGET := show_file



obj-y += common/
obj-y += screen/
obj-y += taptic_engine/
obj-y += Fonts/
obj-y += key/
obj-y += time/
obj-y += watch/
obj-y += ATserial/
obj-y += voice/
obj-y += app/
obj-y += main.o


all : 
	make -C ./ -f $(TOPDIR)/Makefile.build
	$(CC) $(LDFLAGS) -o $(TARGET) built-in.o


clean:
	rm -f $(shell find -name "*.o")
	rm -f $(TARGET)

distclean:
	rm -f $(shell find -name "*.o")
	rm -f $(shell find -name "*.d")
	rm -f $(TARGET)
	