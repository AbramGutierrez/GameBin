

// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>


/***************************************************
Music Player & SD Card: Define & Initialization
 ****************************************************/
// define the pins used
//#define CLK 13       // SPI Clock, shared with SD card
//#define MISO 12      // Input data, from VS1053/SD card
//#define MOSI 11      // Output data, to VS1053/SD card
// Connect CLK, MISO and MOSI to hardware SPI pins.
// See http://arduino.cc/en/Reference/SPI "Connections"

// These are the pins used for the breakout example
#define BREAKOUT_RESET  9      // VS1053 reset pin (output)
#define BREAKOUT_CS     10     // VS1053 chip select pin (output)
#define BREAKOUT_DCS    8      // VS1053 Data/command select pin (output)
// These are the pins used for the music maker shield
#define SHIELD_RESET  -1      // VS1053 reset pin (unused!)
#define SHIELD_CS     7      // VS1053 chip select pin (output)
#define SHIELD_DCS    6      // VS1053 Data/command select pin (output)

// These are common pins between breakout and shield
#define CARDCS 4     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin

Adafruit_VS1053_FilePlayer musicPlayer =
  Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);



/***************************************************
Break-Beam Verifier: Define & Initialization
 ****************************************************/

#define SENSORPIN_1 A1	// Pin A0: tier 1 can
#define SENSORPIN_2 A0	// Pin A1: tier 2 can
#define SENSORPIN_3 A3	// Pin A2: tier 1 paper
#define SENSORPIN_4 A2	// Pin A3: tier 2 paper
#define SENSORPIN_5 A5	// Pin A4: tier 1 trash
#define SENSORPIN_6 A4	// Pin A5: tier 2 trash

// initialize variables
int sensor2_1 = 0;
int lastState_1 = 0, lastState_2_1 = 0;

int sensor2_2 = 0;
int lastState_2 = 0, lastState_2_2 = 0;

int sensor2_3 = 0;
int lastState_3 = 0, lastState_2_3 = 0;

/***************************************************
LED randomizer: Define & Initialization
 ****************************************************/

// defining LED randomizer PIN
#define LED_1  2
#define LED_2  5
#define LED_3  8

unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 3000;              // interval at which to blink (milliseconds)

// random number
int r = 0;

//constants used to store amount of can/paper/trash
int n1 = 0;
int n2 = 0;
int n3 = 0;

/***************************************************
Communication to Visual Uno: Define & Initialization
 ****************************************************/
#define SENDSIGNAL 0





void setup() {

  Serial.begin(9600); // Initialize the serial monitor

  /********************************************************/
  /********************************************************/
  if (! musicPlayer.begin()) { // initialise the music player
    Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
    while (1);
  }
  Serial.println(F("VS1053 found"));
  // initialise the SD card
  if (!SD.begin(CARDCS)) {
    Serial.println("initialization failed!");
    return;
  }

  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(0, 0);

  // If DREQ is on an interrupt pin (on uno, #2 or #3) we can do background audio playing
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int

  /********************************************************/
  /********************************************************/
  // initialize the sensor pin as an input:
  pinMode(SENSORPIN_1, INPUT);
  pinMode(SENSORPIN_2, INPUT);
  pinMode(SENSORPIN_3, INPUT);
  pinMode(SENSORPIN_4, INPUT);
  pinMode(SENSORPIN_5, INPUT);
  pinMode(SENSORPIN_6, INPUT);
  digitalWrite(SENSORPIN_1, HIGH); // turn on the pullup
  digitalWrite(SENSORPIN_2, HIGH);
  digitalWrite(SENSORPIN_3, HIGH);
  digitalWrite(SENSORPIN_4, HIGH);
  digitalWrite(SENSORPIN_5, HIGH);
  digitalWrite(SENSORPIN_6, HIGH);
  
  
  /********************************************************/
  /********************************************************/
  randomSeed(analogRead(0));
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);

  /********************************************************/
  /********************************************************/
  pinMode(SENDSIGNAL, OUTPUT);
}

void loop() {
  // playMusic();
  // start randomizer
  randomizer();
 
  // verify breaks 
  bool break_can = verifier1(SENSORPIN_1, SENSORPIN_2);
  bool break_paper = verifier2(SENSORPIN_3, SENSORPIN_4);
  bool break_trash = verifier3(SENSORPIN_5, SENSORPIN_6);
  
  // store data according to break;
  if (break_can == true) storeData(1);
  if (break_paper == true) storeData(2);
  if (break_trash == true) storeData(3);
  
  // determine when to play music & send signal to visual
  if (break_can == true && r == 1) {
	playMusic();
	digitalWrite(SENDSIGNAL, HIGH); 
  }
  else if (break_paper == true && r == 2){
	playMusic();
	digitalWrite(SENDSIGNAL, HIGH);  
  } 
  else if (break_trash == true && r == 3) {
	playMusic();
	digitalWrite(SENDSIGNAL, HIGH);  
  } 
  else digitalWrite(SENDSIGNAL, LOW);

  
}

// type 1 = can, type 2 = bottle, type 3 = waste
void storeData(int type) {

  File myFile = SD.open("data.txt", FILE_WRITE);

  if (myFile) {
    if (type == 1) {
      myFile.println("Paper");
    }
    else if (type == 2) {
      myFile.println("Bottle");
    }
    else {
      myFile.println("Trash");
    }
    myFile.close();
  }
  else {
    Serial.println("error opening data.txt");
  }

}

