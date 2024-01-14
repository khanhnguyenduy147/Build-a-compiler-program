/*
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "reader.h"
#include "parser.h"
#include "codegen.h"

/******************************************************************/

int main(int argc, char *argv[])
{
	if (argc <= 1)
	{
		printf("kplc: no input file.\n");
		return -1;
	}
	initCodeBuffer();
	if (compile(argv[1]) == IO_ERROR)
	{
		printf("Can\'t read input file!\n");
		return -1;
	}

	printf("\n\nAssembly: \n");
	printCodeBuffer();

	return 0;
}
