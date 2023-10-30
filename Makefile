.ONESHELL:
.SHELLFLAGS += -e

TARGET := libpesterchum.so

CC := gcc
PACKAGES := glib-2.0

# Submodules
PURPLE_DIR := ./lib/libpurple-mini
PURPLE := $(PURPLE_DIR)/libpurple.so
MUNIT_DIR := ./lib/munit
MUNIT := $(MUNIT_DIR)/munit.o

TARGET_SOURCES := $(shell find src/ -name '*.c')
TEST_SOURCES := $(shell find test/ -name '*.c')

TARGET_OBJECTS := $(TARGET_SOURCES:%.c=%.o)
TEST_OBJECTS := $(TEST_SOURCES:%.c=%.o)

CFLAGS := -Wall -Wextra -Werror -pedantic -std=c99 -fPIC \
          -DGLIB_DISABLE_DEPRECATION_WARNINGS
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
	$(CC) $(CFLAGS) $(INCLUDES) $(TARGET_OBJECTS) -o $@ \
	$(LDFLAGS) -Wl,-rpath=$(shell realpath $(PURPLE_DIR))

# Compile target
$(TARGET_OBJECTS): $(TARGET_SOURCES)
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -o $@ -c $<


testrunner: $(TEST_OBJECTS) $(MUNIT) $(TARGET)
	$(CC) $(CFLAGS) $(TEST_OBJECTS) $(MUNIT) -o $@ \
	-L. -lpesterchum

$(TEST_OBJECTS): $(TEST_SOURCES)
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -o $@ -c $<


# Submodules
$(PURPLE):
	cd $(PURPLE_DIR)
	make libpurple.so CFLAGS+=-DGLIB_DISABLE_DEPRECATION_WARNINGS SSL=n
	ln -s libpurple.so libpurple.so.0
	cd ../..

$(MUNIT):
	cd $(MUNIT_DIR)
	make munit.o CC=$(CC) CFLAGS="$(CFLAGS)"
	cd ../..


.PHONY: clean
clean:
	rm -f **/*.o **/*.d **/*.so **/*.so.* **/*.a **/*.dll **/*.exe **/*.de
	rm -f $(TARGET)

-include $(TARGET_OBJECTS:%.o=%.d)
