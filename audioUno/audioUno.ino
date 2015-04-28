/*
AUDIO UNO
Randomizer Modules  + Verifier Modules =  audioUNO
*/

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

#define SENSORPIN_1 A0	// Pin A0: tier 1 can
#define SENSORPIN_2 A1	// Pin A1: tier 2 can
#define SENSORPIN_3 A2	// Pin A2: tier 1 paper
#define SENSORPIN_4 A3	// Pin A3: tier 2 paper
#define SENSORPIN_5 A5	// Pin A4: tier 1 trash
#define SENSORPIN_6 A4	// Pin A5: tier 2 trash

// initialize variables
int sensor2_1 = 0;
int lastState_1 = 0, lastState_2_1 = 0;

int sensor2_2 = 0;
int lastState_2 = 0, lastState_2_2 = 0;

int sensor2_3 = 0;
int lastState_3 = 0, lastState_2_3 = 0;

//int canCount = 0;

/***************************************************
LED randomizer: Define & Initialization
 ****************************************************/

// defining LED randomizer PIN
#define LED_1  2
#define LED_2  5
#define LED_3  8

unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 2500;              // interval at which to blink (milliseconds)

// random number
int r = 0;

//constants used to store amount of can/paper/trash
int n1 = 0;
int n2 = 0;
int n3 = 0;

//Verifier Flag
int canFlag = 0;
int paperFlag = 0;
int trashFlag = 0;

/***************************************************
Communication to Visual Uno: Define & Initialization
 ****************************************************/
#define SENDSIGNAL 10

unsigned long timer;
unsigned long tempTime;
unsigned long oneMin = 60000;
unsigned long minCounter = 0;
int itemsCounter;
bool flag;

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
  musicPlayer.setVolume(30, 30);

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
  
  flag = false
}

void loop() {
  // playMusic();
  // start randomizer
  randomizer();
 
  // verify breaks 
  bool break_can = verifier1(SENSORPIN_1, SENSORPIN_2);
  bool break_paper = verifier2(SENSORPIN_3, SENSORPIN_4);
  bool break_trash = verifier3(SENSORPIN_5, SENSORPIN_6);
  
  //r = 1;
  // determine when to play music & send signal to visual
  if (break_can == true && r == 1) {
	playMusic();
	digitalWrite(SENDSIGNAL, HIGH); 
        itemsCounter++;
  }
  else if (break_paper == true && r == 2){
	playMusic();
	digitalWrite(SENDSIGNAL, HIGH);  
        itemsCounter++;
  } 
  else if (break_trash == true && r == 3) {
	playMusic();
	digitalWrite(SENDSIGNAL, HIGH);  
        itemsCounter++;
  } 
  else digitalWrite(SENDSIGNAL, LOW);

  //Write to SD Card total count, every minute
  timer = millis();
if ((timer > 0) && (flag==false)){
      tempTime = timer;
      flag=true;
      Serial.println("String");
}
  
  if ((timer-tempTime)>=oneMin){
    Serial.println("Writing Stuff into SD card");
    storeValue(itemsCounter);
    timer=0;
    tempTime=0;
  }

}

// Store value onto SD card
void storeValue(int val, int time) {

  File myFile = SD.open("value.txt", FILE_WRITE);

  if (myFile) {
    myFile.println(val);
    myFile.close();
  }
  else {
    Serial.println("error opening value.txt");
  }

}

