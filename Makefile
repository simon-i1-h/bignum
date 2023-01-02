PROG = test_bignum
#OBJS = bignat.o bigint.o bigrat.o main.o
OBJS = bignat.o main.o
DEPS:=$(OBJS:.o=.d)

CC = gcc
CFLAGS = -std=c2x -g -Wall -Wextra

.PHONY: all test clean

all: test_bignum

test: all
	./$(PROG)

test_bignum: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -c -o $@ $<

clean:
	rm -f $(PROG) $(OBJS)

-include $(DEPS)
