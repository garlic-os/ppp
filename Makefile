.ONESHELL:
.SHELLFLAGS += -e

SOURCES = libpesterchum.c
TARGET = libpesterchum

CC = gcc

CFLAGS = -Wall -Wextra -Werror -pedantic -std=c99 -O3 -fvisibility=hidden
CFLAGS += -DGLIB_DISABLE_DEPRECATION_WARNINGS  # it's pidgin's fault

INCLUDES = -I/usr/include/glib-2.0

.PHONY: clean configure

all: linux windows

linux: $(SOURCES)
	$(CC) $(CFLAGS) $(INCLUDES) $(SOURCES) -o $(TARGET).so -shared -fPIC

windows: $(SOURCES)
	$(CC) $(CFLAGS) $(INCLUDES) $(SOURCES) -o $(TARGET).dll -shared -Wl,--out-implib,$(TARGET).dll.a

clean:
	rm -f $(TARGET).so $(TARGET).dll $(TARGET).dll.a

configure:
	cd ./libpurple-mini
	./update-version
	# Keep git from saying you modified the submodule
	git config submodule.libpurple-mini.ignore all
