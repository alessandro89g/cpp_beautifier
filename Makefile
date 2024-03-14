# Usage:
# make        # compile all binary
# make clean  # remove ALL binaries and objects

.PHONY = all clean

PROJECT_NAME = ClassScraper
ARGS = test_files/include/class.hpp test_files/src/class.cpp

SRC_DIR = src
INC_DIR = include
TEST_DIR = tests
BUILD_DIR = build

CC = g++
CFLAGS = -Wall -g -std=c++20

GOOGLE_TEST = -lgtest -lgtest_main -lpthread

all: $(PROJECT_NAME)

run: $(PROJECT_NAME)
	@echo "Running binary.."
	./${PROJECT_NAME} ${ARGS}

$(BUILD_DIR)/Breaker.o : $(SRC_DIR)/Breaker.cpp $(INC_DIR)/Breaker.hpp $(BUILD_DIR)/utilities.o
	@echo "Creating object.."
	${CC} ${CFLAGS} -c $< -o $@

$(BUILD_DIR)/FileReader.o: $(SRC_DIR)/FileReader.cpp $(INC_DIR)/FileReader.hpp $(BUILD_DIR)/utilities.o
	@echo "Creating object.."
	${CC} ${CFLAGS} -c $< -o $@

$(BUILD_DIR)/ClassScraper.o:	$(SRC_DIR)/ClassScraper.cpp $(INC_DIR)/ClassScraper.hpp
	@echo "Creating object.."
	${CC} ${CFLAGS} -c $< -o $@

$(BUILD_DIR)/utilities.o: $(SRC_DIR)/utilities.cpp $(INC_DIR)/utilities.hpp
	@echo "Creating object.."
	${CC} ${CFLAGS} -c $< -o $@

$(BUILD_DIR)/main.o: main.cpp
	@echo "Creating object.."
	${CC} ${CFLAGS} -c $< -o $@

$(PROJECT_NAME): $(BUILD_DIR)/main.o $(BUILD_DIR)/Breaker.o $(BUILD_DIR)/FileReader.o $(BUILD_DIR)/ClassScraper.o $(BUILD_DIR)/utilities.o
	# @make $(BUILD_DIR)/Breaker.o
	# @make $(BUILD_DIR)/FileReader.o
	# @make $(BUILD_DIR)/ClassScraper.o
	# @make $(BUILD_DIR)/utilities.o
	# @make $(BUILD_DIR)/main.o
	@echo "Creating object.."
	${CC} ${CFLAGS} -o $@ $< $(BUILD_DIR)/ClassScraper.o $(BUILD_DIR)/Breaker.o $(BUILD_DIR)/FileReader.o $(BUILD_DIR)/utilities.o

$(BUILD_DIR)/test_utilities: $(TEST_DIR)/test_utilities.cpp $(BUILD_DIR)/utilities.o
	@echo "Creating executable.."
	${CC} ${CFLAGS} -o $@ $< $(BUILD_DIR)/utilities.o ${GOOGLE_TEST}

$(BUILD_DIR)/test_Breaker: $(TEST_DIR)/test_Breaker.cpp $(BUILD_DIR)/Breaker.o $(BUILD_DIR)/utilities.o
	@echo "Creating executable.."
	${CC} ${CFLAGS} -o $@ $< $(BUILD_DIR)/Breaker.o $(BUILD_DIR)/utilities.o ${GOOGLE_TEST}

$(BUILD_DIR)/test_FileReader: $(TEST_DIR)/test_FileReader.cpp $(BUILD_DIR)/FileReader.o $(BUILD_DIR)/utilities.o
	@echo "Creating executable.."
	${CC} ${CFLAGS} -o $@ $< $(BUILD_DIR)/FileReader.o $(BUILD_DIR)/utilities.o ${GOOGLE_TEST}

$(BUILD_DIR)/test_ClassScraper: $(TEST_DIR)/test_ClassScraper.cpp $(BUILD_DIR)/ClassScraper.o $(BUILD_DIR)/FileReader.o $(BUILD_DIR)/Breaker.o $(BUILD_DIR)/utilities.o
	@echo "Creating executable.."
	${CC} ${CFLAGS} -o $@ $< $(BUILD_DIR)/ClassScraper.o $(BUILD_DIR)/Breaker.o $(BUILD_DIR)/FileReader.o $(BUILD_DIR)/utilities.o ${GOOGLE_TEST}


run_test: $(BUILD_DIR)/test_utilities
	@echo "Running tests.."
	./$(BUILD_DIR)/test_utilities

clean:
	@echo "Cleaning up..."
	rm -rvf $(BUILD_DIR)/*.o ${PROJECT_NAME} $(BUILD_DIR)/test_utilities
