#include <stdlib.h>
#include "intermediate_code.h"

void resizeIntermediateCodeTable(IntermediateCode *intermediateCode) {
	intermediateCode->intermediateCode = (Memory *)realloc(intermediateCode->intermediateCode, (intermediateCode->maxNumberOfInstructions + 10) * sizeof(Memory));
	intermediateCode->maxNumberOfInstructions += 10;
}