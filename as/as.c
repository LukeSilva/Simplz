#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "boilerplate.h"
#include "label.h"
#include "inst.h"

struct context
{
	struct label *labels;
	struct instruction *instructions;
	int pos;
	
};

struct context *context_init()
{
	struct context *context;
	context = malloc(sizeof(*context));
	if (context == NULL)
		error("Out of Memory");
	
	context->labels = NULL;
	context->pos = 0;
	
	context->instructions = calloc(MEM_SIZE, sizeof(*context->instructions));
	if (context->instructions == NULL)
		error("Out of Memory");

	return context;
}

void assemble_file(struct context *context, const char *filename)
{
	char line[256];
	FILE *in;

	in = fopen(filename, "r");

	while(fgets(line, 255, in) != NULL && context->pos < MEM_SIZE) {
		/* remove trailing new line */
		line[strcspn(line, "\r\n")] = '\0';

		if (line[0] == ';' || line[0] == '\0') {
			/* ignore comments and empty lines */
		} else if (line[0] == ':') {
			struct label *lbl;
			lbl = label_init(&line[1], context->pos);
			labels_append(&context->labels, lbl);
		} else if (line[0] == '\t' || line[0] == ' ') {
			struct instruction* inst;
			inst = &context->instructions[context->pos++];
			
			instruction_parse(inst, line);
		} else
			error("Unknown statement");

	}
	
	
	fclose(in);
}


int main(int argc, char **argv)
{
	struct context *context;
	const char *filename;
	FILE *out_file;
	FILE *lbl_file;
	context = context_init();

	filename = "";
	out_file = stdout;
	lbl_file = NULL;
	
	
	if (argc < 2 || argc > 4) {
		error("Usage: simplz-as <in-file> [out-file] [lbl-file]");
	}
	if (argc > 1) {
		filename = argv[1];
	}
	if (argc > 2) {
		out_file = fopen(argv[2], "w");
		if (out_file == NULL)
			error("Could not open out-file");
	}
	if (argc > 3) {
		lbl_file = fopen(argv[3], "w");
		if (lbl_file == NULL)
			error("Could not open lbl-file");
	}


	labels_append(&(context->labels), label_init("OUT", 0x3f));
	labels_append(&(context->labels), label_init("IN", 0x3e));
	
	assemble_file(context, filename);
	
	instructions_compile(
		context->instructions,
		context->labels,
		out_file
		);
	
	if (lbl_file != NULL)
		labels_print(
			context->labels,
			lbl_file
			);
}
