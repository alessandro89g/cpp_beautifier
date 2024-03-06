# Usage:
# make        # compile all binary
# make clean  # remove ALL binaries and objects

.PHONY = all clean

PROJECT_NAME = ClassScraper
ARGS = test_files/include/class.hpp test_files/src/class.cpp

CC = g++
CFLAGS = -Wall -g -std=c++20

GOOGLE_TEST = -lgtest -lgtest_main -lpthread

all: $(PROJECT_NAME)

run: $(PROJECT_NAME)
	@echo "Running binary.."
	./${PROJECT_NAME} ${ARGS}

src/Breaker.o : src/Breaker.cpp include/Breaker.hpp src/utilities.o
	@echo "Creating object.."
	${CC} ${CFLAGS} -c $< -o $@

src/FileReader.o: src/FileReader.cpp include/FileReader.hpp src/utilities.o
	@echo "Creating object.."
	${CC} ${CFLAGS} -c $< -o $@

src/ClassScraper.o:	src/ClassScraper.cpp include/ClassScraper.hpp
	@echo "Creating object.."
	${CC} ${CFLAGS} -c $< -o $@

src/utilities.o: src/utilities.cpp include/utilities.hpp
	@echo "Creating object.."
	${CC} ${CFLAGS} -c $< -o $@

main.o: main.cpp
	@echo "Creating object.."
	${CC} ${CFLAGS} -c $< -o $@

$(PROJECT_NAME): main.o src/Breaker.o src/FileReader.o src/ClassScraper.o src/utilities.o
	@make src/Breaker.o
	@make src/FileReader.o
	@make src/ClassScraper.o
	@echo "Creating object.."
	${CC} ${CFLAGS} -o $@ $< src/ClassScraper.o src/Breaker.o src/FileReader.o src/utilities.o

tests/test_utilities: tests/test_utilities.cpp src/utilities.o
	@echo "Creating executable.."
	${CC} ${CFLAGS} -o $@ $< src/utilities.o ${GOOGLE_TEST}

run_tests: tests/test_utilities
	@echo "Running tests.."
	./tests/test_utilities

clean:
	@echo "Cleaning up..."
	rm -rvf src/*.o ${PROJECT_NAME} ${PROJECT_NAME}.o tests/test_utilities
