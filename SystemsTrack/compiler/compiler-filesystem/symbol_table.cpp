#include <string.h>
#include <stdlib.h>
#include "symbol_table.h"
#include "util.h"

unsigned int getSymbolSize(char *symbol) {
	char *pos = strchr(symbol, '[');
	int size = 1;
	if (pos) {
		size = atoi(pos + 1);
	}	else	{
		pos = strchr(symbol, ':');
		if (pos)
			return -1;	// label
	}

	return size;
}

char *getSymbol(char *symbol) {
	char *token = strtok(cloneStr(symbol), "[:,");

	return token;
}

unsigned int getNextAddress(SymbolTable *table) {
	if (table->numberOfSymbols == 0) 
		return 0;
	unsigned int itr = table->numberOfSymbols - 1;

	for (; itr >= 0; itr--) {
		if (table->symbols[itr].size) {
			return table->symbols[itr].size + table->symbols[itr].address;
		}
	}
	return table->symbols[table->numberOfSymbols - 1].size + table->symbols[table->numberOfSymbols - 1].address;
}

void addNewSymbol(SymbolTable *table, char *symbol, unsigned int size) {
	if (table->numberOfSymbols > 0) {
		table->symbols = (Symbol *)realloc(table->symbols, (table->numberOfSymbols + 1) * sizeof(Symbol));
	}	else	{
		table->symbols = (Symbol *)calloc(1, sizeof(Symbol));
	}

	table->symbols[table->numberOfSymbols].address = getNextAddress(table);
	strcpy(table->symbols[table->numberOfSymbols].symbol, symbol);
	table->symbols[table->numberOfSymbols].size = size;
	
	table->numberOfSymbols++;
}

long long getSymbol(SymbolTable table, char *symbol) {
	unsigned int itr = 0;

	for (itr = 0; itr < table.numberOfSymbols; ++itr) {
		if (!strcmp(table.symbols[itr].symbol, symbol))
			return itr;
	}

	return -1;
}

unsigned int isSymbol(SymbolTable table, char *symbol) {

	return 0;
}