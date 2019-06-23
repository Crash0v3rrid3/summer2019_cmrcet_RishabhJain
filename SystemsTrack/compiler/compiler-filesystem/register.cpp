#include "register.h"
#include <string.h>

int isRegister(char *token) {
	return strlen(token) == 2 && token[0] >= 'A' && token[0] <= 'H' && token[1] == 'X';
}

unsigned int getRegisterCode(char *token) {
	if (!strcmp("AX", token)) return AX;
	if (!strcmp("BX", token)) return BX;
	if (!strcmp("CX", token)) return CX;
	if (!strcmp("DX", token)) return DX;
	if (!strcmp("EX", token)) return EX;
	if (!strcmp("FX", token)) return FX;
	if (!strcmp("GX", token)) return GX;
	if (!strcmp("HX", token)) return HX;
}
