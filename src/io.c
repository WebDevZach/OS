#include "./io.h"
#include "./types.h"

// Track the current cursor's row and column
volatile int cursorCol = 0;
volatile int cursorRow = 0;

// Define a keymap to convert keyboard scancodes to ASCII
char keymap[128] = {};

// C version of assembly I/O port instructions
// Allows for reading and writing with I/O
// The keyboard status port is 0x64
// The keyboard data port is 0x60
// More info here:
// https://wiki.osdev.org/I/O_Ports
// https://wiki.osdev.org/Port_IO
// https://bochs.sourceforge.io/techspec/PORTS.LST

// outb (out byte) - write an 8-bit value to an I/O port address (16-bit)
void outb(uint16 port, uint8 value)
{
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
	return;
}

// outw (out word) - write an 16-bit value to an I/O port address (16-bit)
void outw(uint16 port, uint16 value)
{
    asm volatile ("outw %1, %0" : : "dN" (port), "a" (value));
	return;
}

// inb (in byte) - read an 8-bit value from an I/O port address (16-bit)
uint8 inb(uint16 port)
{
   uint8 ret;
   asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
   return ret;
}

// inw (in word) - read an 16-bit value from an I/O port address (16-bit)
uint16 inw(uint16 port)
{
   uint16 ret;
   asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
   return ret;
}

// Setting the cursor does not display anything visually
// Setting the cursor is simply used by putchar() to find where to print next
// This can also be set independently of putchar() to print at any x, y coordinate on the screen
char* setcursor(int x, int y)
{	

    char* startingPosition = (char *)0XB8000; 

    // Sets proper row + column when column is greater than 80 if 82 it goes it next row column 2 
    while(x > 80) {
        x = x - 79;
        y++;
    }

    // Returns pointer to start if the row selected is out of bounds (0-24)rows
    if(y > 25) {
        y = 0; 
    }

    // sets new position accounts for 2 memory for letter and color 
    char* newPosition = startingPosition + x * 2;
    newPosition = newPosition + y * 80 * 2; // row is calculated by multiplying by 80 since there are 80 columns in a row 

	return newPosition;
}

// Using a pointer to video memory we can put characters to the display
// Every two addresses contain a character and a color
void putchar(char character, char* cursorPosition)
{
    *cursorPosition = character;
    cursorPosition++;
    *cursorPosition = TEXT_COLOR;

	return 0;
}

// Print the character array (string) using putchar()
// Print until we find a NULL terminator (0)
int printf(char string[]) 
{
    // Counts characters in the string
    int countChars = 0;
    int index = 0;
    while (string[index] != 0) {
        countChars++;
        index++;
    }

    for(int i = 0; i <= countChars; i++){
        putchar(string[i], setcursor(i,0));
    }
	return 0;
}

// Prints an integer to the display
// Useful for debugging
int printint(uint32 n) 
{
	int characterCount = 0;
	if (n >= 10)
	{
        characterCount = printint(n / 10);
    }
    //putchar('0' + (n % 10));
	characterCount++;

	return characterCount;
}

// Clear the screen by placing a ' ' character in every character location
void clearscreen()
{

    for(int y = 0; y < 25; y++) {
        for(int x = 0; x < 80; x++) {
            char* cursorPosition = setcursor(x,y);
            *cursorPosition = ' ';
            cursorPosition++;
            *cursorPosition = 0x00; 
        }
    }

	return;
}