/*
 * nxtm - No Xorg Tiling Manager (sort of)
 * Mateusz d3s Dukat - 2019
 * 
 * Init:
 *  - Check /proc/bus/input/devices for keyboard event file
 *  - Check screen size and bpp
 *  - Draw background
 *  - (DEBUG) draw test text on bottom right (x*-1, y*-1)
 *
 * Main loop:
 *  - Check for keyboard
 *  - Make changes in windows
 *  - Redraw screen
 *
 */

#include <stdio.h>

#include <linux/fb.h> // fb_var_screeninfo
#include <linux/input.h> // input_event

#ifndef STRUCT_H
#include "struct.h"
#define STRUCT_H
#endif

#include "keyboard.h"

#define DEBUG


// Global structures
struct nxtmScreen NXTMScreen;
struct nxtmKeyboard NXTMKeyboard;
enum keyboardKeys NXTMKeyboardKeys;

int main(){
	keyboardInit((char*)"/dev/input/by-path/platform-i8042-serio-0-event-kbd", &NXTMKeyboard);

	printf("Keyboard fd: %d\n", NXTMKeyboard.fd);

	while(1){
		// Main loop
		keyboardUpdate(&NXTMKeyboard);
#ifdef DEBUG
		printf("IM RUNNING, ESCAPE: %d\n", (int)(NXTMKeyboard.keys[1]));
#endif
	}
	return 0;
}
