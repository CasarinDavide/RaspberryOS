

.global read32reg
.global read64reg
.global write64reg
.global write32reg
.global delay

read32reg:
		ldr w0,[x1]
		RET


read64reg:
	ldr x0,[x1]
	RET


write64reg:
	str x2,[x1]
	RET


write32reg:
	str w2,[x1]
	RET


delay:
    MOV x1, x1   // Number of iterations (change as needed)

delay_loop:
    SUBS x1, x1, #1    // Decrement the counter
    BNE delay_loop     // Repeat until R1 is zero
    RET              // Return from the function












