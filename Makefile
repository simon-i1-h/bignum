PROG = test_bignum
#OBJS = bignat.o bigint.o bigrat.o main.o
OBJS = bignat.o bigint.o main.o
DEPS:=$(OBJS:.o=.d)

CC = gcc
CFLAGS = -std=c2x -g -Wall -Wextra

.PHONY: all test clean bear

all: test_bignum

test: all
	./$(PROG)

bear: clean
	bear -- make

test_bignum: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -c -o $@ $<

clean:
	rm -f $(PROG) $(OBJS) $(DEPS) compile_commands.json

-include $(DEPS)
