CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS =

# Regular build
framedraw: framedraw.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

# Debug build
debug: CFLAGS += -DDEBUG
debug: framedraw

# Rule to compile .c files to .o files
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean run

# Clean up
clean:
	rm -f framedraw *.o

# Run the program
run: framedraw
	./framedraw
