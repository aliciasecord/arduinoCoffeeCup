/************************** Configuration ***********************************/

#include "config.h"

/************************ Project Starts Here *******************************/

// bottom of cup button is digital pin 5
#define BUTTON_PIN 5

// reset button is digital pin 4
#define RESET_BUTTON 4

// Variables will change:
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button

int resetButtonState = 0;   // current state of reset button
int lastResetButtonState = 0; // previous state of reset button

int currentMillis = 0; // set millis to 0
int previousMillis = 0; // set time lapse to 0
int timeSinceLastButton = 0; // set time to 0

// set up the 'digital' feed
AdafruitIO_Feed *coffeebutton = io.feed("coffeebutton");

void setup() {

  // set button pins as an input
  pinMode(BUTTON_PIN, INPUT);
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

  // read the pushbutton input pin:
  buttonState = digitalRead(BUTTON_PIN);

  // read the reset button input pin:
  resetButtonState = digitalRead(RESET_BUTTON);

  currentMillis = millis();

  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == HIGH) {
      // if the current state is HIGH then the button went from off to on (on means the cup is down, off means someone is drinking):
      timeSinceLastButton = (currentMillis-previousMillis)/1000;
      previousMillis = currentMillis;
      buttonPushCounter += timeSinceLastButton/2;
      Serial.print("number of oz consumed: ");
      Serial.println(buttonPushCounter);
      Serial.println(timeSinceLastButton);
      coffeebutton->save(buttonPushCounter);
      
      // set button counter back to 1 after 35 presses
      if (buttonPushCounter > 35) {
        Serial.println(buttonPushCounter);
        coffeebutton->save(35);
        buttonPushCounter = 1;
      }
      else {buttonPushCounter;}
      
    } else {
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }

  // compare the resetButtonState to its previous state
  if (resetButtonState != lastResetButtonState){
    // if the reset button is pushed, set counter back to 0
    if (resetButtonState == HIGH){
      buttonPushCounter = 0;
      Serial.println("reset");
      coffeebutton->save(buttonPushCounter);
      }
    else {}
    // Delay a little bit to avoid bouncing
    delay(50);
  }

  
  // save the current state as the last state, for next time through the loop
  lastButtonState = buttonState;

  // save the current reset state as the last state for next time through loop
  lastResetButtonState = resetButtonState;
}


