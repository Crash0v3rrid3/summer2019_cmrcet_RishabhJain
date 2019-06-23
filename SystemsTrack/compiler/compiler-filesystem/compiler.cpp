#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "compiler.h"
#include "tokenizer.h"
#include "symbol_table.h"
#include "stack.h"
#include "label_table.h"
#include "register.h"
#include "intermediate_code.h"
#include "util.h"

enum keyords {MOV_REG_TO_MEM, MOV_MEM_TO_REG, ADD, SUB, MUL, JMP, IF, EQ, LT, GT, LTEQ, GTEQ, PRINT, READ};

void *writeIntermediateCodeToOutputFile(IntermediateCode intermediateCode, SymbolTable symbolTable, Memory constantMapping, unsigned int *size) {
	unsigned int itr = 0, buf[4];
	char *finalData = (char *)calloc(symbolTable.numberOfSymbols + 4 + intermediateCode.numberOfInstructions, sizeof(Symbol) + 4 + 6 * 4), *finalDataPtr = finalData;

	// Write Constant table
	memcpy(&finalData, &symbolTable.numberOfSymbols, sizeof(unsigned int));
	finalDataPtr = (char *)finalDataPtr + 4;

	for (itr = 0; itr < symbolTable.numberOfSymbols; ++itr) {
		memcpy(finalDataPtr, symbolTable.symbols + itr, sizeof(Symbol));		// Write symbol Structure
		finalDataPtr += sizeof(Symbol);
		if (!symbolTable.symbols[itr].size) {									// if size == 0, write constant value
			memcpy(finalDataPtr, constantMapping.data + symbolTable.symbols[itr].address, sizeof(unsigned int));
			finalDataPtr += sizeof(unsigned int);
		}																		// During loading and execution, read structure, check
	}																			// value of size and proceed

	memcpy(finalDataPtr, &intermediateCode.numberOfInstructions, sizeof(unsigned int));
	finalDataPtr += sizeof(unsigned int);
	for (itr = 0; itr < intermediateCode.numberOfInstructions; ++itr) {
		memcpy(finalDataPtr, &intermediateCode.numberOfInstructions, sizeof(unsigned char) * intermediateCode.intermediateCode[itr].length);
		finalDataPtr += sizeof(unsigned char) * intermediateCode.intermediateCode[itr].length;
	}

	*size = finalDataPtr - finalData;
	return finalData;
}

void processConst(SymbolTable *table, Memory *mem, char *symbol, char *val) {
	unsigned int newLength;
	addNewSymbol(table, symbol, 0);
	newLength = table->numberOfSymbols + 1;

	if (mem->data) {
		mem->data = (unsigned char *)realloc(mem->data, newLength * sizeof(unsigned char));
	}	else	{
		mem->data = (unsigned char *)calloc(1, sizeof(unsigned char));
	}

	mem->data[newLength] = atoi(val);
}

void processSymbolTable(SymbolTable *table, char *symbol) {
	unsigned int size = 1, itr;
	char *pos, *actualSymbol;
	
	if (pos = strchr(symbol, '[')) {
		size = atoi(pos + 1);
		actualSymbol = (char *)calloc(pos - symbol, 1);

		for (itr = 0; itr < pos - symbol; ++itr) {
			actualSymbol[itr] = symbol[itr];
		}
		actualSymbol[itr] = 0;
		symbol = actualSymbol;
	}

	addNewSymbol(table, symbol, size);
}

void processArithmatic(SymbolTable *table, IntermediateCode *intermediateCode, char *op, char *op1, char *op2, char *op3) {
	unsigned int op1Code, op2Code, op3Code, opCode;

	op1Code = getRegisterCode(op1);
	op2Code = getRegisterCode(op2);
	op3Code = getRegisterCode(op3);
	
	if (!strcmp("ADD", op)) opCode = ADD;
	else if (!strcmp("SUB", op)) opCode = SUB;
	else if (!strcmp("MUL", op)) opCode = MUL;

	if (intermediateCode->numberOfInstructions == intermediateCode->maxNumberOfInstructions)
		resizeIntermediateCodeTable(intermediateCode);

	intermediateCode->intermediateCode[intermediateCode->numberOfInstructions].length = 4;
	intermediateCode->intermediateCode[intermediateCode->numberOfInstructions].data = (unsigned char *)calloc(4, sizeof(unsigned char));

	intermediateCode->intermediateCode[intermediateCode->numberOfInstructions].data[0] = opCode;
	intermediateCode->intermediateCode[intermediateCode->numberOfInstructions].data[1] = op1Code;
	intermediateCode->intermediateCode[intermediateCode->numberOfInstructions].data[2] = op2Code;
	intermediateCode->intermediateCode[intermediateCode->numberOfInstructions].data[3] = op3Code;
}

