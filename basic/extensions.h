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

int funhook_(char *msg, int n) 
{
	if (!strcmp(msg,"PRINTS"))
		expr(), emit(PRINTS_);
	else	
		return 0;
	return 1;
}


void registerhook() 
{
    kwdhook=kwdhook_;
    funhook=funhook_;
}
//-------------------------------------------------------------

#endif