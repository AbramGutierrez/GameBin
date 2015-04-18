/*

VISUAL UNO

Display LED Matrix Scoreboard and receive input from audioUno about updated score

*/

#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library

#define CLK 8  // MUST be on PORTB! (Use pin 11 on Mega)
#define LAT A3
#define OE  9
#define A   A0
#define B   A1
#define C   A2
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

#define LEDPIN 13
#define COUNTERPIN 10
 
int value = 0;
int check = 0;
    
void setup() {
  
  matrix.begin();
  
  // initialize the LED pin as an output:
  pinMode(LEDPIN, OUTPUT);
  
  // initialize the sensor pin as an input:
  pinMode(COUNTERPIN, INPUT);
  digitalWrite(COUNTERPIN,LOW);
 
  Serial.begin(9600);
  
  // GAME BIN TITLE - Only on power up
  matrix.setCursor(1, 0);   // start at top left, with one pixel of spacing
  matrix.setTextSize(1);    // size 1 == 8 pixels high
  
  // print each letter with a rainbow color
  matrix.setTextColor(matrix.Color333(7,0,0));
  matrix.print('G');
  matrix.setTextColor(matrix.Color333(7,4,0)); 
  matrix.print('A');
  matrix.setTextColor(matrix.Color333(7,7,0));
  matrix.print('M');
  matrix.setTextColor(matrix.Color333(4,7,0)); 
  matrix.print('E');
  
  matrix.setCursor(1, 9);   // next line
  matrix.setTextColor(matrix.Color333(0,7,7)); 
  matrix.print('B');
  matrix.setTextColor(matrix.Color333(0,4,7)); 
  matrix.print('I');
  matrix.setTextColor(matrix.Color333(0,0,7));
  matrix.print('N');
  
  delay(1000);
  
  // fill the screen with 'black'
  matrix.fillScreen(matrix.Color333(0, 0, 0));
 
}

void loop() {
  
  if (check == 0 ) {

    if (digitalRead(COUNTERPIN)==HIGH) { //Signal from audioUno of proper count increase
      check = 1;
      delay(200);
    }
  }
  
  if (check==1){
    value +=1;
    check = 0;
    
    /*
  Serial.print("Score: ");
  Serial.print(value);
  Serial.print('\n');
  */
  
  //Changing LED Matrix Panel
  matrix.fillScreen(0);
  
  matrix.swapBuffers(false);
  
  matrix.setCursor(1, 0);   // start at top left, with one pixel of spacing
  matrix.setTextSize(1);    // size 1 == 8 pixels high
  
  matrix.setTextColor(matrix.Color333(7,0,0)); //RED
  matrix.print("SCORE");
  
  matrix.setCursor(1, 9);   // next line
  matrix.setTextColor(matrix.Color333(0,7,7)); //BLUE
  
  matrix.print(value);

  delay(1000); // Delay to refresh LED Matrix
  
  // fill the screen with 'black'
  matrix.fillScreen(matrix.Color333(0, 0, 0));

  }
  
  if (value==5){
      value = 0; //Reset Counter
  }
 
}