void processMove(SymbolTable *table, IntermediateCode *intermediateCode, char *op1, char *op2) {
	unsigned int op1Code, op2Code, opCode, tmp;

	if (isRegister(op1)) {
		opCode = MOV_MEM_TO_REG;
		op1Code = getRegisterCode(op1);
		tmp = getSymbol(*table, op2);
		op2Code = table->symbols[tmp].address;
	}	else	{
		opCode = MOV_REG_TO_MEM;
		tmp = getSymbol(*table, op1);
		op1Code = table->symbols[tmp].address;
		op2Code = getRegisterCode(op2);
	}

	if (intermediateCode->numberOfInstructions == intermediateCode->maxNumberOfInstructions)
		resizeIntermediateCodeTable(intermediateCode);

	intermediateCode->intermediateCode[intermediateCode->numberOfInstructions].length = 3;
	intermediateCode->intermediateCode[intermediateCode->numberOfInstructions].data = (unsigned char *)calloc(3, sizeof(unsigned char));

	intermediateCode->intermediateCode[intermediateCode->numberOfInstructions].data[0] = opCode;
	intermediateCode->intermediateCode[intermediateCode->numberOfInstructions].data[1] = op1Code;
	intermediateCode->intermediateCode[intermediateCode->numberOfInstructions].data[2] = op2Code;
}

void processRead(IntermediateCode *intermediateCode, char *operand) {
	unsigned int operandCode = getRegisterCode(operand);

	if (intermediateCode->numberOfInstructions == intermediateCode->maxNumberOfInstructions)
		resizeIntermediateCodeTable(intermediateCode);

	intermediateCode->intermediateCode[intermediateCode->numberOfInstructions].length = 2;
	intermediateCode->intermediateCode[intermediateCode->numberOfInstructions].data = (unsigned char *)calloc(2, sizeof(unsigned char));

	intermediateCode->intermediateCode[intermediateCode->numberOfInstructions].data[0] = READ;
	intermediateCode->intermediateCode[intermediateCode->numberOfInstructions].data[1] = operandCode;
}

void processPrint(SymbolTable *table, IntermediateCode *intermediateCode, char *operand) {
	unsigned int oprCode;

	oprCode = getSymbol(*table, operand);

	if (intermediateCode->numberOfInstructions == intermediateCode->maxNumberOfInstructions)
		resizeIntermediateCodeTable(intermediateCode);

	intermediateCode->intermediateCode[intermediateCode->numberOfInstructions].length = 2;
	intermediateCode->intermediateCode[intermediateCode->numberOfInstructions].data = (unsigned char *)calloc(2, sizeof(unsigned char));

	intermediateCode->intermediateCode[intermediateCode->numberOfInstructions].data[0] = PRINT;
	intermediateCode->intermediateCode[intermediateCode->numberOfInstructions].data[1] = oprCode;
}

void processIF(SymbolTable *table, Stack *s, char *op1, char *op, char *op2, IntermediateCode *intermediateCode) {
	unsigned int op1Code, op2Code, opCode, compCode;

	opCode = IF;
	op1Code = getRegisterCode(op1);
	op2Code = getRegisterCode(op2);

	if (!strcmp("EQ", op)) compCode = EQ;
	else if (!strcmp("LT", op)) compCode = LT;
	else if (!strcmp("GT", op)) compCode = GT;
	else if (!strcmp("LTEQ", op)) compCode = LTEQ;
	else if (!strcmp("GTEQ", op)) compCode = GTEQ;

	if (intermediateCode->numberOfInstructions == intermediateCode->maxNumberOfInstructions)
		resizeIntermediateCodeTable(intermediateCode);

	intermediateCode->intermediateCode[intermediateCode->numberOfInstructions].length = 5;
	intermediateCode->intermediateCode[intermediateCode->numberOfInstructions].data = (unsigned char *)calloc(4, sizeof(unsigned char));

	intermediateCode->intermediateCode[intermediateCode->numberOfInstructions].data[0] = opCode;
	intermediateCode->intermediateCode[intermediateCode->numberOfInstructions].data[1] = op1Code;
	intermediateCode->intermediateCode[intermediateCode->numberOfInstructions].data[2] = op2Code;
	intermediateCode->intermediateCode[intermediateCode->numberOfInstructions].data[3] = compCode;

	push(s, intermediateCode->numberOfInstructions);
}

void processELSE(SymbolTable *table, Stack *s, IntermediateCode *intermediateCode) {
	if (intermediateCode->numberOfInstructions == intermediateCode->maxNumberOfInstructions)
		resizeIntermediateCodeTable(intermediateCode);

	intermediateCode->intermediateCode[intermediateCode->numberOfInstructions].length = 2;
	intermediateCode->intermediateCode[intermediateCode->numberOfInstructions].data = (unsigned char *)calloc(4, sizeof(unsigned char));

	intermediateCode->intermediateCode[intermediateCode->numberOfInstructions].data[0] = JMP;

	push(s, intermediateCode->numberOfInstructions);
}

