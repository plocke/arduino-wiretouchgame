#ifndef constants_h
#define constants_h

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal.h>

const String codeversion = "1.0";



//states
const int STATE_BOOTED_UP = 0;
const int STATE_WAITING_TO_START = 10;
const int STATE_COUNTDOWN = 15;
const int STATE_GOING = 20;
const int STATE_WIN = 25;
const int STATE_FAILED = 30;


//debug flag
const boolean DEBUG_SERIAL = true;


//analog user button inputs.  we are just using them for digital reads
const int PIN_NEWGAMEBUTTON = A0;
const int PIN_STARTCRADLE = A1;
const int PIN_GAMEWIRE = A2;
const int PIN_WINCRADLE = A3;

//output pins
const int PIN_GOING_LED_GREEN = 9; //PWM Pin
const int PIN_YELLOW_LED = 10;
const int PIN_FAILED_LED= 8;
const int PIN_SPEAKER = 13;


//sound constants
const int NOTE_FREQUENCY_FAIL = 80;
const int NUM_SUCCESS_NOTES = 3;
const int NOTE_FREQUENCY_SUCCESS_ARRAY[] = {300,400,500};


//timing constants
const long ERROR_TONE_LENGTH_MILLIS = 1500;
const int SUCCESS_TONE_LENGTH_MS = 350;
const long COUNTDOWN_STEPLENGTH_MS = 500;
const int COUNTDOWN_NUMSTEPS = 3;
const long DEBOUNCE_THRESHOLD_MS = 100;



#endif
