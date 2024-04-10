#include <Arduino.h>

const int ledPin = 0; // the number of the LED pin
const int ledPin2 = 2;
// variable for storing the pushbutton status

void setup()
{
  // initialize the pushbutton pin as an input

  // initialize the LED pin as an output
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);
}

void loop()
{

  digitalWrite(ledPin, HIGH);
  digitalWrite(ledPin2, HIGH);
  delay(2000);

  digitalWrite(ledPin, LOW);
  digitalWrite(ledPin2, LOW);
  delay(2000);
}