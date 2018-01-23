TARGET = libpwr5.so

CC = gcc
LD = gcc
RM = rm -f

CFLAGS = -Wall -std=gnu99 -fPIC -DDEBUG_ENABLED -I../inc
LDFLAGS = -shared

.PHONY: clean

obj: 
	$(CC) $(CFLAGS) -c battery.c -o battery.o
	$(CC) $(CFLAGS) -c sysfsutil.c -o sysfsutil.o
	$(CC) $(CFLAGS) -c backlight.c -o backlight.o

all:
	$(LD) $(LDFLAGS) *.o -o $(TARGET)
	@echo "Build done!"

clean:
	$(RM) $(TARGET)
	$(RM) *.o
	@echo "Clean done!"
