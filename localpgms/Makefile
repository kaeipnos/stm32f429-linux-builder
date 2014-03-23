CROSS_COMPILE ?= arm-uclinuxeabi-
ARCH = arm
# yes, quite ugly, i know, copy/past from busybox build process
CFLAGS = -march=armv7-m -mtune=cortex-m4 -mlittle-endian -mthumb -Os -ffast-math -ffunction-sections -fdata-sections -Wl,--gc-sections -fno-common --param max-inline-insns-single=1000 -Wl,-elf2flt=-s -Wl,-elf2flt=16384 -Wl,-elf2flt -static -I../staging/include/
LDFLAGS = -march=armv7-m -mtune=cortex-m4 -mlittle-endian -mthumb -Os -ffast-math -ffunction-sections -fdata-sections -Wl,--gc-sections -fno-common --param max-inline-insns-single=1000 -Wl,-elf2flt=-s -Wl,-elf2flt=16384 -Wl,-elf2flt -static

AS              = $(CROSS_COMPILE)as
CC              = $(CROSS_COMPILE)gcc
LD              = $(CC)
CPP             = $(CC) -E
AR              = $(CROSS_COMPILE)ar
NM              = $(CROSS_COMPILE)nm
STRIP           = $(CROSS_COMPILE)strip
OBJCOPY         = $(CROSS_COMPILE)objcopy
OBJDUMP         = $(CROSS_COMPILE)objdump

#EXEC = foo
#OBJS = foo.o

EXEC = spidev_test
OBJS = spidev_test.o

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

clean:
	rm -rf $(EXEC) *.elf *.gdb *.o bin/

install: $(EXEC)
	mkdir -p bin
	cp $(EXEC) bin/
