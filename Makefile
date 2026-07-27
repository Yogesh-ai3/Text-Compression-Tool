CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic

CORE_SOURCES = engine.c analysis.c adaptive.c huffman.c rle.c utils.c
APP_SOURCES = $(CORE_SOURCES) main.c
TEST_SOURCES = $(CORE_SOURCES) tests.c
BUILD_DIR = build
APP = $(BUILD_DIR)/adaptive_storage_engine
TEST = $(BUILD_DIR)/tests

.PHONY: all run test clean

all: $(APP)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(APP): $(APP_SOURCES) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(APP_SOURCES) -o $(APP)

$(TEST): $(TEST_SOURCES) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(TEST_SOURCES) -o $(TEST)

run: $(APP)
	$(APP)

test: $(TEST)
	$(TEST)

clean:
	rm -rf $(BUILD_DIR)
