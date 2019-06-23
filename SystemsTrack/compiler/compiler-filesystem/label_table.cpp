#include <stdlib.h>
#include <string.h>
#include "label_table.h"

void addNewLabel(LabelTable *table, char *name, unsigned int address) {
	if (table->numberOfLabels > 0) {
		table->labels = (Label *)realloc(table->labels, (table->numberOfLabels + 1) * sizeof(Label));
	}
	else	{
		table->labels = (Label *)calloc(1, sizeof(Label));
	}

	table->labels[table->numberOfLabels].blockName = name;
	table->labels[table->numberOfLabels].address = address;
	table->numberOfLabels++;
}

unsigned int findLabel(LabelTable *labelTable, char *name) {
	unsigned int itr;

	for (itr = 0; itr < labelTable->numberOfLabels; ++itr) {
		if (!strcmp(name, labelTable->labels[itr].blockName))
			return itr;
	}
}