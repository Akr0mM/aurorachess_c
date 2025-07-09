CC = gcc
CFLAGS = -g -Wall -Wextra -fsanitize=address -O0
TARGET = aurorachess
TEST_TARGET = test_exec
TEST_SRC = test/test.c
TEST_OBJS = aurora.c board.c movegen.c position.c

all: $(TARGET)

$(TARGET): *.c
	$(CC) $(CFLAGS) $^ -o $@

test: $(TEST_SRC) $(TEST_OBJS)
	$(CC) $(CFLAGS) $^ -o $(TEST_TARGET)
	./test_exec

clean:
	rm -f $(TARGET) $(TEST_TARGET)

.PHONY: all clean test runtest

