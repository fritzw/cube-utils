MAKEFLAGS += --quiet

TARGETS = cubepro-encoder cubex-encoder
LIBS = -lm
CC = gcc
CFLAGS += -g -Wall

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

TESTFILES = $(wildcard tests/*.bfb)
TESTOUTPUTS = $(patsubst %.bfb, %.resultpro, $(TESTFILES)) $(patsubst %.bfb, %.resultx, $(TESTFILES))

.PHONY: default all clean test

all: $(TARGETS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGETS) $(OBJECTS)

cubepro-encoder: $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

cubex-encoder: cubepro-encoder
	cp $^ $@

%.resultpro: %.bfb cubepro-encoder
	./cubepro-encoder $< $(patsubst %.bfb, %.resultpro, $<)
	diff $(patsubst %.bfb, %.cubepro, $<) $(patsubst %.bfb, %.resultpro, $<)

%.resultx: %.bfb cubex-encoder
	./cubex-encoder $< $(patsubst %.bfb, %.resultx, $<)
	diff $(patsubst %.bfb, %.cubex, $<) $(patsubst %.bfb, %.resultx, $<)

test: test_clean $(TARGETS) $(TESTOUTPUTS)
	echo "All tests completed successfully"

test_clean:
	-rm -f tests/*.resultpro tests/*.resultx

clean: test_clean
	-rm -f *.o
	-rm -f $(TARGETS)
