CC=avr-gcc
CXX=avr-g++
AR=avr-ar
LD=avr-ld

INC_DIRS := -I. -I../Arduino
CFLAGS  := -Wall -Os -mmcu=atmega328p -DF_CPU=16000000L $(INC_DIRS)

C_SRC=$(wildcard *.c)
C_OBJ=$(C_SRC:.c=.o)

CXX_SRC=$(wildcard *.cpp)
CXX_OBJ=$(CXX_SRC:.cpp=.o)

_= $(eval) $(eval)

HEADERS=$(wildcard *.h)
SOURCES=$(C_SRC) $(CXX_SRC)
OBJECTS=$(C_OBJ) $(CXX_OBJ)

.SUFFIXES: .c .h .cpp .o

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.cpp.o:
	$(CXX) $(CFLAGS) -c $< -o $@

all: spi

spi: $(OBJECTS) $(SOURCES) $(HEADERS)
	$(AR) rcs libspi.a $(OBJECTS)

cloc:
	@cloc ./*

clean:
	@rm -rfv *.o
	@rm -rfv *.a
