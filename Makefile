SOURCES = ppp.c
TARGET = ppp

CFLAGS = -Wall -Wextra -Werror -pedantic -std=c99 -O3 -fvisibility=hidden
INCLUDES = -I/usr/include/glib-2.0

.PHONY: clean

all: linux windows

linux: $(SOURCES)
	$(CC) $(CFLAGS) $(INCLUDES) $(SOURCES) -o $(TARGET).so -shared -fPIC

windows: $(SOURCES)
	$(CC) $(CFLAGS) $(INCLUDES) $(SOURCES) -o $(TARGET).dll -shared -Wl,--out-implib,$(TARGET).dll.a

clean:
	rm -f $(TARGET).so $(TARGET).dll $(TARGET).dll.a
