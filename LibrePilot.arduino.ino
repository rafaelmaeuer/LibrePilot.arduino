#include <SoftwareSerial.h>
#include "FlightStatus.h"
#include "LibrePilotSerial.h"

SoftwareSerial mySerial(8, 9);  // RX, TX
LibrePilotSerial lps(&mySerial); // init software serial
int refreshRate = 50; // set data refresh rate
bool debug = true;

const int LED = 7; // set LED pin

int ledState = LOW; // ledState used to set the LED
int ledMode = 0; // set LED mode (0:off, 1:blink, 2:on)

long blinkInterval = 100; // set blink interval
unsigned long previousMillis = 0; // will store last time LED was updated

void setup() {
  // initialize serial communication at 57600 bits per second:
  Serial.begin(57600);
  lps.serial->begin(57600); // initialize LibrePilotSerial Object
  if (debug) Serial.println("Start");
  pinMode(LED, OUTPUT); // set LED as output
  LEDsetup(1000); // test LED on startup
}

void loop() {
  
  lps.request(FLIGHTSTATUS_OBJID);
  boolean ok = lps.receive(FLIGHTSTATUS_OBJID, FlightStatusDataUnion.arr, 200);

  if (debug) {
    Serial.print(" Result fs: ");
    //Serial.println(FlightStatusDataUnion.data.Armed);
    Serial.println(FlightStatusDataUnion.arr;
  }   

  if(ok) {
    if(FlightStatusDataUnion.data.Armed == FLIGHTSTATUS_ARMED_DISARMED) {
      ledMode = 0;
    } else if (FlightStatusDataUnion.data.Armed == FLIGHTSTATUS_ARMED_ARMING) {
      ledMode = 1;
    } else if (FlightStatusDataUnion.data.Armed == FLIGHTSTATUS_ARMED_ARMED) {
      ledMode = 2;
    }
    setLEDstate();
  }

  delay(refreshRate);
}

void setLEDstate() {
  switch (ledMode) {
    case 2:
      LEDon();
      break;
    case 1:
      LEDblink();
      break;
    default:
      LEDoff();
  }
}

void LEDon() {
  digitalWrite(LED, HIGH);
}

void LEDoff() {
  digitalWrite(LED, LOW);
}

void LEDsetup(int delayTime) {
  LEDon();
  delay(delayTime);
  LEDoff();
}

void LEDblink() {
  // check to see if it's time to blink the LED; that is, if the difference
  // between the current time and last time you blinked the LED is bigger than
  // the interval at which you want to blink the LED.
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= blinkInterval) {
    previousMillis = currentMillis; // save the last time you blinked the LED

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
      LEDon();
    } else {
      ledState = LOW;
      LEDoff();
    }
  }
}
