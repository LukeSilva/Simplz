#include "label.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "boilerplate.h"

const char *invalid_chars = "!@#$%^&*()-=+[{]}\\|;:\'\"/?.>,<";

struct label *label_init(const char *str, int addr)
{
	struct label *lbl;

	lbl = malloc(sizeof(*lbl));
	if (lbl == NULL)
		error("Out of Memory");

	lbl->addr = addr;

	memset(lbl->name, '\0', LABEL_SIZE);
	strncpy(lbl->name, str, LABEL_SIZE - 1);

	/* Check that the label's name is valid */
	if (strpbrk(lbl->name, invalid_chars) != 0)
		error("Invalid label name");

	return lbl;
}

void labels_append(struct label **cur, struct label *label)
{
	while (*cur != NULL) {
		cur = &((*cur)->nxt);
	}

	*cur = label;
}

void labels_print(const struct label *cur, FILE * o)
{
	/*fputs("|-------------------------|\n", o);
	fputs("| Name             | Addr |\n", o);
	fputs("|-------------------------|\n", o);*/
	while (cur != NULL) {
		fprintf(o, "%s:%02x\n", cur->name, cur->addr);
		cur = cur->nxt;
	}
	/*fputs("|-------------------------|\n", o);*/
}


struct label *labels_find(struct label *cur, const char* name)
{
	while (cur != NULL && strcmp(cur->name, name) != 0) {
		cur = cur->nxt;
	}
	return cur;
}
