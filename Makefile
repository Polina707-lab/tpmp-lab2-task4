CC := gcc
CFLAGS := -Wall -Wextra -Wpedantic -std=c11 -Iinclude

BUILD_DIR := build
BIN_DIR := bin
TARGET := $(BIN_DIR)/task4

SRCS := \
  src/main.c \
  src/date_service.c \
  src/worker_service.c \
  src/company_service.c \
  src/file_input.c

OBJS := $(SRCS:src/%.c=$(BUILD_DIR)/%.o)

.PHONY: all clean dirs run

all: dirs $(TARGET)

dirs:
	mkdir -p $(BUILD_DIR) $(BIN_DIR)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	$(TARGET) data/input.txt data/output.txt
	cat data/output.txt

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
