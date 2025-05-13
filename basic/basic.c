#include <stdio.h>
#include "basic.h"

int PRINTS_() 
{ 
	puts((char*)*sp++); STEP; 
}

int kwdhook_(char *msg) 
{
	///if we found a token we put the pointer to the function into the program queue
	if (!strcmp(msg,"PRINTS"))
		expr(), emit(PRINTS_);
	else	
		return 0;
	return 1;
}

int funhook_(char *msg, int n) 
{
	if (!strcmp(msg,"PRINTS"))
		expr(), emit(PRINTS_);
	else	
		return 0;
	return 1;
}


int main(int argc, char **argv) {
	FILE *sf=stdin;
	initbasic(0);
	kwdhook=kwdhook_;
	funhook=funhook_;
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
