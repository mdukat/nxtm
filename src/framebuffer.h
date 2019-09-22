#ifndef STRUCT_H
#define STRUCT_H
#include "struct.h"
#endif

void framebufferInit(char *, struct nxtmScreen *);

void drawBox(struct nxtmScreen *, unsigned int screenX, unsigned int screenY, unsigned int boxX, unsigned int boxY, unsigned short int halfbppcolor, unsigned char R, unsigned char G, unsigned char B, unsigned char Alpha);

void drawBackground(struct nxtmScreen *, unsigned char R, unsigned char G, unsigned char B);

struct nxtmWindow* spawnWindow(struct nxtmScreen *);

void destroyWindow(struct nxtmWindow *);

void drawWindow(struct nxtmWindow *, struct nxtmScreen *, bool);
