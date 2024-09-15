# Define compiler flags
CFLAGS = -Wall -O0 -g -mcpu=cortex-a53 --target=aarch64-elf

# Default target
all: kernel.img

# Build the object file from assembly source
memory_managment.o: memory_managment.c memory_managment.h
	clang $(CFLAGS) -c memory_managment.c -o memory_managment.o


# Build the object file from assembly source
boot.o: boot.S memory_managment.h
	clang $(CFLAGS) -c boot.S -o boot.o

# Build kernel object file from C source
kernel.o: kernel.c types.h gpio.h mini_uart.h utilities_func.h
	clang $(CFLAGS) -c kernel.c -o kernel.o

# Build the types object file
types.o: types.h types.c
	clang $(CFLAGS) -c types.c -o types.o

# Build the utilities_func object file (for C file)
utilities_func.o: utilities_func.c utilities_func.h
	clang $(CFLAGS) -c utilities_func.c -o utilities_func.o

# Build the utilities_func object file (for assembly file)
utilities_func_asm.o: utilities_func.S
	clang $(CFLAGS) -c utilities_func.S -o utilities_func_asm.o

# Build GPIO object file from C source
gpio.o: gpio.c gpio.h
	clang $(CFLAGS) -c gpio.c -o gpio.o

# Build UART object file from C source
mini_uart.o: mini_uart.c mini_uart.h gpio.h
	clang $(CFLAGS) -c mini_uart.c -o mini_uart.o

# Link the object files into the final kernel image
kernel.img: kernel.o boot.o gpio.o mini_uart.o types.o utilities_func.o utilities_func_asm.o memory_managment.o
	clang $(CFLAGS) -fuse-ld=lld -T linker.ld -nostdlib kernel.o boot.o gpio.o mini_uart.o types.o utilities_func.o utilities_func_asm.o memory_managment.o -o kernel.img

# Clean up build artifacts
.PHONY: clean
clean:
	rm -f boot.o kernel.o gpio.o mini_uart.o types.o utilities_func.o utilities_func_asm.o kernel.img memory_managment.o
