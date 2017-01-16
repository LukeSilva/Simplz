#ifndef MEM_H
#define MEM_H

#include "label.h"

#define MEM_SIZE 0x3e
#define MEM_IN 0x3f
#define MEM_OUT 0x3e

struct instruction {

	/*
	  Literals are encoded with op = 0, and addr = value
	*/
	
	/* The instruction op code */
	int op;

	/* 
	   The user-specified address as the operand
	   Only used when lbl_name is NULL
	*/
	int lit;

	/* The label to use as the address operand */
	char lbl_name[LABEL_SIZE];
	/* An increment to add to the labels address */
	int inc;
};

void instruction_parse(struct instruction *inst, const char *line);
void instructions_compile(
	const struct instruction *instructions,
	struct label *labels,
	FILE *out
	);



#endif
