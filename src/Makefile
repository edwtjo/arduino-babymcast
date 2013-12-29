UNO_CFLAGS := -mmcu=atmega328p -DF_CPU=16000000L -s -Os
UNO_AVRDUDE := -c arduino -p m328p -b 115200 -P/dev/ttyACM0

CC=avr-gcc
CXX=avr-g++
AR=avr-ar
LD=avr-ld
OBJCOPY=avr-objcopy

DEFS := -D__AVR_ATmega328P__ -DARDUINO=155
SRC := babymcast.cpp
OBJ := $(SRC:.cpp=.o)
INC := $(subst ..,-I..,$(sort $(dir $(wildcard ../lib/*/))))
LIB := $(subst ..,-L..,$(sort $(dir $(wildcard ../lib/*/))))
LDLIBS := -lethernet -lspi -larduino -lm

CFLAGS_ELF_OUTPUT=$(UNO_CFLAGS) -Wall -Os $(LIB)
CFLAGS=-c $(UNO_CFLAGS) $(DEFS) -Wall -Os $(INC)

AVRDUDE_VERBOSITY=
AVRDUDE_CONF=../etc/avrdude.conf
STRIP=avr-strip

.SUFFIXES: .cpp .c .o

.c.o:
	$(CC) $(CFLAGS) $(INC) $(LIB) -c $< -o $@

.cpp.o:
	$(CXX) $(CFLAGS) $(INC) $(LIB) -c $< -o $@

all: babymcast.hex

babymcast.hex: babymcast.elf
	@cp babymcast.elf babymcast_unstripped.elf
	$(STRIP) -s babymcast.elf
	$(OBJCOPY) -O ihex -R .eeprom babymcast.elf babymcast.hex

babymcast.elf: $(OBJ)
	$(CC) $(UNO_CFLAGS) $(LIB) babymcast.o -o babymcast.elf $(LDLIBS)

upload: babymcast.hex
	sudo avrdude $(AVRDUDE_VERBOSITY) -C $(AVRDUDE_CONF) -q -q $(UNO_AVRDUDE) -D -Uflash:w:babymcast.hex:i

clean:
	rm -f *.o
	rm -f *.elf
	rm -f *.hex
	rm -f *~
