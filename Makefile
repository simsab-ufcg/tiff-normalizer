CC=g++
CFLAGS = -std=c++14
LIBS = -ltiff

DEPS = *.h

SRCS = $(shell find -name *.cpp)
OBJS := $(addsuffix .o,$(basename $(SRCS)))

all: run

%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

run: $(OBJS)
	$(CC) -o $@ $^ $(LIBS)

clean: 
	rm -rf $(OBJS)
	rm -f run