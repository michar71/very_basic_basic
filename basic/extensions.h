#ifndef extensions_h
#define extensions_h


//------------------------------------
//USER DEFINED FUNCTIONS
//------------------------------------
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


//Hmmm... This is an array of variable namees and the number of arguments passed to a function.
//But how do we know what function was called? And how do we get to the variable contents?
int funhook_(char *msg, int n) 
{
	return 0;
}


void registerhook() 
{
    kwdhook=kwdhook_;
    funhook=funhook_;
}
//-------------------------------------------------------------

#endif