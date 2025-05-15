#include <stdio.h>
#include "basic.h"


int main(int argc, char **argv) {
	FILE *sf=stdin;
	initbasic(0);

 //   argv[1] = "testf.bas";
	if (argv[1])
	{
		if ((sf=fopen(argv[1],"r")))
			compile++;
	}
	else 
	{
		printf("CANNOT OPEN: %s\n", argv[1]);
		return 255;
	}
	return interp(sf);
}