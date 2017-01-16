#include "boilerplate.h"

#include <stdio.h>
#include <stdlib.h>

void error(const char *str)
{
	fputs("Error: ", stderr);
	fputs(str, stderr);
	fputs("\n", stderr);
	exit(-1);
}
