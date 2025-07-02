CC = gcc
CFLAGS = -g -Wall -Wextra -fsanitize=address -O0
TARGET = aurorachess

all: $(TARGET)

$(TARGET): *.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(TARGET)

.PHONY: all clean
