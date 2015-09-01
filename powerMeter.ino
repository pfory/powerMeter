//  Elektroměr digitální, třífázový 800 impulsů = 1kWh, impulsni vystup musi byt napajen 12V, vstup na D2 pres delic
//  Arduino Pro Mini 328

//  impulsni vystup
//  +12V - elektromer - odpor - D2 - odpor - GND

//  indikacni LED
//  D3 - 220 ohm - LED anoda - LED katoda - GND

//---------------------------------------------------------------
//         Arduino   
//         D2                   interrupt od elektromeru
//         D3                   LED indikujici impulsy
//---------------------------------------------------------------

//  version history
//  0.1   - 2015 initial version


//  ---------------------------------------------------------------- C O D E -----------------------------------------------
const byte counterPin = 2; 
const byte counterInterrupt = 0; // = pin D2
unsigned long startPulse=0;
unsigned int pulseLength=0;
unsigned int pulseCount=0;

#define STATUS_LED 3

#define watchdog
#ifdef watchdog
#include <avr/wdt.h>
#endif

#include <Wire.h>

float versionSW=0.1;
char versionSWString[] = "energyMeter v"; //SW name & version

byte status=0;

unsigned int const SERIAL_SPEED=9600;

//#define verbose

//------------------------------------------------------------- S E T U P -------------------------------------------------------
void setup() {
  Wire.begin(2);                // join i2c bus with address #2
  Wire.onRequest(getPulseCount); // register event
  
#ifdef watchdog
	wdt_enable(WDTO_8S);
#endif
  Serial.begin(SERIAL_SPEED);
  //Serial.print(versionSWString);
  Serial.println(versionSW);
  pinMode(STATUS_LED, OUTPUT);      // sets the digital pin as output
#ifdef watchdog
	wdt_reset();
#endif  
  
#ifdef watchdog
	wdt_reset();
#endif  

  pinMode(counterPin, INPUT);      
  attachInterrupt(counterInterrupt, counterISR, CHANGE);
}

//------------------------------------------------------------ L O O P -----------------------------------------------------------------------
void loop() {
#ifdef watchdog
	wdt_reset();
#endif  
}

//------------------------------------------------------------ F U N C T I O N S --------------------------------------------------------------
void counterISR() { 
  if (digitalRead(counterPin)==HIGH) {
    digitalWrite(STATUS_LED,HIGH);
    startPulse=millis();
  } else {
    digitalWrite(STATUS_LED,LOW);
    pulseLength = millis()-startPulse;
    if ((pulseLength)>35 && (pulseLength)<100) {
      pulseCount++;
    }
  }
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void getPulseCount()
{
  Wire.write(pulseCount); 
  pulseCount=0;
}