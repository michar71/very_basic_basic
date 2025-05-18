#include <Arduino.h>
#include "FS.h"
#include "SPIFFS.h"
#include <SimpleSerialShell.h>
#include "basic.h"
#include "main.h"




/*
Shell Commands

test (Test command prompt)
dir  (List files)
run (Interacive basic shell)
run file
list file
load file  (Text after this will be safed to file system unitl a "break" character is received. Existing file will be overwritten)
del file 
ren file
*/

/*
SPIFFS
*/

/* You only need to format SPIFFS the first time you run a
   test or else use the SPIFFS plugin to create a partition
   https://github.com/me-no-dev/arduino-esp32fs-plugin */
   #define FORMAT_SPIFFS_IF_FAILED true

void renameFile(fs::FS &fs, const char *path1, const char *path2) 
{
    Serial.printf("Renaming file %s to %s\r\n", path1, path2);
    if (fs.rename(path1, path2)) {
      Serial.println("- file renamed");
    } else {
      Serial.println("- rename failed");
    }
  }
  
  void deleteFile(fs::FS &fs, const char *path) 
  {
    Serial.printf("Deleting file: %s\r\n", path);
    if (fs.remove(path)) {
      Serial.println("- file deleted");
    } else {
      Serial.println("- delete failed");
    }
  }


