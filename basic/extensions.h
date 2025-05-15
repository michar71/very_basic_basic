#ifndef extensions_h
#define extensions_h

/*
Functions:
=========

Math/Mapping:

LOADMAP(0...x)  (GAMMA,LOG, SIN,PULSE,FIRE_R,FIRE_G,FIRE_B,TEMPERATURE_R,TEMPERATURE_G, TEMPERATUE_B, RAINBOW_R, RAINBOW_G,RAINBOW_B)
SETMAP256(array)
int MAP(int val)
uint8 SCALE256(int val)    
uint8 LIMIT256(int val)
int RANDOM(int min, int max)
int ABS(int)

System:

int TIMESTAMP(mS,Sec)
WAIT (int ms)
uint8 READANALOG(int ch)
uint8 READPIN(int gpio)
SETPIN(int gpio)
CLEARPIN(int gpio)
WAITEVENT(int event, int cond)  -> 1=Wait for a GPIO to Toggle to 0 or 1,Wait for an analog Channel to go above or below a threshold, wait for timer to reach a specific value in mS or Sec 

Color Space ->RGBtoHSB and Back (Requires multiple parameters or arrays as parameters? For all these color functions we just ignore values in arrays larger then 3 and set it to zero if less then 3.... ):

int RGBTOH(array[3])
int RGBYOS(array[3])
int RGBTOB(array[3])
int HSBTOR(array[3])
int HSBTOG(array[3])
int HSBTOB(array[3])

HSBTORGB(array[3],array[3])
RGBTOHSB(array[3],array[3])

LED Specific:
SETLED(array R, array G, array B)   -> Upload Array to LED Strip. LED Length is defined by array size. Will init LED Strip on first call. (Usually set all to black...)
CLEARLED(count)  -> Saves us from having to define an array, set the whole array to 0 and then having to send the array....
COLORLED(int start, int end, array[3]) -> Set the whole array to one solid color

Array:
SHIFT_A(array A, int amount) -> Shift the values of an array to left or right by a specific amount
ROTATE_A(array A, int amount)
SET_A(array A, int start,int end, int value) -> Set the value of an array at a specific index
SCALE_A(array A, int perc_scale) -> Scale the values of an array by a percentage
SCALE_LIMIT_A(array A, int perc) -> Scale the values of an array by a percentage but kimit it to max 255, min 0
*/


//------------------------------------
//USER DEFINED FUNCTIONS
//------------------------------------
#define PRINTS_T "PRINTS"

#define LIKMIT256_T "LIMIT256"
#define SCALE256_T "SCALE256"

#define ABS_T "ABS"

#define SETARRAY_T "SETARRAY"
#define SHIFTARRAY_T "SHIFTARRAY"
#define ROTATEARRAY_T "ROTATEARRAY"
#define SCALEARRAY_T "SCALEARRAY"
#define SCALELIMITARRAY_T "SCALELIMITARRAY"

int PRINTS_() 
{ 
    //Output the string on the Run-time Stack and return for next command.
	puts((char*)*sp++); STEP; 
}




//This looks for a keyword and adds the associated function pointer to the program queue.
int kwdhook_(char *msg) 
{
	if (!strcmp(msg,PRINTS_T))
    {
		expr(), emit(PRINTS_);STEP;
    }  
    else
		return 0;
}

int ABS_() 
{ 
    int val = *sp--;  //Pull value from Stack
    if (val<0) 
        val = -val; //Make it positive
    *sp=val; //Push back to to the stack
}

//Hmm.. Do we scale from the whole 4 bit integer? Thats a huge number space.... 
//Or do we asume the numbers are limited to 2 bytes? (0..65535)
int SCALE256_() 
{ 
    int val = *sp--;  //Pull value from Stack
    val = 2*val; //Multiply by 2
    *sp=val; //Push 88 to the stack
}

int LIMIT256_() 
{ 
    int val = *sp--;  //Pull value from Stack
    if (val<0)        //Limit it to 9..255
        val = 0; 
    if (val>255)
        val = 255; 
    *sp=val; //Push 88 to the stack
}

int SETARRAY()
{ 
    //Pull the values off the stack
    Val *s = sp; //Stack pointer
    int val = (int)*s++;
    int end = (int)*s++;
    int start = (int)*s++;
    Val *arr = (Val*)*s++;  

    sp--; //Rewind stack pointer

    if (arr == 0)
    {
        bad("SETARRAY: BAD ARRAY POINTER");
        return 0;
    }

    int size = (int)arr[0];

    //Check if the array indices are valid
    if (start<1 || end<1 || start>end || end>size)
    {
        bad("SETARRAY: INDEX OUT OF BOUNDS");
        return 0;
    }

    //Set Array
    int i;
    for (i=start; i<=end; i++)
        arr[i] = val; //Push back to to the stack


    //Hmm... How do we deal with no return??? Just return a dummy value?
    *sp = 0; //Push 0 to the stack
}



//Hmmm... so we get the name of the function and the number of arguments.
//Validate here if the number of arguments is correct and then push the function to the progam buffer.
//In the function we pull the arguments off the dstasck and put the result back on the stack.
int funhook_(char *msg, int n) 
{   
	if (!strcmp(msg,SCALE256_T))
    {
        if (n!=1) 
        {
            bad("SCALE256: 1 ARGUMENT REQUIRED");
            return 0;
        }
        emit(SCALE256_);STEP;
    }
	if (!strcmp(msg,ABS_T))
    {
        if (n!=1) 
        {
            bad("ABS: 1 ARGUMENT REQUIRED");
            return 0;
        }
        emit(ABS_);STEP;
    }    
	if (!strcmp(msg,SETARRAY_T))
    {
        if (n!=4) 
        {
            bad("SET_A: 4 ARGUMENTS REQUIRED");
            return 0;
        }        
		emit(SETARRAY);STEP;
    }       
    else	
		return 0;
}

//-------------------------------------

void registerhook() 
{
    kwdhook=kwdhook_;
    funhook=funhook_;
}


#endif