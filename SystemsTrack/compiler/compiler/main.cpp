#include <stdio.h>
#include "compiler.h"

int main() {
	compile("program.mrnd", "program.executable");
	
	return 0;
}