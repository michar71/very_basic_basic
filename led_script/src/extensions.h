#ifndef extensions_h
#define extensions_h


#include "main.h"
#include <math.h>

/*
Functions:
=========

General:
DONE int PRINTS(char *msg) -> Print a string to the console  
DONE uint8 LIMIT256(int val) -> Limits a valure oy 0..255
DONE int LIMIT(int val, int min, int max) -> Limits a valuer to given min/max
DONE int ABS(int) -> Absoluter of value
DONE int SIN256(int) -> Returns the sine of a value in 0..255 (0=0, 64=90, 128=180, 192=270, 255=360)
DONE int SCALE(int val, int valmin, int valmax, int rmin, int rmax) -> Scales a value from one range to another
DONE int GAMMA256(int val) -> Same lookup based Gamma function  we apply before setting the LEDS
DONE int USEGAMMA(int enable) -> Automatically apply Gamma correction to all LED values 0 = Disable, 1 = Enable

System:
DONE int TIMESTAMP(int divider)
DONE WAIT (int ms)
LOCKTIME()        -> Stores the current time 
WAITIME(ms)       -> x mS minus the time that has passed since LOCKTIME was called. This allows to have consistent timing/delays even if scripts take some (even variable) time to execute for each frame.
DONE int RANDOM(int min, int max)

Hardware:
uint8 READANALOG(int ch)
uint8 READPIN(int gpio)
SETPIN(int gpio)
CLEARPIN(int gpio)
int CHECKEVENT(int event, int cond, int val)  -> Wait for "event" (Analog, Digital, Timer, Sync Pulse) to (cross low/high, cross high/low, larger, smaller, equal, happened) of value (abs. value, count)

Color Space: 
DONE HSVTORGBARRAY(array_h, array_s, array_v) -> Convert HSV values in an array to RGB values in an array
DONE HSVTORGBARRAY(array_h, array_s, array_v) -> Convert HSV values in an array to RGB values in an array

LED Specific:
DONE SETLEDRGB(array R, array G, array B)   -> Upload Array to LED Strip. LED Length is defined by array size. Will init LED Strip on first call. (Usually set all to black...)
DONE SETLEDCOL(uint8_t r, uint8_t g, uint8_t b) -> Set the whole LED Strip to one solid color.
DONE int GETMAXLED() -> Get the number of LEDs in Strip

Array:
DONE SHIFTARRAY(array A, int amount, int val) -> Shift the values of an array to left or right by a specific amount setting the new pixels to val
DONE ROTATEARRAY(array A, int amount)
DONE COPYARRAY(array A, array B) -> Copy the content of array A to array B. If B is smaller then A it will be truncated, if larger it will be padded with 0.
DONE RGBTOHSVARRAY(array R, array G, array B) -> Convert RGB values in an array to HSV values in an array
DONE HSVTORGBARRAY(array H, array S, array V) -> Convert HSV values in an array to RGB values in an array
DONE SETARRAY(array A, int start,int end, int value) -> Set the value of an array at a specific index
DONE SCALELIMITARRAY(array A, int perc,int min, int max) -> Scale the values of an array by a percentage but kimit it to max 255, min 0

LUTs:
There is 1 LUT in thre system. (Fast Acceess, dynamically allocated/released memory). But LUTs can be copied to/from arrays (Which are slow and eat up basic ressources...).

DONE int LOADLUT(index) -> Loads a lut form filesystem. (LUT_[index].csv so for example LUT_1.csv for index 1). Returns the number of entries or zero if failed. 
DONE int SAVELUT(index) -> Saves ther LUT to the filesystem. (LUT_[index].csv so for example LUT_1.csv for index 1). Returns 1 on success, 0 on failure
DONE int LUTSIZE(index) -> Returns the size of the LUT in entries or 0 if it does not exist. Actually openes the file and reades it byte by byte to find commas unless its the current index lut already loaded so this can be used to DIM an array.
DONE int LUTTOARRAY(array) -> Copies LUT to array. If LUT is larger then array it gets truncated, if bigger its filled with zeros. Returns numbers of entries.
DONE int ARRAYTOLUT(array) -> Copies array to LUT. 
DONE int LUT(int)  -> Returns the value of the LUT at index. If no LUT is loaded it will return 0. If the index is larger then the LUT size it will return 0.

Location Based Functions. All Distances in meters, angles in Degrees:
int HASORIGIN(0)   //Origin Data is avaliable
int HASGPS(0) //Spped/Dir are avialable
int ORIGINXDIST(0)
int ORIGINYDIST(0)
int ORIGINDIST(0)
int ORIGINANGLE(0)
int GPSSPEED(0)    //in m/s
int GPSDIR(0) Angle

IMU Basaed Functions:
int HASGYRO(0) //IMU Data is avaliable
int HASACC(0) //IMU Data is avaliable
int HASMAG(0)  
int PITCH(0) //Pitch in degrees
int ROLL(0) //Roll in degrees
int YAW(0) //Yaw in degrees
int ACCX(0) //Accelerometer X in m/s^2
int ACCY(0) //Accelerometer Y in m/s^2
int ACCZ(0) //Accelerometer Z in m/s^2
int MAGDIR(0) //Magnetometer Direction in degrees 

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
#define GAMMA256_T "GAMMA256"
#define USEGAMMA_T "USEGAMMA"


#define ABS_T "ABS"
#define WAIT_T "WAIT"
#define RANDOM_T "RANDOM"
#define TIMESTAMP_T "TIMESTAMP"
#define GETMAXLED_T "GETMAXLED"
#define SETLEDRGB_T "SETLEDRGB"
#define SETLEDCOL_T "SETLEDCOL"

#define SETARRAY_T "SETARRAY"
#define SHIFTARRAY_T "SHIFTARRAY"
#define ROTATEARRAY_T "ROTATEARRAY"
#define COPYARRAY_T "COPYARRAY"
#define SCALELIMITARRAY_T "SCALELIMITARRAY"
#define RGBTOHSVARRAY_T "RGBTOHSVARRAY"
#define HSVTORGBARRAY_T "HSVTORGBARRAY"
#define LOADLUT_T "LOADLUT"
#define SAVELUT_T "SAVELUT"
#define LUTSIZE_T "LUTSIZE"
#define LUTTOARRAY_T "LUTTOARRAY"
#define ARRAYTOLUT_T "ARRAYTOLUT"
#define LUT_T "LUT"

//-------------------------------------
//Real HW dependecies.... We can ifdef this with stubs or PC functions for testing on other platform
//-------------------------------------
#ifdef REAL_ESP32_HW

#include "FastLED.h"

int getNumLeds()
{
    return NUM_LEDS; //Return the number of LEDs in the strip
}

void setLEDr(int pos, int val)
{
    leds[pos].r = (uint8_t)val;
}

void setLEDg(int pos, int val)
{
    leds[pos].g = (uint8_t)val;
}

void setLEDb(int pos, int val)
{
    leds[pos].b = (uint8_t)val;
}

void updateLEDs()
{
    FastLED.show(); //Update the LED strip with the new values
}

unsigned long getTimestamp()
{
    return millis(); //Return the current timestamp in milliseconds
}

#else
int getNumLeds()
{
    return 0; //Return the number of LEDs in the strip
}

void setLEDr(int pos, int val)
{
}

void setLEDg(int pos, int val)
{
}

void setLEDb(int pos, int val)
{
}

void updateLEDs()
{
    //Update the LED strip with the new values
}

unsigned long getTimestamp()
{
    return 0; //Return the current timestamp in milliseconds
}
#endif
//-------------------------------------
//LUTs
//-------------------------------------
int* pLUT = NULL;
int lutSize = 0;
int currentLUTIndex = -1; //-1 means no LUT loaded

bool useGamma = false;

//Try to open LUT and check number of elements
//Returns number of elements
//0 = Failed to count elements
//-1 = LUT does not exists
int checkLut(uint8_t index)
{
    //Try to open the file
    String filename = String("/LUT_") + String(index) + ".csv";
    File file = FSLINK.open(filename.c_str(), FILE_READ);
    if (!file)
    {
        Serial.printf("LUT %d does not exists\n", index);
        return -1; //LUT does not exists
    }
    int count = 0;
    char c;
    while (file.available())
    {
        c = file.read();
        if (c == ',')
            count++;
    }
    file.close();
    if (count == 0)
    {
        Serial.printf("LUT %d is empty\n", index);
        return 0; //LUT is empty
    }
    return count + 1; //Return number of elements
}

int loadLut(uint8_t index)
{
    //Check if we already have a LUT loaded
    if (currentLUTIndex == index)
    {
        return lutSize; //Return size of current LUT
    }

    //Check if the LUT exists and get the size
    int size = checkLut(index);
    if (size < 0)
        return 0; //LUT does not exists or is empty

    //Allocate memory for the LUT
    if (pLUT != NULL)
        free(pLUT); //Free previous LUT memory

    pLUT = (int*)calloc(size, sizeof(int));
    if (pLUT == NULL)
    {
        return 0; //Memory allocation failed
    }

    //Open the file and read the values into the LUT
    String filename = String("/LUT_") + String(index) + ".csv";
    File file = FSLINK.open(filename.c_str(), FILE_READ);
    if (!file)
    {
        free(pLUT);
        pLUT = NULL;
        return 0; //Failed to open file
    }

    int i = 0;
    String value;
    while (file.available())
    {
        char c = file.read();
        if (c == ',')
        {
            pLUT[i++] = value.toInt();
            value = ""; //Reset value for next read
        }
        else
        {
            value += c; //Append character to value
        }
    }
    
    //Read last value (after last comma)
    if (value.length() > 0 && i < size)
        pLUT[i++] = value.toInt();

    file.close();
    
    lutSize = i; //Set the size of the LUT
    currentLUTIndex = index; //Set current LUT index
    return lutSize; //Return size of loaded LUT
}
int saveLut(uint8_t index)
{
    //Check if we have a LUT loaded
    if (pLUT == NULL || lutSize <= 0)
    {
        return 0; //No LUT to save
    }

    //Open the file for writing
    String filename = String("/LUT_") + String(index) + ".csv";
    File file = FSLINK.open(filename.c_str(), FILE_WRITE);
    if (!file)
    {
        return 0; //Failed to open file
    }

    //Write the values to the file
    for (int i = 0; i < lutSize; i++)
    {
        file.print(pLUT[i]);
        if (i < lutSize - 1)
            file.print(","); //Add comma between values
    }
    
    file.close(); 
    return 1; //Success
}   

int LUT_()
{
    int val = *sp;  //Pull value from Stack
    if (val<0) 
    {
        bad((char*)"LUT: NEGATIVE INDEX");
        return 0;
    }
    if (pLUT == NULL || currentLUTIndex < 0)
    {
        bad((char*)"LUT: NO LUT LOADED");
        return 0; //No LUT loaded
    }
    if (val >= lutSize)
    {
        bad((char*)"LUT: INDEX OUT OF BOUNDS");
        return 0; //Index out of bounds
    }
    *sp = pLUT[val]; //Push the value from the LUT to the stack
    STEP;
}


int LOADLUT_()
{
    int index = *sp;  //Pull value from Stack
    if (index<0) 
    {
        bad((char*)"LOADLUT: NEGATIVE INDEX");
        return 0;
    }
    int size = loadLut(index);
    if (size <= 0)
    {
        bad((char*)"LOADLUT: FAILED TO LOAD LUT");
        return 0; //Failed to load LUT
    }
    *sp = size; //Push the size of the LUT to the stack
    STEP;
}

int SAVELUT_()
{
    int index = *sp;  //Pull value from Stack
    if (index<0) 
    {
        bad((char*)"SAVELUT: NEGATIVE INDEX");
        return 0;
    }
    int result = saveLut(index);
    if (result <= 0)
    {
        bad((char*)"SAVELUT: FAILED TO SAVE LUT");
        return 0; //Failed to save LUT
    }
    *sp = result; //Push the result to the stack
    STEP;
}   

int LUTTOARRAY_()
{
    Val *arr = (Val*)*sp;  //Pull array from stack
    //Validate array
    if (arr == 0)
    {
        bad((char*)"LUTTOARRAY: BAD ARRAY POINTER");
        return 0;
    } 

    if (pLUT == NULL || currentLUTIndex < 0)
    {
        bad((char*)"LUTTOARRAY: NO LUT LOADED");
        return 0; //No LUT loaded
    }

    //Copy LUT to array
    int size = lutSize < arr[0] ? lutSize : arr[0]; //Limit to array size
    for (int ii=1; ii<=size; ii++)
    {
        arr[ii] = pLUT[ii-1]; //Copy LUT value to array
    }
    
    arr[0] = size; //Set the first element to the size of the LUT

    *sp = 0; //Push 0 to the stack
    STEP;    
}   

int ARRAYTOLUT_()
{
    Val *arr = (Val*)*sp;      //Pull array from stack
    //Validate array
    if (arr == 0)
    {
        bad((char*)"ARRAYTOLUT: BAD ARRAY POINTER");
        return 0;
    }

    if (pLUT != NULL)
        free(pLUT); 

    pLUT = (int*)calloc(arr[0], sizeof(int)); //Allocate memory for LUT
    if (pLUT == NULL)
    {
        bad((char*)"ARRAYTOLUT: MEMORY ALLOCATION FAILED");
        return 0; //Memory allocation failed
    }
    lutSize = arr[0]; //Set the size of the LUT
    currentLUTIndex = -1; //No index set yet
    
    //Copy array to LUT
    int size = arr[0] < lutSize ? arr[0] : lutSize; //Limit to LUT size
    for (int ii=1; ii<=size; ii++)
    {
        pLUT[ii-1] = arr[ii]; //Copy array value to LUT
    }

    *sp = 0; //Push 0 to the stack to indicate success
    STEP;    
}

int LUTSIZE_()
{
    int index = *sp;  //Pull value from Stack and rewind stack
    if (index<0) 
    {
        bad((char*)"LUTSIZE: NEGATIVE INDEX");
        return 0;
    }
    //If we have a LUT loaded just return the size of the current LUT
    if (pLUT != NULL && currentLUTIndex == index)
    {
        *sp = lutSize; //If we already have the LUT loaded, return its size
        STEP;
    }

    //If not check the LUT on FS
    int size = checkLut(index);
    if (size < 0)
    {
        bad((char*)"LUTSIZE: LUT DOES NOT EXISTS");
        return 0; //LUT does not exists
    }
    *sp = size; //Push the size of the LUT to the stack
    STEP;
}   

//------------------------------------
//General Commands
//------------------------------------


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

    unsigned long start = getTimestamp();
    do
    {
        yield();    
    } 
    while (getTimestamp() < (start + val));
     
    *sp=0; //Push back to to the stack
    STEP;
}

int GETMAXLED_()
{
    int val = *sp;  //Pull value from Stack (We don't need it but functions need to pass at least one param?)
    if (val<0)
        val = 0;
    
    *sp=getNumLeds(); //Push back to to the stack
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
        bad((char*)"SETLEDRGB: BAD ARRAY POINTER");
        return 0;
    } 

    if ((arr_r[0] != getNumLeds()) || (arr_g[0] != getNumLeds()) || (arr_b[0] != getNumLeds()))
    {
        bad((char*)"SETLEDRGB: WRONG ARRAY LENGTH");
        return 0;
    } 
    //copy arrays to LED array
    for (int ii=0;ii<getNumLeds();ii++)
    { 
        if (useGamma)
        {
            setLEDr(ii, gamma8[arr_r[ii+1]]);
            setLEDg(ii, gamma8[arr_g[ii+1]]);
            setLEDb(ii, gamma8[arr_b[ii+1]]);
        }
        else
        {
            setLEDr(ii, arr_r[ii+1]);
            setLEDg(ii, arr_g[ii+1]);
            setLEDb(ii, arr_b[ii+1]);       
        }                
    }
    //Show LED 
    updateLEDs();

    //Hmm... How do we deal with no return??? Just return a dummy value?
    *sp = 0; //Push 0 to the stack
    STEP;    
}

int RGBTOHSVARRAY_()
{
    //Pull 3 arrays from stack
    Val *arr_b = (Val*)*sp++;  
    Val *arr_g = (Val*)*sp++;  
    Val *arr_r = (Val*)*sp;      

    //Validate arrays
    if ((arr_r == 0) || (arr_g == 0) || (arr_b == 0))
    {
        bad((char*)"RGBTOHSVARRAY: BAD ARRAY POINTER");
        return 0;
    } 

    if ((arr_r[0] != arr_g[0]) || (arr_g[0] != arr_b[0]) || (arr_b[0] != arr_r[0]))
    {
        bad((char*)"RGBTOHSVARRAY: ARRAY LENGTH NOT MATCHING");
        return 0;
    } 


    //copy arrays to LED array
    CRGB rgb;
    CHSV hsv;
    for (int ii=1;ii<=arr_r[0];ii++)
    {
        rgb.r = arr_r[ii];
        rgb.g = arr_g[ii];
        rgb.b = arr_b[ii];

        hsv2rgb_rainbow( hsv, rgb);  //Convert RGB to HSV
        
        arr_r[ii] = hsv.h; //Store Hue
        arr_g[ii] = hsv.s; //Store Saturation
        arr_b[ii] = hsv.v; //Store Brightness             
    }

    //Hmm... How do we deal with no return??? Just return a dummy value?
    *sp = 0; //Push 0 to the stack
    STEP;    
}

int HSVTORGBARRAY_()
{
    //Pull 3 arrays from stack
    Val *arr_b = (Val*)*sp++;  
    Val *arr_g = (Val*)*sp++;  
    Val *arr_r = (Val*)*sp;      

    //Validate arrays
    if ((arr_r == 0) || (arr_g == 0) || (arr_b == 0))
    {
        bad((char*)"HSVTORGBARRAY: BAD ARRAY POINTER");
        return 0;
    } 

    if ((arr_r[0] != arr_g[0]) || (arr_g[0] != arr_b[0]) || (arr_b[0] != arr_r[0]))
    {
        bad((char*)"HSVTORGBARRAY: ARRAY LENGTH NOT MATCHING");
        return 0;
    } 


    //copy arrays to LED array
    CRGB rgb;
    CHSV hsv;
    for (int ii=1;ii<=arr_r[0];ii++)
    {
        hsv.h = arr_r[ii];
        hsv.s = arr_g[ii];
        hsv.v = arr_b[ii];

        rgb = rgb2hsv_approximate( hsv);  //Convert HSV to RGB
        
        arr_r[ii] = rgb.r; //Store Hue
        arr_g[ii] = rgb.g; //Store Saturation
        arr_b[ii] = rgb.b; //Store Brightness             
    }

    //Hmm... How do we deal with no return??? Just return a dummy value?
    *sp = 0; //Push 0 to the stack
    STEP;    
}

int SETLEDCOL_()
{
    //Pull 3 colors from stack
    int b = (int)*sp++;  
    int g = (int)*sp++;  
    int r = (int)*sp;      

    r = constrain(r,0,255); //Limit to 0..255
    g = constrain(g,0,255); //Limit to 0..255
    b = constrain(b,0,255); //Limit to 0..255

    //copy arrays to LED array
    for (int ii=0;ii<getNumLeds();ii++)
    {
        if (useGamma)
        {
            setLEDr(ii, gamma8[r]);
            setLEDg(ii, gamma8[g]);
            setLEDb(ii, gamma8[b]);
        }
        else
        {
            setLEDr(ii, r);
            setLEDg(ii, g);
            setLEDb(ii, b);
        }             
    }
    //Show LED 
    updateLEDs();

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

int GAMMA256_()
{
    int val = *sp;  //Pull value from Stack
    *sp=(int)gamma8[val]; //Push result to the stack
    STEP; 
}

int USEGAMMA_()
{
    int val = *sp;  //Pull value from Stack
    if (val == 0)
        useGamma = false;
    else
        useGamma = true;
    *sp= 0;
    STEP; 
}

int COPYARRAY_()
{
    //Pull 2 arrays from stack
    Val *arr_b = (Val*)*sp++;  
    Val *arr_a = (Val*)*sp;   
    
    int size_a = (int)arr_a[0];
    int size_b = (int)arr_b[0];

    //copy arrays to LED array
    for (int ii=1;ii<=size_a;ii++)
    {
        if (ii>size_b) //If we are larger then the target array we stop
            break;
        arr_b[ii] = arr_a[ii]; //Copy value from A to B
    }

    if (size_b > size_a) //If we are larger then the source array we fill the rest with 0
    {
        for (int ii=size_a+1;ii<=size_b;ii++)
        {
            arr_b[ii] = 0; //Fill with 0
        }
    }

    //Hmm... How do we deal with no return??? Just return a dummy value?
    *sp = 0; //Push 0 to the stack
    STEP;    
}

int ROTATEARRAY_()
{
    //Pull the values off the stack
    int amount = (int)*sp++;
    Val *arr = (Val*)*sp;  

    if (arr == 0)
    {
        bad((char*)"SETARRAY: BAD ARRAY POINTER");
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

int SHIFTARRAY_()
{
    //Pull the values off the stack
    int val = (int)*sp++;
    int amount = (int)*sp++;
    Val *arr = (Val*)*sp;  

    if (arr == 0)
    {
        bad((char*)"SETARRAY: BAD ARRAY POINTER");
        return 0;
    }

    int size = (int)arr[0];

    //Check if we shift more then the array size
    if (abs(amount) > size)
    {
        bad((char*)"SHIFTARRAY: SHIFT AMOUNT LARGER THEN ARRAY SIZE");
        return 0;
    }   

    int ii =0;
    if (amount>0)
    {
        for (ii= size-amount; ii>0; ii--)
        {
            arr[ii+amount] = arr[ii]; //Shift right
        }

        for (ii=1;ii<=amount;ii++)
        {
            arr[ii] = val; //Set new values
        }
    }
    else if (amount<0)
    {
        for (ii=1; ii<=size+amount; ii++)
        {
            arr[ii] = arr[ii-amount]; //Shift left
        }

        for (ii=size+amount+1;ii<=size;ii++)
        {
            arr[ii] = val; //Set new values
        }
    }
    else
    {
        //If amount is 0 we do nothing
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
        bad((char*)"SETARRAY: BAD ARRAY POINTER");
        return 0;
    }

    int size = (int)arr[0];

    //Check if the array indices are valid
    if (start<1 || end<1 || start>end || end>size)
    {
        bad((char*)"SETARRAY: INDEX OUT OF BOUNDS");
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
        bad((char*)"SCALELIMITARRAY: BAD ARRAY POINTER");
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
    int ts = (int)(getTimestamp()/div);
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
            bad((char*)"LIMIT256: 1 ARGUMENT REQUIRED");
            return 0;
        }
        emit(LIMIT256_);STEP;
    }
	if (!strcmp(msg,SCALE_T))
    {
        if (n!=5) 
        {
            bad((char*)"SCALE: 5 ARGUMENTS REQUIRED");
            return 0;
        }
        emit(SCALE_);STEP;
    }    
	if (!strcmp(msg,LIMIT_T))
    {
        if (n!=3) 
        {
            bad((char*)"LIMIT: 3 ARGUMENTS REQUIRED");
            return 0;
        }
        emit(LIMIT_);STEP;
    }    
	if (!strcmp(msg,ABS_T))
    {
        if (n!=1) 
        {
            bad((char*)"ABS: 1 ARGUMENT REQUIRED");
            return 0;
        }
        emit(ABS_);STEP;
    }    
	if (!strcmp(msg,SIN256_T))
    {
        if (n!=1) 
        {
            bad((char*)"ABS: 1 ARGUMENT REQUIRED");
            return 0;
        }
        emit(SIN256_);STEP;
    }        
	if (!strcmp(msg,SETARRAY_T))
    {
        if (n!=4) 
        {
            bad((char*)"SETARRAY: 4 ARGUMENTS REQUIRED");
            return 0;
        }        
		emit(SETARRAY_);STEP;
    }     
	if (!strcmp(msg,ROTATEARRAY_T))
    {
        if (n!=2) 
        {
            bad((char*)"ROTATEARRAY: 2 ARGUMENTS REQUIRED");
            return 0;
        }        
		emit(ROTATEARRAY_);STEP;
    }   
	if (!strcmp(msg,SHIFTARRAY_T))
    {
        if (n!=3) 
        {
            bad((char*)"SHIFTARRAY: 3 ARGUMENTS REQUIRED");
            return 0;
        }        
		emit(SHIFTARRAY_);STEP;
    }       
	if (!strcmp(msg,COPYARRAY_T))
    {
        if (n!=2) 
        {
            bad((char*)"COPYARRAY: 2 ARGUMENTS REQUIRED");
            return 0;
        }        
		emit(COPYARRAY_);STEP;
    }              
	if (!strcmp(msg,GETMAXLED_T))
    {
        if (n!=1) 
        {
            bad((char*)"GETMAXLED: 1 ARGUMENT REQUIRED");
            return 0;
        }        
		emit(GETMAXLED_);STEP;
    }  
	if (!strcmp(msg,WAIT_T))
    {
        if (n!=1) 
        {
            bad((char*)"WAIT: 1 ARGUMENT REQUIRED");
            return 0;
        }        
		emit(WAIT_);STEP;
    }  
	if (!strcmp(msg,SETLEDRGB_T))
    {
        if (n!=3) 
        {
            bad((char*)"SETLEDRGB: 3 ARGUMENTS REQUIRED");
            return 0;
        }        
		emit(SETLEDRGB_);STEP;
    }    
    	if (!strcmp(msg,SETLEDCOL_T))
    {
        if (n!=3) 
        {
            bad((char*)"SETLEDCOL: 3 ARGUMENTS REQUIRED");
            return 0;
        }        
		emit(SETLEDCOL_);STEP;
    }  
 	if (!strcmp(msg,TIMESTAMP_T))
    {
        if (n!=1) 
        {
            bad((char*)"TIMESTAMP: 1 ARGUMENT REQUIRED");
            return 0;
        }        
		emit(TIMESTAMP_);STEP;
    }      
 	if (!strcmp(msg,RANDOM_T))
    {
        if (n!=2) 
        {
            bad((char*)"RANDOM: 2 ARGUMENTS REQUIRED");
            return 0;
        }        
		emit(RANDOM_);STEP;
    }      
 	if (!strcmp(msg,SCALELIMITARRAY_T))
    {
        if (n!=4) 
        {
            bad((char*)"SCALELIMITARRAY: 4 ARGUMENTS REQUIRED");
            return 0;
        }        
		emit(SCALELIMITARRAY_);STEP;
    }      
	if (!strcmp(msg,RGBTOHSVARRAY_T))
    {
        if (n!=3) 
        {
            bad((char*)"RGBTOHSVARRAY: 3 ARGUMENTS REQUIRED");
            return 0;
        }        
		emit(RGBTOHSVARRAY_);STEP;
    }   
	if (!strcmp(msg,HSVTORGBARRAY_T))
    {
        if (n!=3) 
        {
            bad((char*)"HSVTORGBARRAY: 3 ARGUMENTS REQUIRED");
            return 0;
        }        
		emit(HSVTORGBARRAY_);STEP;
    }      
    if (!strcmp(msg,LOADLUT_T))
    {
        if (n!=1) 
        {
            bad((char*)"LOADLUT: 1 ARGUMENT REQUIRED");
            return 0;
        }
        emit(LOADLUT_);STEP;
    }
    if (!strcmp(msg,SAVELUT_T))
    {
        if (n!=1) 
        {
            bad((char*)"SAVELUT: 1 ARGUMENT REQUIRED");
            return 0;
        }
        emit(SAVELUT_);STEP;
    }
    if (!strcmp(msg,LUTSIZE_T))
    {
        if (n!=1) 
        {
            bad((char*)"LUTSIZE: 1 ARGUMENT REQUIRED");
            return 0;
        }
        emit(LUTSIZE_);STEP;
    }
    if (!strcmp(msg,LUTTOARRAY_T))
    {
        if (n!=1) 
        {
            bad((char*)"LUTTOARRAY: 1 ARGUMENT REQUIRED");
            return 0;
        }
        emit(LUTTOARRAY_);STEP;
    }
    if (!strcmp(msg,ARRAYTOLUT_T))
    {
        if (n!=1) 
        {
            bad((char*)"ARRAYTOLUT: 1 ARGUMENT REQUIRED");
            return 0;
        }
        emit(ARRAYTOLUT_);STEP;
    }
    if (!strcmp(msg,LUT_T))
    {
        if (n!=1) 
        {
            bad((char*)"LUT: 1 ARGUMENT REQUIRED");
            return 0;
        }
        emit(LUT_);STEP;
    }
    if (!strcmp(msg,USEGAMMA_T))
    {
        if (n!=1) 
        {
            bad((char*)"USEGAMMA: 1 ARGUMENT REQUIRED");
            return 0;
        }
        emit(USEGAMMA_);STEP;
    }    
    if (!strcmp(msg,GAMMA256_T))
    {
        if (n!=1) 
        {
            bad((char*)"GAMMA256: 1 ARGUMENT REQUIRED");
            return 0;
        }
        emit(GAMMA256_);STEP;
    }        
    //If we reach here we did not find a matching function                   
    else	
		return 0;
}

//-------------------------------------
void registerhook() 
{
    kwdhook=kwdhook_;
    funhook=funhook_;
}

//Location Callback provides potentially Latitude, Longitude, Altitude, Speed, Course
void register_location_callback()
{
    //This is called by the main program to register the location callback
    //We don't need to do anything here as we don't have a location callback
}


//IMU Callback provides potentially Roll, Pitch, Yaw, AccX, AccY, AccZ, MagDir 
void register_imu_callback()
{
    //This is called by the main program to register the IMU callback
    //We don't need to do anything here as we don't have an IMU callback
}

#endif