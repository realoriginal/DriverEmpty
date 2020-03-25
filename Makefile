CROSS_PREFIX := x86_64-w64-mingw32-
OBJ          := $(wildcard src/*.c)
OBJS         := $(OBJ:%.c=%.o)
TARGET       := driver.sys
CFLAGS       := -Isrc/ddk/ -DMINGW -std=c99 -Wall -m64 -shared

all: $(TARGET)

clean:
	rm -rf $(OBJS) $(TARGET)

.c.o:
	$(CROSS_PREFIX)gcc $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CROSS_PREFIX)gcc $(CFLAGS) -Os -Wl,--subsystem,native         \
	-Wl,--image-base,0x10000 -Wl,--file-alignment,0x1000            \
	-Wl,--entry,DriverEntry@8 -Wl,--stack,0x40000 -Wl,--dynamicbase \
	-Wl,--nxcompat -nostartfiles -nostdlib -o $(TARGET) $(OBJS)     \
	-lntoskrnl -lhal -Wint-to-pointer-cast
	x86_64-w64-mingw32-strip -s $(TARGET)
