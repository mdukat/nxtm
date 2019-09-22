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
#include <stdlib.h> // system, exit

#ifndef STRUCT_H
#include "struct.h"
#define STRUCT_H
#endif

#include "keyboard.h"
#include "framebuffer.h"

//#define DEBUG


// Global structures
struct nxtmScreen NXTMScreen;
struct nxtmKeyboard NXTMKeyboard;
enum keyboardKeys NXTMKeyboardKeys;
struct nxtmWindow* NXTMWindows[20];
int selectedWindow = 0; // 0-19

void closenxtm(){
	system("clear && chvt 3");
	printf("Exiting\n");
	exit(0);
}

void redrawWindows(){
	drawBackground(&NXTMScreen, 120, 120, 200);
	for(int i = 0; i<20; i++){
		if(NXTMWindows[i]->show == 1){
			if(NXTMWindows[i]->active)
				drawWindow(NXTMWindows[i], &NXTMScreen, 1);
			else
				drawWindow(NXTMWindows[i], &NXTMScreen, 0);
		}
	}
}

int main(){
	//printf("\\e[?17;14;224c"); // stop cursor from blinking // TODO this does not work
	system("/bin/chvt 8"); // God forgive me for what i have to do here
	
	keyboardInit((char*)"/dev/input/by-path/platform-i8042-serio-0-event-kbd", &NXTMKeyboard);
	framebufferInit((char *)"/dev/fb0", &NXTMScreen);

	// Windows init
	for(int i = 0; i<20; i++){
		NXTMWindows[i] = (struct nxtmWindow *)malloc(sizeof(struct nxtmWindow *));
		NXTMWindows[i]->R = 100;
		NXTMWindows[i]->G = 100;
		NXTMWindows[i]->B = 100;
		NXTMWindows[i]->sizeX = 200;
		NXTMWindows[i]->sizeY = 200;
		NXTMWindows[i]->screenX = 10;
		NXTMWindows[i]->screenY = 10;
		NXTMWindows[i]->show = 0;
	}
	printf("Keyboard fd: %d\n", NXTMKeyboard.fd);
	
	drawBackground(&NXTMScreen, 120, 120, 200);
	//drawBox(&NXTMScreen, 0, 0, (unsigned int)(NXTMScreen.vinfo.xres), (unsigned int)(NXTMScreen.vinfo.yres), 0, 120, 120, 200, 0);
	//drawBox(&NXTMScreen, 100, 100, 300, 300, 0, 100, 100, 100, 20);

	while(1){
		// Main loop
		keyboardUpdate(&NXTMKeyboard);
		if(NXTMKeyboard.keys[K_LAlt] && NXTMKeyboard.keys[K_Q]) // Close NXTM
			closenxtm();
		
		if(NXTMKeyboard.keys[K_LAlt] && NXTMKeyboard.keys[K_Enter]){ // Create new window
			for(int i = 0; i<20; i++){
				if(NXTMWindows[i]->show == 0){
					//drawBackground(&NXTMScreen, 120, 120, 200);
					NXTMWindows[i]->show = 1;
					NXTMWindows[selectedWindow]->active = 0;
					NXTMWindows[i]->active = 1;
					selectedWindow = i;
					printf("Spawned new window\n");
					while(NXTMKeyboard.keys[K_Enter] == 1)
						keyboardUpdate(&NXTMKeyboard);
					printf("Released\n");
					break;
				}
			}
			redrawWindows();
		}

		if(NXTMKeyboard.keys[K_LAlt] && NXTMKeyboard.keys[K_C]){ // Close all windows (actually hide)
			for(int i = 0; i<20; i++){
				destroyWindow(NXTMWindows[i]);
			}
			drawBackground(&NXTMScreen, 120, 120, 200);
		}
		
		if(NXTMKeyboard.keys[K_LAlt] && NXTMKeyboard.keys[K_L] && NXTMKeyboard.keys[K_LShift]){ // resize to right
			if(NXTMWindows[selectedWindow]->show){
				NXTMWindows[selectedWindow]->sizeX += 10;
				redrawWindows();
			}
		}else if(NXTMKeyboard.keys[K_LAlt] && NXTMKeyboard.keys[K_L]){ // move window to right
			if(NXTMWindows[selectedWindow]->show){
				NXTMWindows[selectedWindow]->screenX += 10;
				redrawWindows();
			}
		}

		if(NXTMKeyboard.keys[K_LAlt] && NXTMKeyboard.keys[K_J] && NXTMKeyboard.keys[K_LShift]){ // resize to down 
			if(NXTMWindows[selectedWindow]->show){
				NXTMWindows[selectedWindow]->sizeY += 10;
				redrawWindows();
			}
		}else if(NXTMKeyboard.keys[K_LAlt] && NXTMKeyboard.keys[K_J]){ // move window to down
			if(NXTMWindows[selectedWindow]->show){
				NXTMWindows[selectedWindow]->screenY += 10;
				redrawWindows();
			}
		}
		
		if(NXTMKeyboard.keys[K_LAlt] && NXTMKeyboard.keys[K_H] && NXTMKeyboard.keys[K_LShift]){ // resize to left
			if(NXTMWindows[selectedWindow]->show){
				NXTMWindows[selectedWindow]->sizeX -= 10;
				redrawWindows();
			}
		}else if(NXTMKeyboard.keys[K_LAlt] && NXTMKeyboard.keys[K_H]){ // move window to left
			if(NXTMWindows[selectedWindow]->show){
				NXTMWindows[selectedWindow]->screenX -= 10;
				redrawWindows();
			}
		}

		if(NXTMKeyboard.keys[K_LAlt] && NXTMKeyboard.keys[K_K] && NXTMKeyboard.keys[K_LShift]){ // resize to up
			if(NXTMWindows[selectedWindow]->show){
				NXTMWindows[selectedWindow]->sizeY -= 10;
				redrawWindows();
			}
		}else if(NXTMKeyboard.keys[K_LAlt] && NXTMKeyboard.keys[K_K]){ // move window to up
			if(NXTMWindows[selectedWindow]->show){
				NXTMWindows[selectedWindow]->screenY -= 10;
				redrawWindows();
			}
		}

		if(NXTMKeyboard.keys[K_LAlt] && NXTMKeyboard.keys[K_2]){ // go to next window
			NXTMWindows[selectedWindow]->active = 0;
			selectedWindow++;
			if(selectedWindow == 20)
				selectedWindow = 0;
			NXTMWindows[selectedWindow]->active = 1;
			redrawWindows();
			while(NXTMKeyboard.keys[K_2])
				keyboardUpdate(&NXTMKeyboard);
		}

		if(NXTMKeyboard.keys[K_LAlt] && NXTMKeyboard.keys[K_1]){ // go to previous window
			NXTMWindows[selectedWindow]->active = 0;
			selectedWindow--;
			if(selectedWindow == -1)
				selectedWindow = 19;
			NXTMWindows[selectedWindow]->active = 1;
			redrawWindows();
			while(NXTMKeyboard.keys[K_1])
				keyboardUpdate(&NXTMKeyboard);
		}

#ifdef DEBUG
		printf("IM RUNNING, ESCAPE: %d\n", (int)(NXTMKeyboard.keys[1]));
#endif
	}
	return 0;
}
