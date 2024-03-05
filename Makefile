# Usage:
# make        # compile all binary
# make clean  # remove ALL binaries and objects

.PHONY = all clean

PROJECT_NAME = ClassScraper
ARGS = test_files/include/class.h test_files/src/class.cpp

CC = g++
CFLAGS = -Wall -g -std=c++20

GOOGLE_TEST = -lgtest -lgtest_main -lpthread

all: $(PROJECT_NAME)

run: 
	@echo "Running binary.."
	./${PROJECT_NAME} ${ARGS}

Breaker.o : Breaker.cpp
	@echo "Creating object.."
	${CC} ${CFLAGS} -g -c $< -o $@

FileReader.o:	FileReader.cpp
	@echo "Creating object.."
	${CC} ${CFLAGS} -g -c $< -o $@

ClassScraper.o:	ClassScraper.cpp Breaker.o FileReader.o
	@echo "Creating object.."
	${CC} ${CFLAGS} -g -c $< -o $@

$(PROJECT_NAME): main_2.cpp ClassScraper.o
	@echo "Creating object.."
	${CC} ${CFLAGS} -g -c $< -o $@

clean:
	@echo "Cleaning up..."
	rm -rvf src/*.o ${PROJECT_NAME}
