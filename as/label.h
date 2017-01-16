#ifndef LABEL_H
#define LABEL_H

#include <stdio.h>

#define LABEL_SIZE 16

struct label {
	char name[LABEL_SIZE];
	int addr;

	struct label *nxt;
};

struct label *label_init(const char *str, int addr);

void labels_append(struct label **labels, struct label *label);
void labels_print(const struct label *labels, FILE* o);
struct label * labels_find(struct label *labels, const char* name);



#endif
