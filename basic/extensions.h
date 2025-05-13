#ifndef extensions_h
#define extensions_h


//------------------------------------
//USER DEFINED FUNCTIONS
//------------------------------------
#define PRINTS_T "PRINTS"
#define SCALE256_T "SCALE256"

int PRINTS_() 
{ 
    //Output the string on the Run-time Stack and return for next command.
	puts((char*)*sp++); STEP; 
}

//This looks for a keyword and adds the associated function pointer to the program queue.
int kwdhook_(char *msg) 
{
	if (!strcmp(msg,PRINTS_T))
		expr(), emit(PRINTS_);
    else
		return 0;
    return 1;    
}


//Hmmm... so we get the name of the function and the number of arguments.
//How do we extract the arguments?
//How do we return a value? (Via Ret variable? Doesn't seem to work....)
//Do we need to first register a function for the call stack or process it here directly?
int funhook_(char *msg, int n) 
{   
	if (!strcmp(msg,SCALE256_T))
    {
        int val = sub[find(msg)][2];
		ret = val * 2;
    }
    else	
		return 0;
	return 1;
}

//-------------------------------------

void registerhook() 
{
    kwdhook=kwdhook_;
    funhook=funhook_;
}


#endif