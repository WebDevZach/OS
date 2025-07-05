#include "./io.h"
#include "./multitasking.h"
#include "./irq.h"
#include "./isr.h"

void prockernel();
void proc_a();
void proc_b();
void proc_c();
void proc_d();
void proc_e();

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
	  createproc(proc_e, (void *) 0x50000);

	// Count how many processes are ready to run
	int userprocs = ready_process_count();

	printf("Kernel Process Starting ...\n");
	
	// As long as there is 1 user process that is ready, yield to it so it can run
	while(userprocs > 0)
	{

		// Yield to the user process
		yield();
	

		// Count the remaining ready processes (if any)
		userprocs = ready_process_count();
	}

	printf("\nKernel Process Exiting ...\n");
}

// The user processes
void proc_a()
{
	printf("A");

	exit();
}

// The user processes
void proc_b()
{
	printf("B");

	yield();

	printf("B");

	exit();
}

// The user processes
void proc_c()
{
	printf("C");

	yield();

	printf("C");

	yield();

	printf("C");

	yield();

	printf("C");

	exit();
}

// The user processes
void proc_d()
{
	printf("D");

	yield();

	printf("D");

	yield();

	printf("D");

	exit();
}

// The user processes
void proc_e()
{
	printf("E");

	yield();

	printf("E");

	exit();
}