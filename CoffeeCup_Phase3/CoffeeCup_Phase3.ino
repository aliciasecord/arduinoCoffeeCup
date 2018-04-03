/************************** Configuration ***********************************/

#include "config.h"

/************************ Project Starts Here *******************************/

// tilt sensor is digital pin 3
#define TILT_PIN 3

// reset button is digital pin 4
#define RESET_BUTTON 4

// Variables will change
int ouncesCounter = 0;  // counter for ounces consumed
int tiltState = 0;      // current state of tilt sensor
int lastTiltState = 0;  // previous tilt state

int buttonState = 0;    // current state of the button
int lastButtonState = 0; // previous state of the button

int currentMillis = 0;  // set millis to 0
int previousMillis = 0;  // set time elapsed to 0
int tiltTime = 0;       // set time to 0

// set up the 'coffeebutton' feed
AdafruitIO_Feed *coffeebutton = io.feed("coffeebutton");

void setup() {
  // set tilt pin as input
  pinMode(TILT_PIN, INPUT);
  
  // set button pin as an input
  pinMode(RESET_BUTTON, INPUT);

  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while(! Serial);

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
    }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  
}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

  // read tilt input pin
  tiltState = digitalRead(TILT_PIN);

  // read the button input pin
  buttonState = digitalRead(RESET_BUTTON);

  currentMillis = millis();

  // compare the tiltState to its previous state
  if (tiltState != lastTiltState) {
    // if the current state is LOW then person is no longer drinking:
    if (tiltState == HIGH) {
      Serial.println("upright");
      tiltTime = (currentMillis-previousMillis)/1000;
      previousMillis = currentMillis;
      ouncesCounter += tiltTime/2;
      Serial.print("number of oz consumed: ");
      Serial.println(ouncesCounter);
      Serial.println(tiltTime);
      coffeebutton->save(ouncesCounter);

      // set ounces counter back to 1 after 35 presses
      if (ouncesCounter > 35) {
        Serial.println(ouncesCounter);
        coffeebutton->save(35);
        ouncesCounter = 1;
      }
      else {ouncesCounter;}
    }
    else{
      Serial.println("tilted");
      }
  } else{}


      
  // compare the resetButtonState to its previous state
  if (buttonState != lastButtonState){
    // if the reset button is pushed, set counter back to 0
    if (buttonState == HIGH){
      ouncesCounter = 0;
      Serial.println("reset");
      coffeebutton->save(ouncesCounter);
      }
    else {}
  }
  
  // save the current tilt state as the last state, for next time through the loop
  lastTiltState = tiltState;

  // save the current reset state as the last state for next time through loop
  lastButtonState = buttonState;  

}
