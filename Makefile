CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -fstack-protector-all -g
SRC_DIR = src
OBJ_DIR = output
RESULT_DIR = $(PROJECT_PATH)/result
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

c1:
	./${TARGET} -o ${RESULT_DIR} -e "./input/t2" -f "c1.geo" -v "c1-v.via" -q "c1/00-perc-smpl.qry"
	./${TARGET} -o ${RESULT_DIR} -e "./input/t2" -f "c1.geo" -v "c1-v.via" -q "c1/01-joined.qry"
	./${TARGET} -o ${RESULT_DIR} -e "./input/t2" -f "c1.geo" -v "c1-v.via" -q "c1/02-alag-central-perc-smpl.qry"
	./${TARGET} -o ${RESULT_DIR} -e "./input/t2" -f "c1.geo" -v "c1-v.via" -q "c1/03-alag-central-dren-perc-smpl.qry"
	./${TARGET} -o ${RESULT_DIR} -e "./input/t2" -f "c1.geo" -v "c1-v.via" -q "c1/04-alag-esp-perc-smpl.qry"
	./${TARGET} -o ${RESULT_DIR} -e "./input/t2" -f "c1.geo" -v "c1-v.via" -q "c1/05-alag-l-perc-smpl.qry"
	./${TARGET} -o ${RESULT_DIR} -e "./input/t2" -f "c1.geo" -v "c1-v.via" -q "c1/06-perc-estr3.qry"
	./${TARGET} -o ${RESULT_DIR} -e "./input/t2" -f "c1.geo" -v "c1-v.via" -q "c1/07-perc-estr3-showall.qry"
	./${TARGET} -o ${RESULT_DIR} -e "./input/t2" -f "c1.geo" -v "c1-v.via" -q "c1/08-perc-estr3-joinall.qry"
	./${TARGET} -o ${RESULT_DIR} -e "./input/t2" -f "c1.geo" -v "c1-v.via" -q "c1/09-perc-varios-joinall.qry"
	./${TARGET} -o ${RESULT_DIR} -e "./input/t2" -f "c1.geo" -v "c1-v.via" -q "c1/10-perc-smpl.qry"

c2: 
	./${TARGET} -o ${RESULT_DIR} -e "./input/t2" -f "c2.geo" -v "c2-v.via" -q "c2/00-perc-smpl.qry"
	./${TARGET} -o ${RESULT_DIR} -e "./input/t2" -f "c2.geo" -v "c2-v.via" -q "c2/01-joined.qry"
	./${TARGET} -o ${RESULT_DIR} -e "./input/t2" -f "c2.geo" -v "c2-v.via" -q "c2/02-alag-central-perc-smpl.qry"
	./${TARGET} -o ${RESULT_DIR} -e "./input/t2" -f "c2.geo" -v "c2-v.via" -q "c2/03-alag-central-dren-perc-smpl.qry"
	./${TARGET} -o ${RESULT_DIR} -e "./input/t2" -f "c2.geo" -v "c2-v.via" -q "c2/04-alag-esp-perc-smpl.qry"
	./${TARGET} -o ${RESULT_DIR} -e "./input/t2" -f "c2.geo" -v "c2-v.via" -q "c2/05-alag-l-perc-smpl.qry"
	./${TARGET} -o ${RESULT_DIR} -e "./input/t2" -f "c2.geo" -v "c2-v.via" -q "c2/06-perc-estr3.qry"
	./${TARGET} -o ${RESULT_DIR} -e "./input/t2" -f "c2.geo" -v "c2-v.via" -q "c2/07-perc-estr3-showall.qry"
	./${TARGET} -o ${RESULT_DIR} -e "./input/t2" -f "c2.geo" -v "c2-v.via" -q "c2/08-perc-estr3-joinall.qry"
	./${TARGET} -o ${RESULT_DIR} -e "./input/t2" -f "c2.geo" -v "c2-v.via" -q "c2/09-perc-varios-joinall.qry"
	./${TARGET} -o ${RESULT_DIR} -e "./input/t2" -f "c2.geo" -v "c2-v.via" -q "c2/10-perc-smpl.qry" 

debug:
	valgrind --leak-check=full ./${TARGET} -o ${RESULT_DIR} -e "./input/t2" -f "c1.geo" -v "c1-v.via" -q "c1/00-perc-smpl.qry"

.PHONY: all clean test debug