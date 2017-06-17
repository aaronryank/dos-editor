/* terminfo.c
   This is released under the Do Whatever The Heck You Want With It license, the terms and agreements being:
	Do whatever the heck you want with it.

   Honestly, this was simple enough to write I really don't care.
   Useful to see if your terminal screen is big enough for, e.g., an asciigame. Currently implemented in j-rogue112.
*/

#include <sys/ioctl.h>

int termsize(int x)
{
	struct winsize sz;
	ioctl(1,TIOCGWINSZ,&sz);

	switch(x) {
	case 'r': return sz.ws_row;
	case 'c': return sz.ws_col;
	default:  return -1;
	}
}
