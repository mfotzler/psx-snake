#include <sys/types.h>	// This provides typedefs needed by libgte.h and libgpu.h
#include <stdio.h>	// Not necessary but include it anyway
#include <stdlib.h>
#include <stdbool.h>
#include <psxetc.h>	// Includes some functions that controls the display
#include <psxgte.h>	// GTE header, not really used but libgpu.h depends on it
#include <psxgpu.h>	// GPU library header
#include <psxpad.h>
#include <psxapi.h>

#define OTLEN 8         // Ordering table length (recommended to set as a define
                        // so it can be changed easily)

DISPENV disp[2];
DRAWENV draw[2];
int db = 0;

u_long ot[2][OTLEN];    // Ordering table length
char pribuff[2][32768]; // Primitive buffer
char *nextpri;          // Next primitive pointer
u_char padbuff[2][34];  // Controller input buffers

void display() {
    
    DrawSync(0);                // Wait for any graphics processing to finish
    
    VSync(0);                   // Wait for vertical retrace

    PutDispEnv(&disp[db]);      // Apply the DISPENV/DRAWENVs
    PutDrawEnv(&draw[db]);

    SetDispMask(1);             // Enable the display

    DrawOTag(ot[db]+OTLEN-1);   // Draw the ordering table
    
    db = !db;                   // Swap buffers on every pass (alternates between 1 and 0)
    nextpri = pribuff[db];      // Reset next primitive pointer
    
}

void init() {
    // Reset graphics
    ResetGraph(0);

    // First buffer
    SetDefDispEnv(&disp[0], 0, 0, 320, 240);
    SetDefDrawEnv(&draw[0], 0, 240, 320, 240);
    // Second buffer
    SetDefDispEnv(&disp[1], 0, 240, 320, 240);
    SetDefDrawEnv(&draw[1], 0, 0, 320, 240);

    draw[0].isbg = 1;               // Enable clear
    setRGB0(&draw[0], 0, 0, 0);  // Set clear color (dark purple)
    draw[1].isbg = 1;
    setRGB0(&draw[1], 0, 0, 0);

    nextpri = pribuff[0];           // Set initial primitive pointer address

	InitPAD(padbuff[0], 34, padbuff[1], 34);
	StartPAD();
    ChangeClearPAD(1);
}

void drawSquare(TILE* tile, int x, int y) {
	tile = (TILE*)nextpri;      // Cast next primitive

	setTile(tile);              // Initialize the primitive (very important)
	setXY0(tile, x, y);       // Set primitive (x,y) position
	setWH(tile, 8, 8);        // Set primitive size
	setRGB0(tile, 255, 255, 0); // Set color yellow
	addPrim(ot[db], tile);      // Add primitive to the ordering table
	
	nextpri += sizeof(TILE);    // Advance the next primitive pointer
}

int jitter(int currentValue, int min, int max, int defaultValue) {
	int result = currentValue + (rand() % 11 - 5);
	if(result > max || result < min)
		result = defaultValue;

	return result;
}

bool isController1Connected() {
	return padbuff[0][0] == 0;
}

bool isButtonPressed(PADTYPE *pad, PadButton button) {
    return !(pad->btn & button);
}

void updatePositionFromPad(PADTYPE *pad, int *x, int *y) {
    if(isController1Connected()) {
        if(isButtonPressed(pad, PAD_RIGHT))
            *x += 4;
        if(isButtonPressed(pad, PAD_LEFT))
            *x -= 4;
        if(isButtonPressed(pad, PAD_DOWN))
            *y += 4;
        if(isButtonPressed(pad, PAD_UP))
            *y -= 4;
    }
}

void beforeGameLogic() {
    ClearOTagR(ot[db], OTLEN);  // Clear ordering table
}

void afterGameLogic() {
    display();
}

int main() {
    TILE *tile;                     // Primitive pointer
    int x = 150;
	int y = 100;
    PADTYPE *pad = (PADTYPE*)padbuff[0];

	init();
    while(1) {
        beforeGameLogic();

        updatePositionFromPad(pad, &x, &y);
		drawSquare(tile, x, y);

        afterGameLogic();
    }
    
    return 0;
}