CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -fstack-protector-all -g
SRC_DIR = src
OBJ_DIR = output
RESULT_DIR = /home/stallmarch/projects/uel/CityGraph/result
PROJECT_PATH := $(shell pwd)

# Find all .c files in the source directory and subdirectories
SRC_FILES := $(shell find $(SRC_DIR) -name "*.c")
# Generate corresponding .o file names (maintaining subdirectory structure)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

# Main executable
TARGET = ted

# Create obj and result directories if they don't exist
$(shell mkdir -p $(OBJ_DIR))
$(shell mkdir -p $(RESULT_DIR))

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ -lm

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target
clean:
	rm -rf $(OBJ_DIR) $(TARGET)
	rm -rf ${RESULT_DIR}

run:
	./${TARGET} -o ${RESULT_DIR} -e "./input/t2" -f "c1.geo" -v "c1-v.via" -q "c1/01-joined.qry"

debug:
	valgrind --leak-check=full ./${TARGET} -o ${RESULT_DIR} -e "./input/t2" -f "c1.geo" -v "c1-v.via" -q "c1/01-joined.qry"

.PHONY: all clean test debug