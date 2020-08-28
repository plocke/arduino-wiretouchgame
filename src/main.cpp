#include <Arduino.h>
#include <constants.h>

void testLEDPin(int ledPIN)
{
  digitalWrite(ledPIN, HIGH);
  delay(350);
  digitalWrite(ledPIN, LOW);
}

void setup() {
  // put your setup code here, to run once:
  //LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

  for (int i = 2; i<= 13; i++) {
    pinMode(i, OUTPUT);
  }

  testLEDPin(PIN_FAILED_LED);
  testLEDPin(PIN_YELLOW_LED);
  testLEDPin(PIN_GOING_LED_GREEN);
  tone(PIN_SPEAKER, 523);
  delay(250);
  noTone(PIN_SPEAKER);
}



void loop() {
  // put your main code here, to run repeatedly:

  

}

