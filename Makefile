# Define compiler flags
CFLAGS = -Wall -O0 -mcpu=cortex-a53 --target=aarch64-elf

# Default target
all: boot.o

# Build the object file from assembly source
boot.o: boot.S
	clang $(CFLAGS) -c boot.S -o boot.o

# Clean up build artifacts
.PHONY: clean
clean:
	rm -f boot.o
