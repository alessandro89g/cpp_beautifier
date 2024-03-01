# Usage:
# make        # compile all binary
# make clean  # remove ALL binaries and objects

.PHONY = all clean

PROJECT_NAME = cpp_beautifier

CC = g++
CFLAGS = -Wall -g -std=c++20

SRC_DIR = src
SRCS := $(wildcard ${SRC_DIR}/*.cpp)
SRCS2 := $(notdir ${SRCS})
OBJS := $(SRCS2:%.cpp=%.o)
OBJS2 := $(SRCS2:%.cpp=build/%.o)

BUILD_DIR = build

all: $(PROJECT_NAME)

run: 
	@echo "Running binary.."
	./${PROJECT_NAME} ${ARGS}

all_obj: ${OBJS2}

check:
	@echo "SRCS: ${SRCS}"
	@echo "OBJS: ${OBJS}"
	@echo "OBJS2: ${OBJS2}"

$(PROJECT_NAME): main.cpp ${OBJS2}
	@echo "Building binary.."
	${CC} ${CFLAGS} -o ${PROJECT_NAME} main.cpp ${OBJS2}

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Creating object.."
	${CC} ${CFLAGS} -c $< -o $@

clean:
	@echo "Cleaning up..."
	rm -rvf build/*.o ${PROJECT_NAME}
