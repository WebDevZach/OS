#include "./io.h"
#include "./multitasking.h"
#include "./irq.h"
#include "./isr.h"

void prockernel();
void proc_a();
void proc_b();
void proc_c();
void proc_d();

int main() 
{
	// Clear the screen
	clearscreen();

	// Initialize our keyboard
	initkeymap();

	// Initialize interrupts
	idt_install();
    isrs_install();
    irq_install();

	// Start executing the kernel process
	startkernel(prockernel);
	
	return 0;
}

void prockernel()
{
	// Create the user processes
	createproc(proc_a, (void *) 0x10000);
	createproc(proc_b, (void *) 0x20000);
	createproc(proc_c, (void *) 0x30000);
	createproc(proc_d, (void *) 0x40000);

	// Count how many processes are ready to run
	int userprocs = ready_process_count();

	printf("Kernel Process Started\n");
	
	// As long as there is 1 user process that is ready, yield to it so it can run
	while(userprocs > 0)
	{
		// Yield to the user process
		yield();
		
		printf("Kernel Process Resumed\n");

		// Count the remaining ready processes (if any)
		userprocs = ready_process_count();
	}

	printf("Kernel Process Terminated\n");
}

// The user processes
void proc_a()
{
	printf("User Process A Started\n");

	exit();
}

// The user processes
void proc_b()
{
	printf("User Process B Started\n");

	yield();

	printf("User Process B Resumed\n");

	exit();
}

// The user processes
void proc_c()
{
	printf("User Process C Started\n");

	yield();

	printf("User Process C Resumed\n");

	yield();

	printf("User Process C Resumed\n");

	exit();
}

// The user processes
void proc_d()
{
	printf("User Process D Started\n");

	yield();

	printf("User Process D Resumed\n");

	yield();

	printf("User Process D Resumed\n");

	yield();

	printf("User Process D Resumed\n");

	exit();
}