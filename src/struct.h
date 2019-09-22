#include <linux/fb.h> // fb_var_screeninfo
#include <linux/input.h> // input_event
#include <stdbool.h>

struct nxtmScreen {
	int fbfd; // framebuffer file descriptor
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
	long int screensize;

	char *fbp; // framebuffer memory map
	// int x = vinfo.xres
	// int y = vinfo.yres
	// int bpp = vinfo.bits_per_pixel
};

struct nxtmKeyboard {
	struct input_event ev;
	int fd; // keyboard device
	bool keys[105]; // state of all keys (+ virtual 4fun?)
			 // 1 = pressed; 0 = released
};

struct nxtmWindow {
	unsigned int sizeX;
	unsigned int sizeY;
	unsigned int screenX;
	unsigned int screenY;
	unsigned char R;
	unsigned char G;
	unsigned char B;
	bool show;
	bool active;
};
