.ONESHELL:
.SHELLFLAGS += -e

TARGET := libpesterchum.so

CC := g++
PACKAGES := purple

TARGET_SOURCES := $(shell find src/ -name '*.c' -o -name '*.cpp')
TEST_SOURCES := $(shell find test/ -name '*.c' -o -name '*.cpp')

TARGET_OBJECTS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(TARGET_SOURCES)))
TEST_OBJECTS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(TEST_SOURCES)))


CFLAGS := -std=c++20 -Wall -Wextra -Werror -pedantic \
          -DGLIB_DISABLE_DEPRECATION_WARNINGS
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

INCLUDES := $(shell pkg-config --cflags $(PACKAGES))
LDFLAGS := $(shell pkg-config --libs $(PACKAGES)) -shared


# Link target
$(TARGET): $(TARGET_OBJECTS)
	$(CC) $(CFLAGS) $(SHARED_CFLAGS) $(INCLUDES) $(TARGET_OBJECTS) -o $@ \
	$(LDFLAGS)

# Compile target
$(TARGET_OBJECTS): $(TARGET_SOURCES)
	$(CC) $(CFLAGS) $(SHARED_CFLAGS) $(INCLUDES) -MMD -o $@ -c $<


testrunner: $(TEST_OBJECTS) $(TARGET)
	$(CC) $(CFLAGS) $(TEST_OBJECTS) -o testrunner \
	-L. -lpesterchum -Wl,-rpath=$(shell pwd)

*.o: *.cpp
	$(CC) $(CFLAGS) -MMD -o $@ -c $<

install: $(TARGET)
	cp $(TARGET) ~/.purple/plugins/


.PHONY: clean install
clean:
	rm -f **/*.o **/*.d **/*.so **/*.so.*
	rm -f $(TARGET) testrunner

-include $(TARGET_OBJECTS:%.o=%.d)
