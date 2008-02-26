CC=gcc
CFLAGS=-W -Wall -fno-builtin

TARGET=LOADME.elf

OBJS=main.o \
     video.o \
     serial.o \
     init.o

LDSCRIPT=layout.ld

all: $(TARGET)

$(TARGET): $(OBJS)
	ld -T $(LDSCRIPT) -o $@ $(OBJS)

%.o: %.S
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f $(TARGET)
	rm -f $(OBJS)
