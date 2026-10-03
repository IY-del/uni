CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c99 -O2

TESTS = test_display test_load test_coord test_iterator test_view
TARGET = main

.PHONY: all test clean run

all: $(TARGET) $(TESTS)

$(TARGET): main.c fontlib.c fontlib.h
	$(CC) $(CFLAGS) main.c fontlib.c -o $@

test_display: test_display.c fontlib.c fontlib.h
	$(CC) $(CFLAGS) test_display.c fontlib.c -o $@

test_load: test_load.c fontlib.c fontlib.h
	$(CC) $(CFLAGS) test_load.c -o $@

test_coord: test_coord.c fontlib.c fontlib.h
	$(CC) $(CFLAGS) test_coord.c -o $@

test_iterator: test_iterator.c fontlib.c fontlib.h
	$(CC) $(CFLAGS) test_iterator.c fontlib.c -o $@

test_view: test_view.c fontlib.c fontlib.h
	$(CC) $(CFLAGS) test_view.c fontlib.c -o $@

test: $(TESTS)
	@echo "=== Running all tests ==="
	./test_display
	./test_load
	./test_coord
	./test_iterator
	./test_view
	@echo "=== All tests passed successfully ==="

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(TESTS)
