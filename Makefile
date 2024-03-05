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

src/Breaker.o : src/Breaker.cpp include/Breaker.h
	@echo "Creating object.."
	${CC} ${CFLAGS} -c $< -o $@

src/FileReader.o: src/FileReader.cpp include/FileReader.h
	@echo "Creating object.."
	${CC} ${CFLAGS} -c $< -o $@

src/ClassScraper.o:	src/ClassScraper.cpp include/ClassScraper.h
	@echo "Creating object.."
	${CC} ${CFLAGS} -c $< -o $@

main.o: main.cpp
	@echo "Creating object.."
	${CC} ${CFLAGS} -c $< -o $@

$(PROJECT_NAME): main.o
	@make src/Breaker.o
	@make src/FileReader.o
	@make src/ClassScraper.o
	@echo "Creating object.."
	${CC} ${CFLAGS} -o $@ $< src/ClassScraper.o src/Breaker.o src/FileReader.o

clean:
	@echo "Cleaning up..."
	rm -rvf src/*.o ${PROJECT_NAME} ${PROJECT_NAME}.o