void playMusic() {
	
  int randNumber = random(1, 13);
  
  switch (randNumber){
	case 1:
	  musicPlayer.playFullFile("track001.mp3");
	  break;
	case 2:
	  musicPlayer.playFullFile("track002.mp3");
	  break;
	case 3:
	  musicPlayer.playFullFile("track003.mp3");
	  break;
	case 4:
	  musicPlayer.playFullFile("track004.mp3");
	  break;
	case 5:
	  musicPlayer.playFullFile("track005.mp3");
	  break;  
	case 6:
	  musicPlayer.playFullFile("track006.mp3");
	  break;  
	case 7:
	  musicPlayer.playFullFile("track007.mp3"); 
	  break; 
	case 8:
	  musicPlayer.playFullFile("track008.mp3"); 
	  break; 
	case 9:
	  musicPlayer.playFullFile("track009.mp3");
	  break;
	case 10:
	  musicPlayer.playFullFile("track0010.mp3");
	  break;
	case 11:
	  musicPlayer.playFullFile("track0011.mp3");
	  break;
	case 12:
	  musicPlayer.playFullFile("track0012.mp3");
	  break;

  }
  
}



void randomizer() {

  // if the different between the current time and last time is greater
  // pick a random number between 1 and 10
  // if n <= 4, show LED_1
  // if N >= 7, show LED_2
  // otherwise, show LED_3
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {

    previousMillis = currentMillis;

    int randNumber = random(1, 11);
    if (randNumber <= 4 ) {
      r = 1;
      n1++;
      digitalWrite(LED_1, HIGH);
      digitalWrite(LED_2, LOW);
      digitalWrite(LED_3, LOW);
    }
    else if (randNumber == 5 || randNumber == 6) {
      r = 2;
      n3++;
      digitalWrite(LED_1, LOW);
      digitalWrite(LED_2, HIGH);
      digitalWrite(LED_3, LOW);
    }
    else {
      r = 3;
      n2++;
      digitalWrite(LED_1, LOW);
      digitalWrite(LED_2, LOW);
      digitalWrite(LED_3, HIGH);
    }
    Serial.println("randomNumber: " + randNumber);
    Serial.print(" n1: ");
    Serial.print(n1);
    Serial.print(" n2: ");
    Serial.print(n2);
    Serial.print(" n3: ");
    Serial.println(n3);
  }
}


// 3 verifier functions ----------------------

// return true if item passes verifier THIS IS FOR BOTTLE
bool verifier1(int sensorPin1, int sensorPin2) {

  // read the state of the 2-tier sensor value:
  int sensorState = digitalRead(sensorPin1);
  int sensorState_2 = digitalRead(sensorPin2);

  // check if the sensor beam is broken
  // if it is, the sensorState is LOW:

  if (sensorState && !lastState_1) {
    Serial.println("Unbroken_1");
  }
  if (!sensorState && lastState_1) {
    Serial.println("Broken_1");
  }
  lastState_1 = sensorState;

  // check sensor state 2
  if (sensorState_2 && !lastState_2_1) {
    Serial.println("Unbroken_2");
    //sensor2_1 = 1;
  }
  if (!sensorState_2 && lastState_2_1) {
    Serial.println("Broken_2");
    sensor2_1 = 1;
  }
  lastState_2_1 = sensorState_2;

  //Verifier, tier-1 is on the bottom
  if ( sensorState_2 == HIGH) {
    if ( sensorState == LOW ) {

      if (sensor2_1 == 1) {
        return true;
      }
    }
  }
  return false;
}

// return true if item passes verifier
bool verifier2(int sensorPin1, int sensorPin2) {

  // read the state of the 2-tier sensor value:
  int sensorState = digitalRead(sensorPin1);
  int sensorState_2 = digitalRead(sensorPin2);

  // check if the sensor beam is broken
  // if it is, the sensorState is LOW:

  if (sensorState && !lastState_2_3) {
    Serial.println("Broken_1");
  }
  if (!sensorState && lastState_2_3) {
    Serial.println("Unbroken_1");
  }
  lastState_2_3 = sensorState;

  // check sensor state 2
  if (sensorState_2 && !lastState_2_2) {
    Serial.println("Broken_2");
    sensor2_2 = 1;
  }
  if (!sensorState_2 && lastState_2_2) {
    Serial.println("Unbroken_2");
    //sensor2_2 = 1;
  }
  lastState_2_2 = sensorState_2;

  //Verifier, tier-1 is on the bottom
  if ( sensorState_2 == LOW ) {
    if ( sensorState == HIGH ) {

      if (sensor2_2 == 1) {
        return true;
      }
    }
  }
  return false;
}

// return true if item passes verifier
bool verifier3(int sensorPin1, int sensorPin2) {


  // read the state of the 2-tier sensor value:
  int sensorState = digitalRead(sensorPin1);
  int sensorState_2 = digitalRead(sensorPin2);

  // check if the sensor beam is broken
  // if it is, the sensorState is LOW:

  if (sensorState && !lastState_3) {
    Serial.println("Broken_1");
  }
  if (!sensorState && lastState_3) {
    Serial.println("Unbroken_1");
  }
  lastState_3 = sensorState;

  // check sensor state 2
  if (sensorState_2 && !lastState_2_3) {
    Serial.println("Broken_2");
    sensor2_3 = 1;
  }
  if (!sensorState_2 && lastState_2_3) {
    Serial.println("Unbroken_2");
    //sensor2_3 = 1;
  }
  lastState_2_3 = sensorState_2;

  //Verifier, tier-1 is on the bottom
  if ( sensorState_2 == LOW ) {
    if ( sensorState == HIGH ) {

      if (sensor2_3 == 1) {
        return true;
      }
    }
  }
  return false;
}
