CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude -O3

TARGET = tests/test_allocator
SRC = src/my_alloc.c tests/test_allocator.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean