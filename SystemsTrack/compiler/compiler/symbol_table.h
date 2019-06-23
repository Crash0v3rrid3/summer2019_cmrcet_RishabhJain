#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

typedef struct {
	char symbol[4];			// A, B...
	unsigned int address;	// 0x0, 0x4, 0x8...
	unsigned int size;		// Size of type 0 - const, 1+ - bytes
} Symbol;

typedef struct {
	Symbol *symbols;
	unsigned int numberOfSymbols;
} SymbolTable;


long long getSymbol(SymbolTable table, char *symbol);
void addNewSymbol(SymbolTable *table, char *symbol, unsigned int size);
unsigned int isSymbol(char *symbol);

#endif