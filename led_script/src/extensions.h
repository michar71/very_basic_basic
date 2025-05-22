#ifndef extensions_h
#define extensions_h

#include "main.h"
#include <math.h>

/*
Functions:
=========

Math/Mapping:
LOADMAP(0...x)  (GAMMA,LOG, SIN,PULSE,FIRE_R,FIRE_G,FIRE_B,TEMPERATURE_R,TEMPERATURE_G, TEMPERATUE_B, RAINBOW_R, RAINBOW_G,RAINBOW_B)
SETMAP256(array)
int MAP(int val)
uint8 SCALE256(int val)    
DONE uint8 LIMIT256(int val)
DONE int LIMIT(int val, int min, int max)
DONE int ABS(int)
DONE int SIN256(int)
DONE int SCALE(int val, int valmin, int valmax, int rmin, int rmax)

System:

DONE int TIMESTAMP(int divider)
DONE WAIT (int ms)
LOCKTIME()        -> Stores the current time 
WAITIME(ms)       -> x mS minus the time that has passed since LOCKTIME was called. This allows to have consistent timing even if scripts take some time to execute for each frame.
DONE int RANDOM(int min, int max)
uint8 READANALOG(int ch)
uint8 READPIN(int gpio)
SETPIN(int gpio)
CLEARPIN(int gpio)
WAITEVENT(int event, int cond, int val)  -> Wait for "event" (Analog, Digital, Timer) to (cross low/high, cross high/low) of value

Color Space ->RGBtoHSB and Back (Requires multiple parameters or arrays as parameters? For all these color functions we just ignore values in arrays larger then 3 and set it to zero if less then 3.... ):

int RGBTOH(array[3])
int RGBYOS(array[3])
int RGBTOB(array[3])
int HSBTOR(array[3])
int HSBTOG(array[3])
int HSBTOB(array[3])

HSBTORGBARRAY(array, array)
RGBTOHSBARRAY(array, array)


LED Specific:
DONE SETLED(array R, array G, array B)   -> Upload Array to LED Strip. LED Length is defined by array size. Will init LED Strip on first call. (Usually set all to black...)
CLEARLED(count)  -> Saves us from having to define an array, set the whole array to 0 and then having to send the array....
COLORLED(int start, int end, array[3]) -> Set the whole array to one solid color

Array:
SHIFTARRAY(array A, int amount, int val) -> Shift the values of an array to left or right by a specific amount
DONE ROTATEARRAY(array A, int amount)
DONE SETARRAY(array A, int start,int end, int value) -> Set the value of an array at a specific index
SCALEARRAY(array A, int perc_scale) -> Scale the values of an array by a percentage
DONE SCALELIMITARRAY(array A, int perc,int min, int max) -> Scale the values of an array by a percentage but kimit it to max 255, min 0

//Pack/Unpack Colors:
//For efficiency we allow to store RGB volors in variables and arrays as RGBA of 1 byte each
PACKCOLARRAY(arrayA, array R, array G, Array B)
UNPACKCOLARAY(arrayA, array R, array G, Array B)
int PACKCOL(int R, int G, int B)
int UNPACKCOLR(int RGB)
int UNPACKCOLG(int RGB)
int UNPACKCOLB(int RGB)

LED Specific:
SETLED(array RGB)   -> Upload Array to LED Strip. LED Length is defined by array size. Will init LED Strip on first call. (Usually set all to black...)
DONE SETLEDRGB(array R, array G, array B)
SETLEDCLEAR()       -> Set all LED's to black
COLORLED(int start, int end, int COL) -> Set the whole array to one solid color
DONE int GETMAXLED()
*/



const uint8_t  gamma8[] = {
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2,
2, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5,
5, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10,
10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
90, 92, 93, 95, 96, 98, 99, 101, 102, 104, 105, 107, 109, 110, 112, 114,
115, 117, 119, 120, 122, 124, 126, 127, 129, 131, 133, 135, 137, 138, 140, 142,
144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 167, 169, 171, 173, 175,
177, 180, 182, 184, 186, 189, 191, 193, 196, 198, 200, 203, 205, 208, 210, 213,
215, 218, 220, 223, 225, 228, 231, 233, 236, 239, 241, 244, 247, 249, 252, 255
};

//------------------------------------
//USER DEFINED FUNCTIONS
//------------------------------------
#define PRINTS_T "PRINTS"

#define LIMIT256_T "LIMIT256"
#define LIMIT_T "LIMIT"
#define SCALE_T "SCALE"
#define SIN256_T "SIN256"


#define ABS_T "ABS"
#define WAIT_T "WAIT"
#define RANDOM_T "RANDOM"
#define TIMESTAMP_T "TIMESTAMP"
#define GETMAXLED_T "GETMAXLED"
#define SETLEDRGB_T "SETLEDRGB"