void playMusic() {
  
  int randNumber = random(1, 31);
  
  switch (randNumber){
	case 1:
          Serial.println(1);
	  musicPlayer.playFullFile("track001.mp3");
	  break;
	case 2:
          Serial.println(2);
	  musicPlayer.playFullFile("track002.mp3");
	  break;
	case 3:
          Serial.println(3);
	  musicPlayer.playFullFile("track003.mp3");
	  break;
	case 4:
          Serial.println(4);
	  musicPlayer.playFullFile("track004.mp3");
	  break;
	case 5:
          Serial.println(5);
	  musicPlayer.playFullFile("track005.mp3");
	  break;  
	case 6:
          Serial.println(6);
	  musicPlayer.playFullFile("track006.mp3");
	  break;  
	case 7:
          Serial.println(7);
	  musicPlayer.playFullFile("track007.mp3"); 
	  break; 
	case 8:
          Serial.println(8);
	  musicPlayer.playFullFile("track008.mp3"); 
	  break; 
	case 9:
          Serial.println(9);
	  musicPlayer.playFullFile("track009.mp3");
	  break;
	case 10:
          Serial.println(10);
	  musicPlayer.playFullFile("track010.mp3");
	  break;
	case 11:
          Serial.println(11);
	  musicPlayer.playFullFile("track011.mp3");
	  break;
	case 12:
          Serial.println(12);
	  musicPlayer.playFullFile("track012.mp3");
	  break;
	case 13:
          Serial.println(13);
	  musicPlayer.playFullFile("track013.mp3");
	  break;
	case 14:
          Serial.println(14);
	  musicPlayer.playFullFile("track014.mp3");
	  break;
	case 15:
          Serial.println(15);
	  musicPlayer.playFullFile("track015.mp3");
	  break;
	case 16:
          Serial.println(16);
	  musicPlayer.playFullFile("track016.mp3");
	  break;
	case 17:
          Serial.println(17);
	  musicPlayer.playFullFile("track017.mp3");
	  break;
	case 18:
          Serial.println(18);
	  musicPlayer.playFullFile("track018.mp3");
	  break;
	case 19:
          Serial.println(19);
	  musicPlayer.playFullFile("track019.mp3");
	  break;
	case 20:
          Serial.println(20);
	  musicPlayer.playFullFile("track020.mp3");
	  break;
	case 21:
          Serial.println(21);
	  musicPlayer.playFullFile("track021.mp3");
	  break;
	case 22:
          Serial.println(22);
	  musicPlayer.playFullFile("track022.mp3");
	  break;
	case 23:
          Serial.println(23);
	  musicPlayer.playFullFile("track023.mp3");
	  break;
	case 24:
          Serial.println(24);
	  musicPlayer.playFullFile("track024.mp3");
	  break;
	case 25:
          Serial.println(25);
	  musicPlayer.playFullFile("track025.mp3");
	  break;
	case 26:
          Serial.println(26);
	  musicPlayer.playFullFile("track026.mp3");
	  break;
	case 27:
          Serial.println(27);
	  musicPlayer.playFullFile("track027.mp3");
	  break;
	case 28:
          Serial.println(28);
	  musicPlayer.playFullFile("track028.mp3");
	  break;
	case 29:
          Serial.println(29);
	  musicPlayer.playFullFile("track029.mp3");
	  break;
	case 30:
          Serial.println(30);
	  musicPlayer.playFullFile("track030.mp3");
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

    int randNumber = random(1, 3);
    
    if (r == 1){
      if (randNumber == 1) r = 2;
      else r = 3;
    } 
    else if (r == 2){
      if (randNumber == 1) r = 1;
      else r = 3;
    }
    else{
      if (randNumber == 1) r = 1;
      else r = 2;
    }

    if (r == 1) {
      digitalWrite(LED_1, HIGH);
      digitalWrite(LED_2, LOW);
      digitalWrite(LED_3, LOW);
    }
    else if (r == 2) {
      digitalWrite(LED_1, LOW);
      digitalWrite(LED_2, HIGH);
      digitalWrite(LED_3, LOW);
    }
    else {
      digitalWrite(LED_1, LOW);
      digitalWrite(LED_2, LOW);
      digitalWrite(LED_3, HIGH);
    }
    
/*    Serial.println("randomNumber: " + randNumber);
    Serial.print(" n1: ");
    Serial.print(n1);
    Serial.print(" n2: ");
    Serial.print(n2);
    Serial.print(" n3: ");
    Serial.println(n3);*/
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

  /*if (sensorState && !lastState_1) {
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
  
  if( sensorState_2 == HIGH){
     if( sensorState == HIGH){
       canFlag = 1;
     } 
  }
  //Verifier, tier-1 is on the bottom
  if( canFlag == 1) {
    if ( sensorState_2 == HIGH) {
      if ( sensorState == LOW ) {
  
        if (sensor2_1 == 1) {
          canFlag = 0;
          return true;
        }
      }
    }
    canFlag = 0;
    return false;
  }
  
  canFlag = 0;*/
  
  
    if( sensorState == LOW ){
       canFlag = 0; 
    }
     
    if( sensorState_2 == LOW ){
     if( sensorState == HIGH ){
       //Serial.println("This should score");
       canFlag = 1;
     } 
    }
    
    if( canFlag == 1 ){
      if( sensorState == HIGH && sensorState_2 == HIGH ){
        // Serial.println("This should score"); 
         canFlag = 0;
         //canCount++;
         //Serial.println( canCount );
         return true;
      }
    }
    
    return false;
}

// return true if item passes verifier
bool verifier2(int sensorPin1, int sensorPin2) {

  // read the state of the 2-tier sensor value:
  int sensorState = digitalRead(sensorPin1); //TOP SENSOR
  int sensorState_2 = digitalRead(sensorPin2); //BOTTOM SENSOR

  // check if the sensor beam is broken
  // if it is, the sensorState is LOW:
/*
  if (sensorState && !lastState_2_3) {
   // Serial.println("Broken_1");
  }
  if (!sensorState && lastState_2_3) {
    //Serial.println("Unbroken_1");
  }
  lastState_2_3 = sensorState;
  // check sensor state 2
  if (sensorState_2 && !lastState_2_2) {
    //Serial.println("Broken_2");
    sensor2_2 = 1;
  }
  if (!sensorState_2 && lastState_2_2) {
    //Serial.println("Unbroken_2");
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
  return false;*/

  
    
  
     if( sensorState == HIGH){
       paperFlag = 0; 
    }
     
    if( sensorState == LOW ){
     if( sensorState_2 == HIGH ){
      // Serial.println("HEREEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE");
       paperFlag = 1;
     } 
    }
    
    if( paperFlag == 1 ){
      if( sensorState == LOW && sensorState_2 == LOW ){
        // Serial.println("This should score"); 
         paperFlag = 0;
         //canCount++;
         //Serial.println( canCount );
         return true;
      }
    }
    
    return false;
}

// return true if item passes verifier
bool verifier3(int sensorPin1, int sensorPin2) {


  // read the state of the 2-tier sensor value:
  int sensorState = digitalRead(sensorPin1);
  int sensorState_2 = digitalRead(sensorPin2);

      if( sensorState == HIGH){
       trashFlag = 0; 
    }
     
    if( sensorState == LOW ){
     if( sensorState_2 == HIGH ){
      // Serial.println("HEREEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE");
       trashFlag = 1;
     } 
    }
    
    if( trashFlag == 1 ){
      if( sensorState == LOW && sensorState_2 == LOW ){
        // Serial.println("This should score"); 
         trashFlag = 0;
         //canCount++;
         //Serial.println( canCount );
         return true;
      }
    }
    
    return false;
}
