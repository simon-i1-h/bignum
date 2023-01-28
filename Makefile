PROG = test_bignum
#OBJS = dgtvec.o bignat.o bigint.o bigrat.o main.o # TODO
OBJS = dgtvec.o bignat.o bigint.o main.o
DEPS:=$(OBJS:.o=.d)

CC = gcc
CFLAGS = -std=c2x -g -Wall -Wextra -Og

.PHONY: all test clean bear valgrind

all: test_bignum

test: all
	./test_dumper.sh
	./$(PROG)

bear: clean
	bear -- make

valgrind: all
	valgrind ./$(PROG)

test_bignum: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -c -o $@ $<

clean:
	rm -f $(PROG) $(OBJS) $(DEPS) compile_commands.json
	rm -rf tmp

-include $(DEPS)
