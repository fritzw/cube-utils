MAKEFLAGS += --quiet

TARGET = cube-encoder
LIBS = -lm
CC = gcc
CFLAGS += -g -Wall

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

TESTFILES = $(wildcard tests/*.bfb)
TESTOUTPUTS = $(patsubst %.bfb, %.result, $(TESTFILES))

.PHONY: default all clean test

all: $(TARGET)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

%.result: %.bfb $(TARGET)
	./$(TARGET) $< $(patsubst %.bfb, %.result, $<)
	diff $(patsubst %.bfb, %.cubepro, $<) $(patsubst %.bfb, %.result, $<)

test: test_clean $(TARGET) $(TESTOUTPUTS)
	echo "All tests completed successfully"

test_clean:
	-rm -f tests/*.result

clean: test_clean
	-rm -f *.o
	-rm -f $(TARGET)
