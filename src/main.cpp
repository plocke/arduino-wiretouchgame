#include <Arduino.h>
#include <constants.h>
#include <lcdhelper.h>


//global variables
int currentState = STATE_BOOTED_UP;

long timeSinceStartTimer;
long countDownTimer;
long gameOverDisplayTimer;
int currentCountDownStep;
long debounceTimer;
int lastDebouncePinState;
long winTimeMillis;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void resetVariables() {

 timeSinceStartTimer = 0L;
 countDownTimer = 0L;
 gameOverDisplayTimer = 0L;
 currentCountDownStep = COUNTDOWN_NUMSTEPS;
 debounceTimer = 0L;
 lastDebouncePinState = -1;
 winTimeMillis = -1;
}

void testLEDPin(int ledPIN)
{
  digitalWrite(ledPIN, HIGH);
  delay(350);
  digitalWrite(ledPIN, LOW);
}

void lightOneLEDOnlyInCountdown(int countdownStep) {
  int ledPIN = COUNTDOWN_LIGHT_PINS[countdownStep];
  Serial.println("Lighting LEDPin "+String(ledPIN)+ " for countdown step "+String(countdownStep));
  for (int i = 0; i < COUNTDOWN_NUMSTEPS; i++)
  {
    if (i == countdownStep) {
      digitalWrite(ledPIN, HIGH);
    } else {
      digitalWrite(COUNTDOWN_LIGHT_PINS[i], LOW);
    }
  }
}

void playSoundForCountdownStep(int countDownStep) {
  int toneLength = (countDownStep == 0) ? COUNTDOWN_STEPLENGTH_MS : COUNTDOWN_STEPLENGTH_MS/4; 
  tone(PIN_SPEAKER, NOTE_FREQUENCY_COUNTDOWN_ARRAY[countDownStep],toneLength );
}

long getElapsedMillisForTimerVariable(long timerVariable) {
  return millis() - timerVariable;
}

int getElapsedSecondsForTImerVariable(long timerVariable) {
  return getElapsedMillisForTimerVariable(timerVariable)/1000;
}

void updateLCDBottomLineWithElapsedTime(long timerVariable) {
   if (getElapsedMillisForTimerVariable(timerVariable) % 100 == 0)
    {
      setBottomLine(String(getElapsedSecondsForTImerVariable(timerVariable))+"."+String((getElapsedMillisForTimerVariable(timerVariable)-getElapsedSecondsForTImerVariable(timerVariable)*1000L)/100L)+"s", lcd);
    }
}



void transitionToState(int newState) {
  if (currentState == newState) {
    return;
  } 

  switch (newState)
  {
  case STATE_WAITING_TO_START:
    resetVariables();
    setBothLCDLines("Touch Wand to", "*Start* to begin", lcd);
    break;

   case STATE_COUNTDOWN:
    lcd.clear();
    noTone(PIN_SPEAKER);
    setTopLine("Get Ready!", lcd);
    currentCountDownStep = COUNTDOWN_NUMSTEPS;
    //countDownTimer = millis();
    break;

  case STATE_GOING:
    timeSinceStartTimer = millis();
    lcd.clear();
    setTopLine("Go (Carefully) ;)", lcd);
    break;

  case STATE_WIN:
    winTimeMillis = millis() - timeSinceStartTimer;
    gameOverDisplayTimer = millis();
    setTopLine("Winner! Time:", lcd);
      for (int i = 0; i<NUM_SUCCESS_NOTES; i++){
        tone(PIN_SPEAKER, NOTE_FREQUENCY_SUCCESS_ARRAY[i], SUCCESS_TONE_LENGTH_MS);
        delay(SUCCESS_TONE_LENGTH_MS);
      }
    break;

  case STATE_FAILED:
    tone(PIN_SPEAKER, NOTE_FREQUENCY_FAIL, ERROR_TONE_LENGTH_MILLIS);
    for (int i = 0; i < COUNTDOWN_NUMSTEPS; i++){
        digitalWrite(COUNTDOWN_LIGHT_PINS[i], LOW);
    }
    
    
    gameOverDisplayTimer = millis();
    break;


  
  default:
    break;
  }

  Serial.println("Transitioning state "+String(currentState)+" to "+String(newState));
  Serial.println("game time elapsed is "+String(millis()-timeSinceStartTimer));
  currentState = newState;
}

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
lcd.begin(16,2);


  for (int i = 2; i<= 13; i++) {
    pinMode(i, OUTPUT);
  }

  testLEDPin(PIN_RED_LED);
  testLEDPin(PIN_YELLOW_LED);
  testLEDPin(PIN_GOING_LED_GREEN);
  tone(PIN_SPEAKER, 523);
  delay(250);
  noTone(PIN_SPEAKER);

  transitionToState(STATE_WAITING_TO_START);
}

