.ONESHELL:
.SHELLFLAGS += -e

TARGET := libpesterchum.so
CC := gcc
PACKAGES := glib-2.0

TARGET_SOURCES := $(shell find src/ -name '*.c')
TEST_SOURCES := $(shell find test/ -name '*.c')
PURPLE_SOURCES := $(shell find lib/libpurple-mini -name '*.c')
MUNIT_SOURCES := $(shell find lib/munit -name '*.c')

TARGET_OBJECTS :=  $(TARGET_SOURCES:%.c=%.o)
TEST_OBJECTS := $(TEST_SOURCES:%.c=%.o)

CFLAGS := -Wall -Wextra -Werror -pedantic -std=c99 \
          -DGLIB_DISABLE_DEPRECATION_WARNINGS \
          -fPIC -shared -fvisibility=hidden
ifdef DEBUG
	CFLAGS += -g -O0
else
	ifdef ASAN
		CFLAGS += -g -O0 -fsanitize=address
	else
		CFLAGS += -O3
	endif
endif

INCLUDES := $(shell pkg-config --cflags $(PACKAGES))
LDFLAGS := $(shell pkg-config --libs $(PACKAGES))


# Link target
$(TARGET): $(TARGET_OBJECTS) $(PURPLE_OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDES) $(TARGET_OBJECTS) $(PURPLE_OBJECTS) -o $@

# Compile target
$(TARGET_OBJECTS): purple
$(TARGET_OBJECTS): $(TARGET_SOURCES)
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -o $@ -c $<


testrunner: $(TEST_OBJECTS) $(TARGET_OBJECTS) $(PURPLE_OBJECTS) $(MUNIT_OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDES) $(TEST_OBJECTS) $(TARGET_OBJECTS) $(PURPLE_OBJECTS) $(MUNIT_OBJECTS) -o $@

$(TEST_OBJECTS): munit
$(TEST_OBJECTS): $(TEST_SOURCES)
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -o $@ -c $<


# Submodules
purple:
	cd ./lib/libpurple-mini
	make CFLAGS+=-DGLIB_DISABLE_DEPRECATION_WARNINGS
	cd ../..
purple: PURPLE_OBJECTS := $(shell find lib/libpurple-mini -name '*.o')

munit:
	cd ./lib/munit
	make
	cd ../..
munit: MUNIT_OBJECTS := $(shell find lib/munit -name '*.o')


.PHONY: clean
clean: D_FILES := $(TARGET_SOURCES:%.c=%.d) $(TEST_SOURCES:%.c=%.d)
clean:
	rm -f $(TARGET_OBJECTS)
	rm -f $(TARGET)
	rm -f $(D_FILES)
	cd ./lib/libpurple-mini
	make clean
	cd ../munit
	make clean
	cd ../..