void processENDIF(SymbolTable *table, Stack *s, IntermediateCode *intermediateCode) {
	unsigned int instructionNumber, prevInstructionNumber;

	instructionNumber = intermediateCode->numberOfInstructions;
	prevInstructionNumber = peek(*s);
	pop(s);

	while (intermediateCode->intermediateCode[prevInstructionNumber].data[0] != IF) {
		intermediateCode->intermediateCode[prevInstructionNumber].data[1] = instructionNumber;
		instructionNumber = prevInstructionNumber;
		prevInstructionNumber = peek(*s);
		pop(s);
	}

	intermediateCode->intermediateCode[prevInstructionNumber].data[4] = instructionNumber;
}

void processLabel(LabelTable *table, char *labelName, unsigned int instructionNumber) {
	if (table->numberOfLabels) {
		table->labels = (Label *)realloc(table->labels, (table->numberOfLabels + 1) * sizeof(Label));
	}	else	{
		table->labels = (Label *)calloc(1, sizeof(Label));
	}

	table->labels[table->numberOfLabels].blockName = cloneStr(labelName);
	table->labels[table->numberOfLabels].address = instructionNumber;

	table->numberOfLabels++;
}

void processJump(IntermediateCode *intermediateCode, LabelTable *labelTable, char *name) {
	unsigned int labelInstructionNumber = findLabel(labelTable, name);

	if (intermediateCode->numberOfInstructions == intermediateCode->maxNumberOfInstructions)
		resizeIntermediateCodeTable(intermediateCode);

	intermediateCode->intermediateCode[intermediateCode->numberOfInstructions].length = 2;
	intermediateCode->intermediateCode[intermediateCode->numberOfInstructions].data = (unsigned char *)calloc(4, sizeof(unsigned char));

	intermediateCode->intermediateCode[intermediateCode->numberOfInstructions].data[0] = JMP;
	intermediateCode->intermediateCode[intermediateCode->numberOfInstructions].data[1] = labelTable->labels[labelInstructionNumber].address;
}

void *compile(char *sourceData, unsigned int *destFileSize) {
	unsigned int numberOfTokens, numberOfLabels = 0;
	char line[512], *fileData = sourceData, **tokens;
	IntermediateCode intermediateCode = {(Memory *)calloc(10, sizeof(Memory)), 0, 10};
	SymbolTable symbolTable = { 0, 0 };
	Stack s = { 0 };
	Memory mem = { 0, 0 };
	LabelTable labelTable = { 0, 0 };

	while (fileData[0] && sscanf(fileData, "%[^\n]s", line)) {
		fileData += strlen(line) + 1;
		tokens = tokenize(line, &numberOfTokens);

		if (!strcmp("DATA", tokens[0])) {
			processSymbolTable(&symbolTable, tokens[1]);
		}	else if (!strcmp("CONST", tokens[0]))	{
			processConst(&symbolTable, &mem, tokens[1], tokens[3]);
		}	else if (!strcmp("READ", tokens[0]))	{
			processRead(&intermediateCode, tokens[1]);
			intermediateCode.numberOfInstructions++;
		}	else if (!strcmp("MOV", tokens[0]))	{
			processMove(&symbolTable, &intermediateCode, tokens[1], tokens[2]);
			intermediateCode.numberOfInstructions++;
		}	else if (!strcmp("ADD", tokens[0]) || !strcmp("SUB", tokens[0]) || !strcmp("MUL", tokens[0]))	{
			processArithmatic(&symbolTable, &intermediateCode, tokens[0], tokens[1], tokens[2], tokens[3]);
			intermediateCode.numberOfInstructions++;
		}	else if (!strcmp("JMP", tokens[0]))	{
			processJump(&intermediateCode, &labelTable, tokens[1]);
			intermediateCode.numberOfInstructions++;
		}	else if (!strcmp("IF", tokens[0]))	{
			processIF(&symbolTable, &s, tokens[1], tokens[2], tokens[3], &intermediateCode);
			intermediateCode.numberOfInstructions++;
		}	else if (!strcmp("ELSE", tokens[0]))	{
			processELSE(&symbolTable, &s, &intermediateCode);
			intermediateCode.numberOfInstructions++;
		}	else if (!strcmp("ENDIF", tokens[0]))	{
			processENDIF(&symbolTable, &s, &intermediateCode);
		}	else if (!strcmp("PRINT", tokens[0]))	{
			processPrint(&symbolTable, &intermediateCode, tokens[1]);
			intermediateCode.numberOfInstructions++;
		}	else	{
			processLabel(&labelTable, tokens[0], numberOfLabels++);
		}
	}

	return writeIntermediateCodeToOutputFile(intermediateCode, symbolTable, mem, destFileSize);
}