void listDir(fs::FS &fs, const char *dirname, uint8_t levels) 
{
  Serial.printf("Listing directory: %s\r\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    Serial.println("- failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println(" - not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels) {
        listDir(fs, file.path(), levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("\tSIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

void readFile(fs::FS &fs, const char *path) 
{
    Serial.printf("Listing file: %s\r\n", path);
    Serial.println();
  
    File file = fs.open(path);
    if (!file || file.isDirectory()) 
    {
      Serial.println("- failed to open file for reading");
      return;
    }
  
    while (file.available()) 
    {
      Serial.write(file.read());
    }
    file.close();
  }
  
  void writeFile(fs::FS &fs, const char *path, const char *message) {
    Serial.printf("Writing file: %s\r\n", path);
  
    File file = fs.open(path, FILE_WRITE);
    if (!file) {
      Serial.println("- failed to open file for writing");
      return;
    }
    if (file.print(message)) 
    {
      Serial.println("- file written");
    } 
    else 
    {
      Serial.println("- write failed");
    }
    file.close();
  }

/*
Commands
*/
int test(int argc, char **argv) 
{
  Serial.println("Test function called");
  Serial.print(argc);
  Serial.println(" Arguments");
  for (int ii=0;ii<argc;ii++)
  {
    Serial.print("Argument ");
    Serial.print(ii);
    Serial.print(" : ");
    Serial.println(argv[ii]);
  }  
  return 0;
};

int delFile(int argc, char **argv) 
{
    if (argc != 2)
    {
        Serial.println("Wrong argument count");
        return 1;
    }
    deleteFile(SPIFFS,argv[1]);
    return 0;
}

int renFile(int argc, char **argv) 
{
    if (argc != 3)
    {
        Serial.println("Wrong argument count");
        return 1;
    }
    renameFile(SPIFFS,argv[1], argv[2]);
    return 0;
}

int listFile(int argc, char **argv) 
{
    if (argc != 2)
    {
        Serial.println("Wrong argument count");
        return 1;
    }

    readFile(SPIFFS,argv[1]); 
    Serial.printf("");
    return 0;
}

int listDir(int argc, char **argv) 
{
    if (argc != 1)
    {
        Serial.println("Wrong argument count");
        return 1;
    }
    listDir(SPIFFS,"/",1); 
    return 0;
}

int loadFile(int argc, char **argv) 
{
    if (argc != 2)
    {
        Serial.println("Wrong argument count");
        return 1;       
    }
    else
    {
        int linecount = 0;
        int charcount = 0;
        char line[256];
        char inchar;
        bool isDone = false;
        Serial.print("Ready for file. Press CTRL+Z to end transmission and save file");
        Serial.println(argv[1]);
        //Flush serial buffer
        Serial.flush();
        //create file
        File file = SPIFFS.open(argv[1], FILE_WRITE);
        if (!file) 
        {
            Serial.println("- failed to open file for writing");
            return 1;
        }

        do
        {
            //Get one character from serial port
            if (Serial.available())
            {
                inchar = Serial.read();
                //Check if its a break character
                if (inchar == 0x1A) 
                {
                    //Break loop 
                    break;
                }
                else
                {
                    //Wait for a full line
                    line[charcount] = inchar;
                    charcount++;
                    if (charcount>254)
                    {
                        Serial.print("Line ");
                        Serial.print(linecount+1);
                        Serial.println(" too long");
                        break;
                    }
                    if (inchar == '\n')
                    {
                        //Write line
                        if (file.print(line)) 
                        {
                        } 
                        else 
                        {
                          Serial.println("Write Error");
                          file.close();
                          return 1;
                        }
                        //increase line counter
                        linecount++;
                        //clear line
                        charcount = 0;
                        line[0] = 0;
            
                    }
                }
            }
        }
        while (isDone == false);
        //close file
        file.close();

        Serial.print(linecount);
        Serial.println(" Lines written to file");
        return 0;
    }
}

int runBasic(int argc, char **argv) 
{
    if (argc == 1)
    {
        Serial.println("RUNNING IN INTERACTIVE MODE. Press CTRL+Z, Return to exit.");
        initbasic(1);
        int res = interp(NULL);
        if (res != 0)
        {
            Serial.print("Error Exit Code: ");
            Serial.println(res);
        }   
        else 
            Serial.println("DONE");
        return 0;
        return 0;
    }
    else if (argc == 2)
    {
        Serial.print("RUNNING ");
        Serial.println(argv[1]);
        initbasic(1);        
        int res = interp(argv[1]);
        if (res != 0)
        {
            Serial.print("Error Exit Code: ");
            Serial.println(res);
        }   
        else 
            Serial.println("DONE");
        return 0;
    }
    else
    {
        Serial.println("Wrong argument count");
        return 1;
    }
}

/*
LED Stuff
*/

void blink_leds(CRGB col)
{
    leds[0] = CRGB::Black;
    //Set LED's
    FastLED.show();
    delay(300);
    leds[0] = col;
    //Set LED's
    FastLED.show();
    delay(500);
    leds[0] = CRGB::Black;
    //Set LED's
    FastLED.show();
    delay(300);      
}

void color_leds(CRGB col)
{
    for (int ii=0;ii<NUM_LEDS;ii++)
    {
        leds[ii] = col;    
    }
    FastLED.show();
}

/*
Main Code
*/


void setup() 
{
    // put your setup code here, to run once:

    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(BUTTON_PIN,INPUT_PULLUP);

    FastLED.addLeds<WS2812B, RGB_DATA_PIN, RGB>(leds, NUM_LEDS);
    blink_leds(CRGB::Red);

    Serial.begin(115200);
    
    while (!Serial) 
    {
        // wait for serial port to connect. Needed for native USB port only
        // AND you want to block until there's a connection
        // otherwise the shell can quietly drop output.
    }

    blink_leds(CRGB::Blue);

    //example
    shell.attach(Serial);
    shell.addCommand(F("test"), test);
    shell.addCommand(F("run"), runBasic);
    shell.addCommand(F("dir"), listDir);    
    shell.addCommand(F("list"), listFile);
    shell.addCommand(F("ren"), renFile);
    shell.addCommand(F("del"), delFile);
    shell.addCommand(F("load"), loadFile);        
    
    if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) 
    {
        Serial.println("SPIFFS Mount Failed");
    }

    Serial.println("Ready.");
    blink_leds(CRGB::Green);
}

void loop() 
{
  static bool startup = true;  
  if (startup)
  {
    startup = false;
    if (SPIFFS.exists("/startup.bas"))
    {
        initbasic(1);
        int res = interp("/startup.bas");
        if (res != 0)
        {
            Serial.print("Error Exit Code: ");
            Serial.println(res);
        }   
        else 
            Serial.println("STARTUP DONE");
    }
    else
        Serial.println("No startup.bas found, proceeding to shell");
  }

  shell.executeIfInput();
}

