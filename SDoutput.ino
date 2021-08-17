//This sketch displays the files on a Mayfly Data Logger's microSD card to the serial monitor.
//It was adapted from the Arduino.cc tutorials on the SD Library: https://www.arduino.cc/en/reference/SD
//Remember that file names for the SD Library are limited to 8 characters followed by a period and 3 further characters.
//For example, "mydata.txt" is permissible but "MySensorData.txt" has too many characters and won't be recognized.
//Typically, a user would upload this sketch to the Mayfly Data Logger, read a file from the SD card to the serial monitor,
//and copy/paste the text from the serial monitor into a worksheet, thus saving the step of removing the SD card and reading it
//directly from a drive on a computer.

#include <SPI.h>        //you need to have the SPI library in your Arduino/libraries folder
#include <SD.h>         //you need to have the SD library in your Arduino/libraries folder

File root;              //define the root File on the SD card

void setup()  {         //the setup portion of the sketch 
  Serial.begin(9600);   //open serial communications
  while (!Serial) {
    ;                  // wait for serial port to connect
  }

  Serial.print("Initializing SD card...");            //print this to the serial monitor

  if (!SD.begin(12)) {                                //digital pin 12 is the microSD slave select pin on Mayfly; see if the card is present and can be initialized
    Serial.println("Card failed, or not present");    //if not present, don't do anything more
    while (1);                                        //
  }
  Serial.println("card initialized.");               //print this as a line in the serial monitor
  Serial.println("");                                //print a blank line in the serial monitor

  root = SD.open("/");                               //open the root location on the SD card
  Serial.println("Files on microSD card");           //print this line to the serial monitor
  printDirectory(root, 0);                           //print the directory of files on the SD card
  Serial.println("");                                //print a bank line in the serial monitor
  Serial.println("Displaying file:");                //print this line to the serial monitor

  File dataFile = SD.open("datalog.txt");            //name the file to be opened; the example here is "datalog.txt" but this can be any
  if (dataFile) {                                    //if the file is available, write to it:
    while (dataFile.available()) {                   //
      Serial.write(dataFile.read());                 //read the dataFile named "datalog.txt" (or the 8 character name of the file stored on your SD card)
    }
    dataFile.close();                                //
  }
  else {
    Serial.println("error opening datalog.txt");    // if the file isn't open, print an error to the serial monitor
  }
}
  
void loop() {                                       //there isn't a loop in this sketch, the work has already been done
}

void printDirectory(File dir, int numTabs) {       //this function displays all the files on the SD card to the serial monitor
  while (true) {   

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}
