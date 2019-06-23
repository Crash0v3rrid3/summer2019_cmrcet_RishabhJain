#ifndef _label_table_h
#define _label_table_h

typedef struct {
	char *blockName;
	unsigned int address;
} Label;

typedef struct {
	unsigned int numberOfLabels;
	Label *labels;
} LabelTable;

void addNewLabel(LabelTable *table, unsigned int address);
unsigned int findLabel(LabelTable *labelTable, char *name);

#endif