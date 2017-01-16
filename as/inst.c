#include "inst.h"

#include <string.h>
#include <ctype.h>

#include "boilerplate.h"
#include "label.h"


void instruction_parse(struct instruction* instruction, const char* line)
{
	int i;
	char inst[4];

	/* Setup the structure */
	instruction->op = 0x00;
	instruction->lit = 0x00;
	instruction->inc = 0x00;
	for (i = 0; i < LABEL_SIZE; ++i)
		instruction->lbl_name[i] = '\0';

	
	/* Check that the line is long enough
	   " ADD "
	   1 character for preceeding space / tab
	   3 characters for instruction name
	   1 character for space between instruction and argument
	   = 5 characters in total
	*/
	if (strlen(line) < 5)
		error("Invalid instruction");


	/* Copy the instruction's name */
	for (i = 0; i < 3; ++i)
		inst[i] = toupper(line[i + 1]);
	inst[3] = '\0';


	/* Identify the instruction type */
	if (strcmp(inst, "ADD") == 0)
		instruction->op = 0x00;
	else if (strcmp(inst, "NOR") == 0)
		instruction->op = 0x40;
	else if (strcmp(inst, "STR") == 0)
		instruction->op = 0x80;
	else if (strcmp(inst, "JPC") == 0)
		instruction->op = 0xC0;
	else if (strcmp(inst, "LIT") == 0)
		instruction->op = 0x00;
	else
		error("Unknown instruction");

	/* Parse the operand */
	if (line[5] == '\0') {
		/* end of line - ignore */
	} else if (line[5] == '@') {
		/* label */
		for (i = 0; i < LABEL_SIZE; ++i) {
			if (line[6+i] == '\0' || line[6+i] == '+') {
				instruction->lbl_name[i] = '\0';
				break;
			}
			else {
				instruction->lbl_name[i] = line[6+i];
			}
		}

		/* increment */
		if (line[6+i] == '+')
			sscanf(&line[7+i], "%x", &instruction->inc);
	} else if (line[5] == '$') {
		/* hex literal */
		sscanf(&line[6], "%x", &instruction->lit);
	} else if (line[5] == '#') {
		/* dec literal */
		sscanf(&line[6], "%d", &instruction->lit);
	}
	
}

void instructions_compile(
	const struct instruction *instructions,
	struct label *labels,
	FILE *out
	)
{
	int i;
	
	for (i = 0; i < MEM_SIZE; ++i)
	{
		int val;
		
		if (i % 16 == 0 && i != 0)
			fputs("\n", out);

		if (instructions[i].lbl_name[0] == '\0') {
			/* no label - use literal */
			val = instructions[i].op;
			val |= instructions[i].lit;
		} else {
			/* label - use lbl->addr + inc */
			struct label *lbl;
			lbl = labels_find(labels, instructions[i].lbl_name);

			if (lbl == NULL) {
			        int j;
				printf("\n\n-----\n");
				for (j = 0; j < LABEL_SIZE; ++j)
					printf("%02x ", instructions[i].lbl_name[j]);
				error("Undefined label");
			}

			val = lbl->addr;
			val += instructions[i].inc;
			val = (val & 0x3F) | instructions[i].op;
		}

		fprintf(out, "%02x ", val);
	}

	fputs("\n", out);
}
