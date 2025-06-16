#include "./types.h"

// Define our constants that will be widely used
#define TEXT_COLOR 0x0A
#define VIDEO_MEM 0xB8000
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

// Define our function prototypes
void outb(uint16 port, uint8 value);
void outw(uint16 port, uint16 value);
uint8  inb(uint16 port);
uint16 inw(uint16 port);

void initkeymap();
char getchar();
void scanf(char address[]);

char* convertRowColumnToMemAddress(int x, int y);
void setcursor(int x, int y);
char putchar(char character);
int printf(char string[]);
int printint(uint32 n);
void clearscreen();
void banner();

/*
0	Black	
1	Blue	
2	Green	
3	Cyan	
4	Red	
5	Purple	
6	Brown	
7	Gray	
8	Dark Gray	
9	Light Blue	
10	Light Green	
11	Light Cyan	
12	Light Red	
13	Light Purple	
14	Yellow	
15	White	
*/

// bits 7-4 = background color
// bits 3-0 = foreground color