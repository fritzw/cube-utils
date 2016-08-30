MAKEFLAGS += --quiet

TARGETS = cubepro-encoder cubex-encoder cube-decoder
LIBS = -lm
CC = gcc
CFLAGS += -g -Wall

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

TESTFILES = $(wildcard tests/*.bfb)
CUBEPROTESTS = $(patsubst %.bfb, %.resultpro, $(TESTFILES)) $(patsubst %.bfb, %.result.decodepro, $(TESTFILES))
CUBEXTESTS = $(patsubst %.bfb, %.resultx, $(TESTFILES)) $(patsubst %.bfb, %.result.decodex, $(TESTFILES))
TESTOUTPUTS = $(CUBEPROTESTS) $(CUBEXTESTS)

.PHONY: default all clean test

all: $(TARGETS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGETS) $(OBJECTS)

cubepro-encoder: cube-encoder.o blowfish.o
	$(CC) $^ -Wall $(LIBS) -o $@

cubex-encoder: cubepro-encoder
	cp $^ $@

cube-decoder: cube-decoder.o blowfish.o
	$(CC) $^ -Wall $(LIBS) -o $@

%.resultpro: %.bfb cubepro-encoder
	./cubepro-encoder $< $(patsubst %.bfb, %.resultpro, $<)
	diff $(patsubst %.bfb, %.cubepro, $<) $(patsubst %.bfb, %.resultpro, $<)

%.resultx: %.bfb cubex-encoder
	./cubex-encoder $< $(patsubst %.bfb, %.resultx, $<)
	diff $(patsubst %.bfb, %.cubex, $<) $(patsubst %.bfb, %.resultx, $<)

%.result.decodepro: %.resultpro cube-decoder
	./cube-decoder $< $(patsubst %.resultpro, %.result.decodepro, $<)
	diff $(patsubst %.resultpro, %.bfb, $<) $(patsubst %.resultpro, %.result.decodepro, $<)

%.result.decodex: %.resultx cube-decoder
	./cube-decoder -x $< $(patsubst %.resultx, %.result.decodex, $<)
	diff $(patsubst %.resultx, %.bfb, $<) $(patsubst %.resultx, %.result.decodex, $<)

test: test_clean $(TARGETS) $(TESTOUTPUTS)
	echo "All tests completed successfully"

test_clean:
	-rm -f tests/*.result*

clean: test_clean
	-rm -f *.o
	-rm -f $(TARGETS)
