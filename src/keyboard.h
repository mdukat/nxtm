#ifndef STRUCT_H
#define STRUCT_H
#include "struct.h"
#endif

void keyboardInit(char *, struct nxtmKeyboard *);
void keyboardUpdate(struct nxtmKeyboard *);
//enum keyboardKeys;

enum keyboardKeys { // if this is somewhere in linux libraries, please tell me
	K_ESC = 1, K_1, K_2, K_3, K_4, K_5, K_6, K_7, K_8, K_9, K_0,
	K_BkSpc = 14, K_TAB, K_Q, K_W, K_E, K_R, K_T, K_Y, K_U, K_I, K_O, K_P,
	K_Enter = 28, K_LCtrl, K_A, K_S, K_D, K_F, K_G, K_H, K_J, K_K, K_L,
	K_LShift = 42, K_BkSlash, K_Z, K_X, K_C, K_V, K_B, K_N, K_M,
	K_LAlt = 56, K_Space, K_Caps, K_F1, K_F2, K_F3, K_F4, K_F5, K_F6, K_F7, K_F8, K_F9, K_F10,
	K_PauseBreak = 90, K_Insert, K_Home, K_PgUp, K_GREYSLASH, K_Delete, K_End, K_PgDn, K_RAlt, K_RCtrl, K_UP, K_LEFT, K_DOWN, K_RIGHT
};
