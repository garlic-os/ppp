.ONESHELL:
.SHELLFLAGS += -e

TARGET := libpesterchum.so

CC := g++
PACKAGES := purple

TARGET_SOURCES := $(shell find src/ -name '*.c' -o -name '*.cpp')
TEST_SOURCES := $(shell find test/ -name '*.c' -o -name '*.cpp')

TARGET_OBJECTS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(TARGET_SOURCES)))
TEST_OBJECTS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(TEST_SOURCES)))


CFLAGS := -std=c++20 -Wall -Wextra -Werror -pedantic -MMD \
          -DGLIB_DISABLE_DEPRECATION_WARNINGS \
          $(shell pkg-config --cflags $(PACKAGES) | sed 's/-I/-isystem /g')
SHARED_CFLAGS := -fPIC
ifdef DEBUG
	CFLAGS += -ggdb3 -O0
else
	ifdef ASAN
		CFLAGS += -ggdb3 -O0
		LDFLAGS += -fsanitize=address -static-libasan
	else
		CFLAGS += -O3
	endif
endif

LDFLAGS := $(shell pkg-config --libs $(PACKAGES)) -shared


$(TARGET): $(TARGET_OBJECTS)
	$(CC) $(CFLAGS) $(SHARED_CFLAGS) $(TARGET_OBJECTS) -o $@ $(LDFLAGS)

pesterchum.o: pesterchum.c
	$(CC) $(CFLAGS) $(SHARED_CFLAGS) -o $@ -c $<


testrunner: $(TEST_OBJECTS) $(TARGET)
	$(CC) $(CFLAGS) $(TEST_OBJECTS) -o $@ \
	-L. -lpesterchum -Wl,-rpath=$(shell pwd)


.PHONY: clean install
clean:
	rm -f **/*.o **/*.d **/*.so **/*.so.*
	rm -f $(TARGET) testrunner

install: $(TARGET)
	cp $(TARGET) ~/.purple/plugins/


-include $(TARGET_OBJECTS:%.o=%.d)
