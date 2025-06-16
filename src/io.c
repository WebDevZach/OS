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
void setcursor(int x, int y)
{	
    cursorCol = x;
    cursorRow = y;
}

 // Converts row + column position to a memory address pointer (used for finding the corresponding video mem address of the column and row inputted)
char* convertRowColumnToMemAddress(int x, int y) {
    
    // sets new position accounts for 2 memory for letter and color 
    char* newPosition = (char*) VIDEO_MEM + x * 2;
    newPosition = newPosition + y * 80 * 2; // row is calculated by multiplying by 80 since there are 80 columns in a row 
    
    return newPosition;
}

// Using a pointer to video memory we can put characters to the display
// Every two addresses contain a character and a color
char putchar(char character)
{

     // Adds one to cursorRow if cursorCol is out of bound i.e greater than 79 (prints on new line)
    while(cursorCol > 79) {
        cursorCol = cursorCol - 80;
        cursorRow++;
    } 
    
    // Checks for new line character adds one to cursorRow and resets cursorCol to 0 if new line character is present 
    if(character == '\n') {
        cursorRow++;
        cursorCol = 0;
        return 0;
    }

    // Returns cursor row + column to (0,0) if cursorRow is out of bounds 
    if(cursorRow > 24) {
       clearscreen();
    }

    // converts row + column to a memory address to print to screen
    char* newPosition = convertRowColumnToMemAddress(cursorCol, cursorRow);

    // puts character and character's color in the right mem address 
    *newPosition = character;
    newPosition++;
    *newPosition = TEXT_COLOR;

    cursorCol++; // moves col over one after printing a character
	return 0;
}

// Print the character array (string) using putchar()
// Print until we find a NULL terminator (0)
int printf(char string[]) 
{
    int index = 0;

    // Prints each character until null terminator is found 
    while (string[index] != 0) {
        putchar(string[index]);
        index++;
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
    putchar('0' + (n % 10));
	characterCount++;

	return characterCount;
}

// Clear the screen by placing a ' ' character in every character location
void clearscreen()
{

    // Goes through each position on the screen and prints a blank character
    for(int y = 0; y < 25; y++) {
        for(int x = 0; x < 80; x++) {
            char* newPosistion = convertRowColumnToMemAddress(x,y); 
            *newPosistion = ' ';
        }
    }

    setcursor(0,0);

	return;
}

// Converts scan code to ascii 
void initkeymap() {
    keymap[0x1E] = 'a';
    keymap[0x30] = 'b';
    keymap[0x2e] = 'c';
    keymap[0x20] = 'd';
    keymap[0x12] = 'e';
    keymap[0x21] = 'f';
    keymap[0x22] = 'g';
    keymap[0x23] = 'h';
    keymap[0x17] = 'i';
    keymap[0x24] = 'j';
    keymap[0x25] = 'k';
    keymap[0x26] = 'l';
    keymap[0x32] = 'm';
    keymap[0x31] = 'n';
    keymap[0x18] = 'o';
    keymap[0x19] = 'p';
    keymap[0x10] = 'q';
    keymap[0x13] = 'r';
    keymap[0x1f] = 's';
    keymap[0x14] = 't';
    keymap[0x16] = 'u';
    keymap[0x2f] = 'v';
    keymap[0x11] = 'w';
    keymap[0x2d] = 'x';
    keymap[0x15] = 'y';
    keymap[0x2c] = 'z';
    keymap[0x0b] = '0';
    keymap[0x02] = '1';
    keymap[0x03] = '2';
    keymap[0x04] = '3';
    keymap[0x05] = '4';
    keymap[0x06] = '5';
    keymap[0x07] = '6';
    keymap[0x08] = '7';
    keymap[0x09] = '8';
    keymap[0x0a] = '9';
    keymap[0x1c] = '\n';
    keymap[0x39] = ' ';

}

char getchar() {

while((inb(0x64) & 0x01) == 0) {

}

uint8 scanCode;

// reads port x60 until a pressed scan code (bit 7 = 0)
do {
scanCode = inb(0x60); 

} while((scanCode & 0x80) == 0x80);

// Converts scan code to ascii 
return (char)keymap[scanCode];

}

void scanf(char address[]) {   

// Reads in max 100 chars 
for(int i = 0; i < 100; i++) {
char scannedChar = getchar(); 

// base case -> enter inputted returns + adds null terminator to the end
if(scannedChar == '\n') {
    address[i] = '\0';
    return;
}

address[i] = scannedChar; // put ascii char in user's array 
}

return;
}

void banner() {
    uint8 textColor = 0x4A;
    char* vidMem = (char*) 0xB8000;

    int stringLength = 0;
    char userString[] = "Welcome to my OS";

    for(int i = 0; i < 80; i++){
        *vidMem = ' ';
        vidMem++;
        *vidMem = 0x10;
        vidMem++;
    }
    
    int index = 0;
    while(userString[index] != '\0') {
        stringLength++;
        index++;
    }

    int centeredString = (80 - stringLength)/2;

    char* memPointer = VIDEO_MEM + centeredString * 2; 

    for(int i = 0; i < stringLength; i++){
        *memPointer = userString[i];
        memPointer++;
        *memPointer = textColor;
        memPointer++;
    }
     
}