#define SETARRAY_T "SETARRAY"
#define SHIFTARRAY_T "SHIFTARRAY"
#define ROTATEARRAY_T "ROTATEARRAY"
#define SCALEARRAY_T "SCALEARRAY"
#define SCALELIMITARRAY_T "SCALELIMITARRAY"

void dumpStack(void)
{
    Serial.print("SP:");
    Serial.println(sp-stk);
    for (int ii=STKSZ;ii>=0;ii--)
    {
        Serial.print(ii);
        Serial.print(":");
        Serial.println(stk[ii]);
    }
}

int PRINTS_() 
{ 
    //Output the string on the Run-time Stack and return for next command.
	puts((char*)*sp++); 
    STEP; 
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
    int val = *sp;  //Pull value from Stack and rewind stack
    if (val<0) 
        val = -val; //Make it positive
    *sp=val; //Push back to to the stack
    STEP;
}

int WAIT_() 
{ 
    int val = *sp;  //Pull value from Stack and rewind stack
    if (val<0)
        val = 0;

    long start = millis();
    do
    {
        yield();    
    } 
    while (millis() < (start + val));
     
    *sp=0; //Push back to to the stack
    STEP;
}

int GETMAXLED_()
{
    int val = *sp;  //Pull value from Stack (We don't need it but functions need to pass at least one param?)
    if (val<0)
        val = 0;
    
    *sp=NUM_LEDS; //Push back to to the stack
    STEP;
}

int SETLEDRGB_()
{
    //Pull 3 arrays from stack
    Val *arr_b = (Val*)*sp++;  
    Val *arr_g = (Val*)*sp++;  
    Val *arr_r = (Val*)*sp;      

    //Validate arrays
    if ((arr_r == 0) || (arr_g == 0) || (arr_b == 0))
    {
        bad("SETLEDRGB: BAD ARRAY POINTER");
        return 0;
    } 

    if ((arr_r[0] != NUM_LEDS) || (arr_g[0] != NUM_LEDS) || (arr_b[0] != NUM_LEDS))
    {
        bad("SETLEDRGB: WRONG ARRAY LENGTH");
        return 0;
    } 
    //copy arrays to LED array
    for (int ii=0;ii<NUM_LEDS;ii++)
    {
        leds[ii].r = gamma8[(uint8_t)arr_r[ii+1]];
        leds[ii].g = gamma8[(uint8_t)arr_g[ii+1]];
        leds[ii].b = gamma8[(uint8_t)arr_b[ii+1]];                
    }
    //Show LED 
    FastLED.show();

    //Hmm... How do we deal with no return??? Just return a dummy value?
    *sp = 0; //Push 0 to the stack
    STEP;    
}

//Hmm.. Do we scale from the whole 4 bit integer? Thats a huge number space.... 
//Or do we asume the numbers are limited to 2 bytes? (0..65535)
int SCALE_() 
{ 
    int rmax = (int)*sp++;    
    int rmin = (int)*sp++;
    int valmax = (int)*sp++;
    int valmin = (int)*sp++;    
    int val = *sp;  //Pull value from Stack
    int res = map(val,valmin, valmax,rmin,rmax);
    *sp=val; //Push back to the stack
    STEP;
}

int LIMIT256_() 
{ 
    int val = *sp;  //Pull value from Stack
    if (val<0)        //Limit it to 9..255
        val = 0; 
    if (val>255)
        val = 255; 
    *sp=val; //Push result to the stack
    STEP;
}

int SIN256_()
{
    int val = *sp;  //Pull value from Stack
    double sinval = sin(((double)val/255.0)* 2 * PI);
    double scaledval = ((sinval + 1) / 2) * 255;
    *sp=(int)scaledval; //Push result to the stack
    STEP; 
}

int ROTATEARRAY_()
{
    //Pull the values off the stack
    int amount = (int)*sp++;
    Val *arr = (Val*)*sp;  

    if (arr == 0)
    {
        bad("SETARRAY: BAD ARRAY POINTER");
        return 0;
    }

    int size = (int)arr[0];

    //We do this in an annoyingly slow way by just repeating it n times
    //There are faster ways to do this but that requires more buffer memeory
    //or memeory allocation. So we keep it simple for the moment.
    for (int ii=0;ii<abs(amount);ii++)
    {
        int tmp=0;
        if (amount>0)
        {
            tmp = arr[size];
            for (int jj=size;jj>1;jj--)
                arr[jj] = arr[jj-1];
            arr[1] = tmp;    
        }
        else
        {
            tmp = arr[1];
            for (int jj=1;jj<size-1;jj++)
                arr[jj] = arr[jj+1];
            arr[size] = tmp;    
        }
    }

    //Hmm... How do we deal with no return??? Just return a dummy value?
    *sp = 0; //Push 0 to the stack
    STEP;
}

int SETARRAY_()
{ 
    //Pull the values off the stack
    int val = (int)*sp++;
    int end = (int)*sp++;
    int start = (int)*sp++;
    Val *arr = (Val*)*sp;  

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
        arr[i] = val; //Set value in array


    //Hmm... How do we deal with no return??? Just return a dummy value?
    *sp = 0; //Push 0 to the stack
    STEP;
}