void loop() {
  // put your main code here, to run repeatedly:
  switch (currentState)
  {
  case STATE_WAITING_TO_START:
    if (digitalRead(PIN_STARTCRADLE) == HIGH) {
      transitionToState(STATE_COUNTDOWN);
    }
    break;

  case STATE_GOING:
    if (digitalRead(PIN_GAMEWIRE) == HIGH) {
      setBothLCDLines("** WIRE TOUCHED ** ","you lasted "+String(getElapsedSecondsForTImerVariable(timeSinceStartTimer))+"s", lcd);
      transitionToState(STATE_FAILED);
    }

    updateLCDBottomLineWithElapsedTime(timeSinceStartTimer);

    if(digitalRead(PIN_WINCRADLE) == HIGH) {
      transitionToState(STATE_WIN);
    }
    break;

  case STATE_WIN:
  if (digitalRead(PIN_STARTCRADLE) == HIGH) {
        transitionToState(STATE_COUNTDOWN);
      }
        if(getElapsedSecondsForTImerVariable(gameOverDisplayTimer) > 5) {
          transitionToState(STATE_WAITING_TO_START);
        }
    break;

  case STATE_FAILED:
      if (digitalRead(PIN_STARTCRADLE) == HIGH) {
        transitionToState(STATE_COUNTDOWN);
      }
        if(getElapsedSecondsForTImerVariable(gameOverDisplayTimer) > 5) {
          transitionToState(STATE_WAITING_TO_START);
        }
    break;

  case STATE_COUNTDOWN:
    
    int currentStartPinState = digitalRead(PIN_STARTCRADLE);
    if (currentStartPinState != lastDebouncePinState) {
      debounceTimer = millis();
      lastDebouncePinState = currentStartPinState;

      //Serial.println("In Debounce. Pinstate is "+String(currentStartPinState)+" lastPinState is "+String(lastDebouncePinState));
    }

    if ((millis() - debounceTimer) > DEBOUNCE_THRESHOLD_MS)
    {
     // Serial.println("In Debounce Else. Pinstate is "+String(currentStartPinState)+" lastPinState is "+String(lastDebouncePinState));

      if( currentStartPinState == LOW) { //started too early
        setBothLCDLines("** DISQUALIFIED ** ","Started too early", lcd);
        transitionToState(STATE_FAILED);
      }
    }

    //Serial.println("millis minus debounce is "+(String)(millis() - debounceTimer));

    if (countDownTimer == 0 || (getElapsedMillisForTimerVariable(countDownTimer) > COUNTDOWN_STEPLENGTH_MS)) {
      countDownTimer = millis();
      currentCountDownStep --;
      playSoundForCountdownStep(currentCountDownStep);
      lightOneLEDOnlyInCountdown(currentCountDownStep);
      setBottomLine("In "+String(currentCountDownStep), lcd);
    }
    
    if (currentCountDownStep == 0) {
      transitionToState(STATE_GOING);
    }

    break;
  }

  

}

