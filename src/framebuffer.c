#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#ifndef STRUCT_H
#include "struct.h"
#define STRUCT_H
#endif

void framebufferInit(char* framebufferDevice, struct nxtmScreen* NXTMScreen){
	NXTMScreen->fbfd = open(framebufferDevice, O_RDWR);
	if (NXTMScreen->fbfd == -1){
		perror("Error: cannot open framebuffer device");
		exit(1);
	}
	printf("The framebuffer device was opened successfully.\n");

	// Get fixed screen information
	if (ioctl(NXTMScreen->fbfd, FBIOGET_FSCREENINFO, &(NXTMScreen->finfo)) == -1){
		perror("Error reading fixed information");
		exit(2);
	}

	// Get variable screen information
	if (ioctl(NXTMScreen->fbfd, FBIOGET_VSCREENINFO, &(NXTMScreen->vinfo)) == -1){
		perror("Error reading variable information");
		exit(3);
	}

	printf("%dx%d, %dbpp\n", NXTMScreen->vinfo.xres, NXTMScreen->vinfo.yres, NXTMScreen->vinfo.bits_per_pixel);

	// Figure out the size of the screen in bytes
	NXTMScreen->screensize = NXTMScreen->vinfo.xres * NXTMScreen->vinfo.yres * NXTMScreen->vinfo.bits_per_pixel / 8;

	// Map the device to memory
	NXTMScreen->fbp = (char *)mmap(0, NXTMScreen->screensize, PROT_READ | PROT_WRITE, MAP_SHARED, NXTMScreen->fbfd, 0);
	if ((int)(NXTMScreen->fbp) == -1){
		perror("Error: failed to map framebuffer device to memory");
		exit(4);
	}
	printf("The framebuffer device was mapped to memory\n");
}

void drawBox(struct nxtmScreen* NXTMScreen, unsigned int screenX, unsigned int screenY, unsigned int boxX, unsigned int boxY, unsigned short int halfbppcolor, unsigned char R, unsigned char G, unsigned char B, unsigned char Alpha){
	//fseek((int *)(NXTMScreen->fbfd), 0, SEEK_SET);
	long int location = 0;
	
	// Figure out where in memory to put the pixel
	for (unsigned int y = screenY; y < (screenY+boxY); y++)
		for (unsigned int x = screenX; x < (screenX+boxX); x++){
			location = (x+(NXTMScreen->vinfo.xoffset)) * ((NXTMScreen->vinfo.bits_per_pixel)/8) + (y+(NXTMScreen->vinfo.yoffset)) * NXTMScreen->finfo.line_length;

			if (NXTMScreen->vinfo.bits_per_pixel == 32) {
				*(NXTMScreen->fbp + location) = B; // Blue
				*(NXTMScreen->fbp + location + 1) = G; // Green
				*(NXTMScreen->fbp + location + 2) = R; // Red
				*(NXTMScreen->fbp + location + 3) = Alpha; // Transparency
			}else{ // assume 16bpp
				//int b = 10;
				//int g = (x-100)/6;
				//int r = 31-(y-100)/16;
				//unsigned short int t = r<<11 | g<<5 | b;
				*((unsigned short int*)(NXTMScreen->fbp + location)) = halfbppcolor;
			}
		}
}

void drawBackground(struct nxtmScreen* NXTMScreen, unsigned char R, unsigned char G, unsigned char B){
	drawBox(NXTMScreen, 0, 0, (NXTMScreen->vinfo.xres)-6, (NXTMScreen->vinfo.yres)-6, 0, R, G, B, 0);
}

struct nxtmWindow* spawnWindow(struct nxtmScreen* NXTMScreen){
	struct nxtmWindow* Window = malloc(sizeof(struct nxtmWindow));
	Window->R = 100;
	Window->G = 100;
	Window->B = 100;
	Window->sizeX = 200;
	Window->sizeY = 200;
	Window->screenX = 10;
	Window->screenY = 10;
	Window->show = 1;
	drawWindow(Window, NXTMScreen);
	return Window;
}

void destroyWindow(struct nxtmWindow* Window){
	Window->show = 0;
}

void drawWindow(struct nxtmWindow* Window, struct nxtmScreen* NXTMScreen){
	// overlay
	drawBox(NXTMScreen, (Window->screenX)-1, (Window->screenY)-1, (Window->sizeX)+2, (Window->sizeY)+2, 0, 0xFF, 0xFF, 0xFF, 0);
	// window itself
	drawBox(NXTMScreen, Window->screenX, Window->screenY, Window->sizeX, Window->sizeY, 0, Window->R, Window->G, Window->B, 0);
}
// Test code below -----------------

/*
int main()
{
	int fbfd = 0;
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
	long int screensize = 0;
	char *fbp = 0;
	int x = 0, y = 0;
	long int location = 0;

	// Open the file for reading and writing
	fbfd = open("/dev/fb0", O_RDWR);
	if (fbfd == -1){
		perror("Error: cannot open framebuffer device");
		exit(1);
	}
	printf("The framebuffer device was opened successfully.\n");

	// Get fixed screen information
	if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1){
		perror("Error reading fixed information");
		exit(2);
	}

	// Get variable screen information
	if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1){
		perror("Error reading variable information");
		exit(3);
	}

	printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

	// Figure out the size of the screen in bytes
	screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

	// Map the device to memory
	fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	if ((int)fbp == -1){
		perror("Error: failed to map framebuffer device to memory");
		exit(4);
	}
	printf("The framebuffer device was mapped to memory\n");

	x = 100; y = 100; // We are going to put the pixel here

	// Figure out where in memory to put the pixel
	for (y = 100; y < 300; y++)
		for (x = 100; x < 300; x++){
			location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) + (y+vinfo.yoffset) * finfo.line_length;

			if (vinfo.bits_per_pixel == 32) {
				*(fbp + location) = 100; // Blue
				*(fbp + location + 1) = 15+(x-100)/2; // Green
				*(fbp + location + 2) = 200-(y-100)/5; // Red
				*(fbp + location + 3) = 0; // No transparency
			}else{ // assume 16bpp
				int b = 10;
				int g = (x-100)/6;
				int r = 31-(y-100)/16;
				unsigned short int t = r<<11 | g<<5 | b;
				*((unsigned short int*)(fbp + location)) = t;
			}
		}
	munmap(fbp, screensize);
	close(fbfd);
	return 0;
}
*/

