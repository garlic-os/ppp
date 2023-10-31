.ONESHELL:
.SHELLFLAGS += -e

TARGET := libpesterchum.so

CC := g++
PACKAGES := glib-2.0

# Submodules
PURPLE_DIR := ./lib/libpurple-mini
PURPLE := $(PURPLE_DIR)/libpurple.so

TARGET_SOURCES := $(shell find src/ -name '*.cpp')
TEST_SOURCES := $(shell find test/ -name '*.cpp')

TARGET_OBJECTS := $(TARGET_SOURCES:%.cpp=%.o)
TEST_OBJECTS := $(TEST_SOURCES:%.cpp=%.o)

CFLAGS := -std=c++20 -Wall -Wextra -Werror -pedantic \
          -DGLIB_DISABLE_DEPRECATION_WARNINGS
SHARED_CFLAGS := -fPIC
ifdef DEBUG
	CFLAGS += -ggdb3 -O0
else
	ifdef ASAN
		CFLAGS += -ggdb3 -O0 -fsanitize=address
	else
		CFLAGS += -O3
	endif
endif

INCLUDES := $(shell pkg-config --cflags $(PACKAGES))
LDFLAGS := $(shell pkg-config --libs $(PACKAGES)) \
           -shared -L$(PURPLE_DIR) -lpurple


# Link target
$(TARGET): $(TARGET_OBJECTS) $(PURPLE)
	$(CC) $(CFLAGS) $(SHARED_CFLAGS) $(INCLUDES) $(TARGET_OBJECTS) -o $@ \
	$(LDFLAGS) -Wl,-rpath=$(shell realpath $(PURPLE_DIR))

# Compile target
$(TARGET_OBJECTS): $(TARGET_SOURCES)
	$(CC) $(CFLAGS) $(SHARED_CFLAGS) $(INCLUDES) -MMD -o $@ -c $<


testrunner: $(TEST_OBJECTS) $(TARGET)
	$(CC) $(CFLAGS) $(TEST_OBJECTS) -o testrunner \
	-L. -lpesterchum -Wl,-rpath=$(shell pwd)

*.o: *.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -o $@ -c $<


# Submodules
$(PURPLE):
	cd $(PURPLE_DIR)
	make libpurple.so CFLAGS+=-DGLIB_DISABLE_DEPRECATION_WARNINGS SSL=n
	ln -s libpurple.so libpurple.so.0
	cd ../..


.PHONY: clean
clean:
	rm -f **/*.o **/*.d **/*.so **/*.so.*
	rm -f $(TARGET) testrunner

-include $(TARGET_OBJECTS:%.o=%.d)
