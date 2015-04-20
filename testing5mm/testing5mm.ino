    /*
    IR Breakbeam sensor demo!
    */
     
    #define LEDPIN 13
    // Pin 13: Arduino has an LED connected on pin 13
    // Pin 11: Teensy 2.0 has the LED on pin 11
    // Pin 6: Teensy++ 2.0 has the LED on pin 6
    // Pin 13: Teensy 3.0 has the LED on pin 13
     
    #define SENSORPIN A0
    #define SENSORPIN_2 A1
     
    // variables will change:
    int sensorState = 0, lastState=0, sensorState_2 = 0, lastState_2 = 0; // variable for reading the pushbutton status
    int confirmCount = 0;
    
    int sensor2 = 0
    ;
     
    void setup() {
    // initialize the LED pin as an output:
    pinMode(LEDPIN, OUTPUT);
    // initialize the sensor pin as an input:
    pinMode(SENSORPIN, INPUT);
    pinMode(SENSORPIN_2, INPUT);
    digitalWrite(SENSORPIN, HIGH); // turn on the pullup
    digitalWrite(SENSORPIN_2, HIGH);
    Serial.begin(9600);
    }
     
    void loop(){
    // read the state of the pushbutton value:
    sensorState = digitalRead(SENSORPIN);
    sensorState_2 = digitalRead(SENSORPIN_2);
     
    // check if the sensor beam is broken
    // if it is, the sensorState is LOW:
    if (sensorState == HIGH) {
    // turn LED on:
      Serial.println("Broken");
    }
    else {
    // turn LED off:
      Serial.println("Unbroken");
    }
  /*  if (sensorState && !lastState) {
    Serial.println("Unbroken_1");
    
    }
    if (!sensorState && lastState) {
    Serial.println("Broken_1");
    
  //  idonteven++;
    //Serial.println(idonteven);
    }
    lastState = sensorState;
    
    
    if (sensorState_2 == LOW) {
    // turn LED on:
    //digitalWrite(LEDPIN, LOW);
    }
    else {
    // turn LED off:
    //digitalWrite(LEDPIN, HIGH);
    }
    if (sensorState_2 && !lastState_2) {
    Serial.println("Unbroken_2");
    //sensor2 = 1;
    }
    if (!sensorState_2 && lastState_2) {
    
    Serial.println("Broken_2");
    sensor2 = 1;
    //idonteven++;
    //Serial.println(idonteven);
    }
    lastState_2 = sensorState_2;*/
    
    
    
    //Verifier, sensor 1 is on the bottom
    if( sensorState_2 == HIGH ){
      if( sensorState == LOW ){
     
          if(sensor2 == 1){
            confirmCount++;
            Serial.println(confirmCount);
            sensor2 = 0;

          }
        }
      }

    
    
    }
