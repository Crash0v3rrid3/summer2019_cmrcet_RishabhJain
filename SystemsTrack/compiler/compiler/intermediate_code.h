#ifndef _intermediate_code_h
#define _intermediate_code_h

typedef struct {
	unsigned char *data;
	unsigned int length;
} Memory;

typedef struct {
	Memory *intermediateCode;
	unsigned int numberOfInstructions;
	unsigned int maxNumberOfInstructions;
} IntermediateCode;

void resizeIntermediateCodeTable(IntermediateCode *intermediateCode);

#endif