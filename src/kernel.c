#include "./io.h"

int main() 
{
	// Clear the screen
	clearscreen();

	// initialize key map
	//initkeymap();

	// Max user string 100 chars
	char userString[100] = {};

	/*Loops menu infinitely 
	while(1==1)
	{
		printf("Please type to the screen: "); // Prompt user
		scanf(userString); // Populates user array with typed chars from keyboard
		printf(userString); // Prints user array 
		printf("\n"); // Goes to next line 
	}
	*/

	banner();

	return 0;
}