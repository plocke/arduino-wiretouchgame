#ifndef constants_h
#define constants_h

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal.h>

const String codeversion = "1.0";



//states
const int STATE_BOOTED_UP = 0;
const int STATE_WAITING_TO_START = 10;
const int STATE_GOING = 20;
const int STATE_FAILED = 30;
const int STATE_SUCCESS = 40;


//debug flag
const boolean DEBUG_SERIAL = true;


//analog user button inputs.  we are just using them for digital reads
const int PIN_NEWGAMEBUTTON = A0;
const int PIN_STARTCRADLE = A1;
const int PIN_GAMEWIRE = A2;

//output pins
const int PIN_GOING_LED_GREEN = 3; //PWM Pin
const int PIN_YELLOW_LED = 4;
const int PIN_FAILED_LED= 2;
const int PIN_SPEAKER = 5;

//global variables
extern int state;
extern long timeSinceStart;

//sound constants
const int NOTE_FREQUENCY_FAIL = 80;
const int NOTE_FREQUENCY_SUCCESS_ARRAY[] = {200,300,400};


//timing constants
const long ERROR_TONE_LENGTH_MILLIS = 1500;
const int SUCCESS_TONE_LENGTH_MS = 350;



#endif
