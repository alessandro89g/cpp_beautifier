# Usage:
# make        # compile all binary
# make clean  # remove ALL binaries and objects

.PHONY = all clean

PROJECT_NAME = cpp_beautifier

CC = g++
CFLAGS = -Wall -g -std=c++20

SRC_DIR = src
INCLUDE_DIR	= include
TEST_DIR = tests
SRCS := $(wildcard ${SRC_DIR}/*.cpp)
SRCS2 := $(notdir ${SRCS})
OBJS := $(SRCS2:%.cpp=%.o)
OBJS2 := $(SRCS2:%.cpp=build/%.o)

LIBS_TEST = -lgtest -lgtest_main -lpthread

BUILD_DIR = build

all: $(PROJECT_NAME)

build:
	@echo "Creating build directory.."
	mkdir -p ${BUILD_DIR}

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

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | build
	@echo "Creating object.."
	${CC} ${CFLAGS} -c $< -o $@

# Tests from folder tests
all_tests: $(TEST_DIR)/test_is_something

$(TEST_DIR)/test_is_something: $(TEST_DIR)/test_is_something.cpp $(BUILD_DIR)/Beautifier.o
	@echo "Building test binary.."
	${CC} ${CFLAGS} -o $@ $< build/Beautifier.o ${LIBS_TEST}

Breaker.o : Breaker.cpp
	@echo "Creating object.."
	${CC} ${CFLAGS} -g -c $< -o $@

FileReader.o:	FileReader.cpp
	@echo "Creating object.."
	${CC} ${CFLAGS} -g -c $< -o $@

ClassScraper.o:	ClassScraper.cpp Breaker.o FileReader.o
	@echo "Creating object.."
	${CC} ${CFLAGS} -g -c $< -o $@

ClassScraper:	main_2.cpp ClassScraper.o
	@echo "Creating object.."
	${CC} ${CFLAGS} -g -c $< -o $@

clean:
	@echo "Cleaning up..."
	rm -rvf build/*.o ${PROJECT_NAME}
