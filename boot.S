	.cpu cortex-a53

	.data 

	.text
	.global _start

	_start:

		// TODO check hardware for init


		// init stack pointer 
		adr x5,_start
		// stack must be aligned to 16bytes for ARM convention
		sub x5, x5, #16
		mov sp,x5
		
		// clear bss section
		// bss sections contains all the non-init variables and global variables
		// its important to clear this section
		// __bss_start and __bss_size are linker variables:

		ldr     x5, =__bss_start   
		ldr     w6, =__bss_size    
		
	    // if w6 == 0 -> load_os else clear 
		cbz w6, load_os 

	_clear_bss_loop:
		// this code implies to have a 64 bit aligned __bss_size
		str xzr, [x5]            
		add x5, x5, #64          // put forward of 64 byte
		subs w6, w6, #64         // decrese BSS size of 64 bit
		b.ge _clear_bss_loop     // loop if >= 0

	load_os:
		bl load_os_func
		// i will never back here ( i hope )


	halt:
		// 
		wfe
		b halt

