#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <string.h>
#include <assert.h>

#include <stdbool.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>

#ifndef STRUCT_H
#define STRUCT_H
#include "struct.h"
#endif

/*
 * Here, we want to check for user input. First check in main loop.
 * Key codes for linux: http://www.penticoff.com/nb/kbds/ibm104kb.htm
 */

void keyboardInit(char* keyboardDevice, struct nxtmKeyboard* NXTMKeyboard){
	// open keyboard device
	NXTMKeyboard->fd = open(keyboardDevice, O_RDONLY);
	assert(NXTMKeyboard->fd != -1);

	for(int i = 0; i<105; i++) // clear keys status
		NXTMKeyboard->keys[i] = false;
}

void keyboardUpdate(struct nxtmKeyboard* NXTMKeyboard){
	ssize_t n = (ssize_t)0; //ssize_t n = read(NXTMKeyboard->fd, &(NXTMKeyboard->ev), sizeof NXTMKeyboard->ev);
	fd_set set; // We want timeout on read of keyboard, since it affects framebuffer redrawing in main loop
	struct timeval timeout; // TODO move this structure and fd_set to struct.h NXTMKeyboard?
	int rv;
	FD_ZERO(&set);
	FD_SET(NXTMKeyboard->fd, &set);
	timeout.tv_sec = 0;
	timeout.tv_usec = 1000;

	rv = select(NXTMKeyboard->fd + 1, &set, NULL, NULL, &timeout);
	if(rv == -1){
		perror("select");
		return;
	}else if(rv == 0){
		//printf("timeout");
		return;
	}else{
		n = read(NXTMKeyboard->fd, &(NXTMKeyboard->ev), sizeof NXTMKeyboard->ev);
	}

	if (n == (ssize_t)-1){
		if (errno != EINTR) return;
	}else if(n != sizeof NXTMKeyboard->ev){
		errno = EIO;
		return;
	}
	if (NXTMKeyboard->ev.type == EV_KEY && NXTMKeyboard->ev.value >= 0 && NXTMKeyboard->ev.value <= 2){
		if(NXTMKeyboard->ev.value == 1)
			NXTMKeyboard->keys[NXTMKeyboard->ev.code] = 1;
		if(NXTMKeyboard->ev.value == 0)
			NXTMKeyboard->keys[NXTMKeyboard->ev.code] = 0;
		//printf("%s 0x%04x (%d)\n", evval[ev.value], (int)ev.code, (int)ev.code);
	}
}


// ======= Test code down below ========
/*
static const char *const evval[3] = {
	"RELEASED", "PRESSED", "REPEATED"
};

int main(void){
	const char *dev = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";
	struct input_event ev;
	ssize_t n;
	int fd;

	fd = open(dev, O_RDONLY);
	assert(fd != -1);

	while(1){
		n = read(fd, &ev, sizeof ev);
		if (n == (ssize_t)-1){
			if (errno == EINTR)
				continue;
			else
				break;
		}else if(n != sizeof ev){
			errno = EIO;
			break;
		}
		if (ev.type == EV_KEY && ev.value >= 0 && ev.value <= 2)
			printf("%s 0x%04x (%d)\n", evval[ev.value], (int)ev.code, (int)ev.code);
	}
	fflush(stdout);
	fprintf(stderr, "%s.\n", strerror(errno));
	return EXIT_FAILURE;
}
*/