int SCALELIMITARRAY_()
{
   //Pull the values off the stack
    int max = (int)*sp++;
    int min = (int)*sp++;
    int perc = (int)*sp++;
    Val *arr = (Val*)*sp;  

    if (arr == 0)
    {
        bad("SETARRAY: BAD ARRAY POINTER");
        return 0;
    }

    int size = (int)arr[0];

    //Set Array
    int i;
    for (i=1; i<=size; i++)
    {
        arr[i] = (int)((float)arr[i]*((float)perc/100.0)); //We cast to int without rounding so stuff converges to 0
        if (arr[i] < min)
            arr[i] = min;
        if (arr[i] > max)
            arr[i] = max;    
    }
    //Hmm... How do we deal with no return??? Just return a dummy value?
    *sp = 0; //Push 0 to the stack
    STEP;
}

int RANDOM_()
{
    int max = (int)*sp++;
    int min = (int)*sp;  //Pull value from Stack and rewind stack
    int rnd = (int)random(min,max);
    *sp=rnd; //Push back to to the stack
    STEP;
}

int LIMIT_()
{
    int max = (int)*sp++;
    int min = (int)*sp++;
    int val = (int)*sp;  //Pull value from Stack and rewind stack
    if (val>max)
        val = max;
    else if (val < min) 
        val = min;   
    *sp=val; //Push back to to the stack
    STEP;
}

int TIMESTAMP_()
{
    int div = (int)*sp;
    int ts = (int)(millis()/div);
    *sp=ts; //Push back to to the stack
    STEP;
}

//Hmmm... so we get the name of the function and the number of arguments.
//Validate here if the number of arguments is correct and then push the function to the progam buffer.
//In the function we pull the arguments off the dstasck and put the result back on the stack.
int funhook_(char *msg, int n) 
{   
	if (!strcmp(msg,LIMIT256_T))
    {
        if (n!=1) 
        {
            bad("LIMIT256: 1 ARGUMENT REQUIRED");
            return 0;
        }
        emit(LIMIT256_);STEP;
    }
	if (!strcmp(msg,SCALE_T))
    {
        if (n!=5) 
        {
            bad("SCALE: 5 ARGUMENTS REQUIRED");
            return 0;
        }
        emit(SCALE_);STEP;
    }    
	if (!strcmp(msg,LIMIT_T))
    {
        if (n!=3) 
        {
            bad("LIMIT: 3 ARGUMENTS REQUIRED");
            return 0;
        }
        emit(LIMIT_);STEP;
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
	if (!strcmp(msg,SIN256_T))
    {
        if (n!=1) 
        {
            bad("ABS: 1 ARGUMENT REQUIRED");
            return 0;
        }
        emit(SIN256_);STEP;
    }        
	if (!strcmp(msg,SETARRAY_T))
    {
        if (n!=4) 
        {
            bad("SETARRAY: 4 ARGUMENTS REQUIRED");
            return 0;
        }        
		emit(SETARRAY_);STEP;
    }     
	if (!strcmp(msg,ROTATEARRAY_T))
    {
        if (n!=2) 
        {
            bad("ROTATEARRAY: 2 ARGUMENTS REQUIRED");
            return 0;
        }        
		emit(ROTATEARRAY_);STEP;
    }         
	if (!strcmp(msg,GETMAXLED_T))
    {
        if (n!=1) 
        {
            bad("GETMAXLED: 1 ARGUMENT REQUIRED");
            return 0;
        }        
		emit(GETMAXLED_);STEP;
    }  
	if (!strcmp(msg,WAIT_T))
    {
        if (n!=1) 
        {
            bad("WAIT: 1 ARGUMENT REQUIRED");
            return 0;
        }        
		emit(WAIT_);STEP;
    }  
	if (!strcmp(msg,SETLEDRGB_T))
    {
        if (n!=3) 
        {
            bad("SETLEDRGB: 3 ARGUMENTS REQUIRED");
            return 0;
        }        
		emit(SETLEDRGB_);STEP;
    }    
 	if (!strcmp(msg,TIMESTAMP_T))
    {
        if (n!=1) 
        {
            bad("TIMESTAMP: 1 ARGUMENT REQUIRED");
            return 0;
        }        
		emit(TIMESTAMP_);STEP;
    }      
 	if (!strcmp(msg,RANDOM_T))
    {
        if (n!=2) 
        {
            bad("RANDOM: 2 ARGUMENTS REQUIRED");
            return 0;
        }        
		emit(RANDOM_);STEP;
    }      
 	if (!strcmp(msg,SCALELIMITARRAY_T))
    {
        if (n!=4) 
        {
            bad("TIMESTAMP: 4 ARGUMENTS REQUIRED");
            return 0;
        }        
		emit(SCALELIMITARRAY_);STEP;
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