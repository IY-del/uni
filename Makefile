CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c99 -O2

TESTS = test_fontlib test_get_pattern test_get_coord test_iterator test_print_pattern
TARGET = main

.PHONY: all test clean run

all: $(TARGET) $(TESTS)

$(TARGET): main.c fontlib.c fontlib.h
	$(CC) $(CFLAGS) main.c fontlib.c -o $@

test_fontlib: test_fontlib.c fontlib.c fontlib.h
	$(CC) $(CFLAGS) test_fontlib.c fontlib.c -o $@

test_get_pattern: test_get_pattern.c fontlib.c fontlib.h
	$(CC) $(CFLAGS) test_get_pattern.c -o $@

test_get_coord: test_get_coord.c fontlib.c fontlib.h
	$(CC) $(CFLAGS) test_get_coord.c -o $@

test_iterator: test_iterator.c fontlib.c fontlib.h
	$(CC) $(CFLAGS) test_iterator.c fontlib.c -o $@

test_print_pattern: test_print_pattern.c fontlib.c fontlib.h
	$(CC) $(CFLAGS) test_print_pattern.c fontlib.c -o $@

test: $(TESTS)
	@echo "=== Running all tests ==="
	./test_fontlib
	./test_get_pattern
	./test_get_coord
	./test_iterator
	./test_print_pattern
	@echo "=== All tests passed successfully ==="

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(TESTS)
