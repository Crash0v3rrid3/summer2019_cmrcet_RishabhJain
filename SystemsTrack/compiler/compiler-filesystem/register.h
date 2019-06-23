#ifndef _register_h
#define _register_h

enum registers {AX, BX, CX, DX, EX, FX, GX, HX};

int isRegister(char *token);
unsigned int getRegisterCode(char *token);

#endif