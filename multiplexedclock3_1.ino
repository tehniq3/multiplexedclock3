/*
 base sketch: https://cdn.sparkfun.com/datasheets/Components/LED/_7Seg_Example.pde
  4 digit 7 segment display: http://www.sparkfun.com/products/9483 
 7 segments + 4 digits + 1 colon = 12 pins required for full control 
modified connexion by Nicu FLORICA (niq_ro) from http://nicuflorica.blogspot.com
http://arduinotehniq.blogspot.com
http://www.tehnic.go.ro
http://www.arduinotehniq.com/
last change for manual adjust for hours and minutes, thermometer and higrometer: 11.08.2017, Craiova
*/

int digit1 = 11; //PWM pin for control digit 1 (left side)
int digit2 = 10; //PWM pin for control digit 2
int digit3 = 9;  //PWM pin for control digit 3
int digit4 = 6;  //PWM pin for control digit 4 (right side)

int segA = 2; // pin for control "a" segment
int segB = 3; // pin for control "b" segment
int segC = 4; // pin for control "c" segment
int segD = 5; // pin for control "d" segment
int segE = 12; // pin for control e" segment
int segF = 7; // pin for control "f" segment
int segG = 8; // pin for control "g" segment
int segDP = 13; // pin for control decimal point

#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;

// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
// original sketck from http://learn.adafruit.com/ds1307-real-time-clock-breakout-board-kit/
// add part with SQW=1Hz from http://tronixstuff.wordpress.com/2010/10/20/tutorial-arduino-and-the-i2c-bus/
// add part with manual adjust http://www.bristolwatch.com/arduino/arduino_ds1307.htm
// original for common anode http://arduinotehniq.blogspot.ro/2014/09/manual-adjust-for-rtc-clock-with.html
// changed for common catode in 7.2.2017

#include "DHT.h"
#define DHTPIN A3     // pin for connect data's sensor
#define DHTTYPE DHT11   // DHT 11 
DHT dht(DHTPIN, DHTTYPE);

#define SW0 A0   // pin for MENU/change
#define SW1 A1   // pin for increase value (+)

// use for hexa in zecimal conversion
int zh, uh, ore;
int zm, um, miniti;

#define DISPLAY_BRIGHTNESS  500
//Display brightness
//Each digit is on for a certain amount of microseconds
//Then it is off until we have reached a total of 20ms for the function call
//Let's assume each digit is on for 1000us
//Each digit is on for 1ms, there are 4 digits, so the display is off for 16ms.
//That's a ratio of 1ms to 16ms or 6.25% on time (PWM).
//Let's define a variable called brightness that varies from:
//5000 blindingly bright (15.7mA current draw per digit)
//2000 shockingly bright (11.4mA current draw per digit)
//1000 pretty bright (5.9mA)
//500 normal (3mA)
//200 dim but readable (1.4mA)
//50 dim but readable (0.56mA)
//5 dim but readable (0.31mA)
//1 dim but readable in dark (0.28mA)
/*
//for common anode
#define DIGIT_ON  HIGH
#define DIGIT_OFF  LOW
#define SEGMENT_ON  LOW
#define SEGMENT_OFF HIGH
*/
// for common catode
#define DIGIT_ON  LOW
#define DIGIT_OFF  HIGH
#define SEGMENT_ON  HIGH
#define SEGMENT_OFF LOW

byte bucla = 30;
int t, h;
int timp; 
byte meniu = 0;   // 0 = usual state (clack, thetmometer & higrometer)
                  // 1 - adjust hour
                  // 2 - adjust minutes
byte minutes1 = 0;
byte hours1 = 0;
byte minutes = 0;
byte hours = 0;


void setup() {
  
 //  Serial.begin(57600);
  Wire.begin();
  RTC.begin();
// RTC.adjust(DateTime(__DATE__, __TIME__));
// if you need set clock... just remove // from line above this

// part code for flashing LED
Wire.beginTransmission(0x68);
Wire.write(0x07); // move pointer to SQW address
// Wire.write(0x00); // turns the SQW pin off
 Wire.write(0x10); // sends 0x10 (hex) 00010000 (binary) to control register - turns on square wave at 1Hz
// Wire.write(0x13); // sends 0x13 (hex) 00010011 (binary) 32kHz

Wire.endTransmission();

  if (! RTC.isrunning()) {
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  
 dht.begin();

  pinMode(segA, OUTPUT);
  pinMode(segB, OUTPUT);
  pinMode(segC, OUTPUT);
  pinMode(segD, OUTPUT);
  pinMode(segE, OUTPUT);
  pinMode(segF, OUTPUT);
  pinMode(segG, OUTPUT);
  pinMode(segDP, OUTPUT);

  pinMode(digit1, OUTPUT);
  pinMode(digit2, OUTPUT);
  pinMode(digit3, OUTPUT);
  pinMode(digit4, OUTPUT);
  
//  pinMode(13, OUTPUT);

 Serial.begin(9600);
 Serial.println("test for niq_ro");

 pinMode(SW0, INPUT);  // for this use a slide switch
 pinMode(SW1, INPUT);  // N.O. push button switch
  digitalWrite(SW0, HIGH); // pull-ups on
  digitalWrite(SW1, HIGH);
}

void loop() {

if (meniu == 0)
{  
digitalWrite(segDP, SEGMENT_OFF);
  DateTime now = RTC.now();
  timp = now.hour()*100+now.minute();

for (byte ceasu = 0; ceasu < 5; ceasu++)
{
// display parts   
   for(int i = 250 ; i >0  ; i--) {
     if (timp >= 1000) displayNumber01(timp); 
     else displayNumber02(timp); 
   } 
 if (!(digitalRead(SW0)))
  {
    minutes=now.minute();
    hours=now.hour(); 
    meniu = 1; //go to menu for change hour
  }
   for(int i = 250 ; i >0  ; i--) {
     if (timp >= 1000) displayNumber03(timp); 
     else displayNumber04(timp); 
   } 

  if (!(digitalRead(SW0)))
  {
    minutes=now.minute();
    hours=now.hour(); 
    meniu = 1; //go to menu for change hour
  }
}

if (bucla > 20)
{
  h = dht.readHumidity();
  t = dht.readTemperature();
  bucla = 0;
}
   for(int i = 1000 ; i >0  ; i--) {
     displayNumber(t); // this is number to diplay
   }
  if (!(digitalRead(SW0)))
  {
    minutes=now.minute();
    hours=now.hour(); 
    meniu = 1; //go to menu for change hour
  }
   for(int i = 1000 ; i >0  ; i--) {
     displayNumber1(h); // this is number to diplay
   }
 if (!(digitalRead(SW0)))
  {
    minutes=now.minute();
    hours=now.hour(); 
    meniu = 1; //go to menu for change hour
    delay(1000);
  }
bucla = bucla+1;
}  // end usual menu (display hour, temperature and humidity)

if (meniu == 1)
{
 if (!digitalRead(SW1)) // set hours ++
 { 
 hours++;       
 if ((hours & 0x0f) > 9) hours =  hours + 6;
 if (hours > 0x23) hours = 0;   
 delay(100);
 }  
   // converting hexa in zecimal:
    zh = hours / 16;
    uh = hours - 16 * zh ;
    ore = 10 * zh + uh; 
    zm = minutes / 16;
    um = minutes - 16 * zm ;
    miniti = 10 * zm + um; 
  
     for(int i = 20 ; i >0  ; i--) {
  //   displayNumber01(ore*100+miniti); 
  displayNumber20(ore); 
     }      
    delay(5);

 if (!(digitalRead(SW0)))
  {
    meniu = 2; //go to menu for change minute
    delay(1000);
  }
} // end menu for change hour

if (meniu == 2)
{
 if (!digitalRead(SW1)) // set minutes ++
 { 
 minutes++;       
 if ((minutes & 0x0f) > 9) minutes = minutes + 6;
 if (minutes > 0x59) minutes = 0;
 delay(100);
 }  
   // converting hexa in zecimal:
    zh = hours / 16;
    uh = hours - 16 * zh ;
    ore = 10 * zh + uh; 
    zm = minutes / 16;
    um = minutes - 16 * zm ;
    miniti = 10 * zm + um; 
  
     for(int i = 20 ; i >0  ; i--) {
//     displayNumber01(ore*100+miniti); 
    displayNumber21(miniti); 
     }      
    delay(5);

 if (!(digitalRead(SW0)))
  {
    meniu = 3; //go to menu for usual style
    delay(500);
  }
}  // end menu for change minutes

if (meniu == 3)  // store data in RTC
{
    Wire.beginTransmission(0x68); // activate DS1307
    Wire.write(0); // where to begin
    Wire.write(0x00);          //seconds
    Wire.write(minutes);          //minutes
    Wire.write(0x80 | hours);    //hours (24hr time)
    Wire.write(0x06);  // Day 01-07
    Wire.write(0x01);  // Date 0-31
    Wire.write(0x05);  // month 0-12
    Wire.write(0x09);  // Year 00-99
    Wire.write(0x10); // Control 0x10 produces a 1 HZ square wave on pin 7. 
    Wire.endTransmission();
 delay(500);
 meniu = 0; 
}

} // end main program


//Given a number, turns on those segments
//If number == 10, then turn off number
void lightNumber(int numberToDisplay) {
  switch (numberToDisplay){

  case 0:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_OFF);
    break;

  case 1:
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_OFF);
    break;

  case 2:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_OFF);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 3:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 4:
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 5:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 6:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 7:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_OFF);
    break;

  case 8:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 9:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  // all segment are ON
  case 10:
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_OFF);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_OFF);
    break;
 // }
    // degree symbol made by niq_ro
  case 11:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_OFF);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  // C letter made by niq_ro
  case 12:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_OFF);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_OFF);
    break;
  
  // H letter made by niq_ro
  case 13:
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;
 
  // L letter made by niq_ro
  case 14:
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_OFF);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_OFF);
    break;

  // A letter made by niq_ro
  case 15:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;
  
    // F letter made by niq_ro
  case 16:
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_OFF);
    break;
    
  // n letter made by niq_ro
  case 17:
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_ON);
    break;

    // o letter made by niq_ro
  case 18:
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_ON);
    break;
  }
}

// display clock with second on
void displayNumber01(int toDisplay) {
  for(int digit = 4 ; digit > 0 ; digit--) {

    //Turn on a digit for a short amount of time
    switch(digit) {
    case 1:
     digitalWrite(digit1, DIGIT_ON);
     digitalWrite(segDP, SEGMENT_OFF);
      break;
   case 2:
      digitalWrite(digit2, DIGIT_ON);
      digitalWrite(segDP, SEGMENT_ON);
      break;
    case 3:
      digitalWrite(digit3, DIGIT_ON);
      digitalWrite(segDP, SEGMENT_OFF);    
      break;
    case 4:
      digitalWrite(digit4, DIGIT_ON);
      digitalWrite(segDP, SEGMENT_OFF);
      break;
    }
    lightNumber(toDisplay % 10);
    toDisplay /= 10;
    delayMicroseconds(DISPLAY_BRIGHTNESS); 

     //Turn off all segments
    lightNumber(10); 

    //Turn off all digits
    digitalWrite(digit1, DIGIT_OFF);
    digitalWrite(digit2, DIGIT_OFF);
    digitalWrite(digit3, DIGIT_OFF);
    digitalWrite(digit4, DIGIT_OFF);
    digitalWrite(segDP, SEGMENT_OFF);
}
} 

// dsplay clock wih small hour (8:31) with second on
void displayNumber02(int toDisplay) {
  for(int digit = 4 ; digit > 0 ; digit--) {

    //Turn on a digit for a short amount of time
    switch(digit) {
    case 1:
     lightNumber(10); 
     digitalWrite(segDP, SEGMENT_OFF);
     break;
   case 2:
      digitalWrite(digit2, DIGIT_ON);
      digitalWrite(segDP, SEGMENT_ON);
      break;
    case 3:
      digitalWrite(digit3, DIGIT_ON);
      digitalWrite(segDP, SEGMENT_OFF);
      break;
    case 4:
      digitalWrite(digit4, DIGIT_ON);
  digitalWrite(segDP, SEGMENT_OFF);
      break;
    }
    lightNumber(toDisplay % 10);
    toDisplay /= 10;
    delayMicroseconds(DISPLAY_BRIGHTNESS); 

     //Turn off all segments
    lightNumber(10); 

    //Turn off all digits
    digitalWrite(digit1, DIGIT_OFF);
    digitalWrite(digit2, DIGIT_OFF);
    digitalWrite(digit3, DIGIT_OFF);
    digitalWrite(digit4, DIGIT_OFF);
    digitalWrite(segDP, SEGMENT_OFF);
}
} 

// display clock with second off
void displayNumber03(int toDisplay) {
  for(int digit = 4 ; digit > 0 ; digit--) {

    //Turn on a digit for a short amount of time
    switch(digit) {
    case 1:
     digitalWrite(digit1, DIGIT_ON);
        digitalWrite(segDP, SEGMENT_OFF);
      break;
   case 2:
      digitalWrite(digit2, DIGIT_ON);
    digitalWrite(segDP, SEGMENT_OFF);
      break;
    case 3:
      digitalWrite(digit3, DIGIT_ON);
   digitalWrite(segDP, SEGMENT_OFF);
      break;
    case 4:
      digitalWrite(digit4, DIGIT_ON);
 //     digitalWrite(segDP, HIGH);
    digitalWrite(segDP, SEGMENT_OFF);
      break;
    }
    lightNumber(toDisplay % 10);
    toDisplay /= 10;
    delayMicroseconds(DISPLAY_BRIGHTNESS); 

     //Turn off all segments
    lightNumber(10); 

    //Turn off all digits
    digitalWrite(digit1, DIGIT_OFF);
    digitalWrite(digit2, DIGIT_OFF);
    digitalWrite(digit3, DIGIT_OFF);
    digitalWrite(digit4, DIGIT_OFF);
    digitalWrite(segDP, SEGMENT_OFF);
}
} 

// display clock for small hour (8:31) with second off
void displayNumber04(int toDisplay) {
  for(int digit = 4 ; digit > 0 ; digit--) {

    //Turn on a digit for a short amount of time
    switch(digit) {
    case 1:
     lightNumber(10); 
     digitalWrite(segDP, SEGMENT_OFF);
     break;
   case 2:
      digitalWrite(digit2, DIGIT_ON);
    digitalWrite(segDP, SEGMENT_OFF);  
      break;
    case 3:
      digitalWrite(digit3, DIGIT_ON);
 digitalWrite(segDP, SEGMENT_OFF);
      break;
    case 4:
      digitalWrite(digit4, DIGIT_ON);
 digitalWrite(segDP, SEGMENT_OFF);
      break;
    }
    lightNumber(toDisplay % 10);
    toDisplay /= 10;
    delayMicroseconds(DISPLAY_BRIGHTNESS); 

     //Turn off all segments
    lightNumber(10); 

    //Turn off all digits
    digitalWrite(digit1, DIGIT_OFF);
    digitalWrite(digit2, DIGIT_OFF);
    digitalWrite(digit3, DIGIT_OFF);
    digitalWrite(digit4, DIGIT_OFF);
    digitalWrite(segDP, SEGMENT_OFF);
}
} 

// display temperature
void displayNumber(int toDisplay) {
#define DISPLAY_BRIGHTNESS  500

  for(int digit = 4 ; digit > 0 ; digit--) {

    //Turn on a digit for a short amount of time
    switch(digit) {
    case 1:
      digitalWrite(digit1, DIGIT_ON);
      lightNumber(toDisplay % 10);
      toDisplay /= 10;
      delayMicroseconds(DISPLAY_BRIGHTNESS); 
      break;
   case 2:
      digitalWrite(digit2, DIGIT_ON);
      lightNumber(toDisplay % 10);
      toDisplay /= 10;
      delayMicroseconds(DISPLAY_BRIGHTNESS); 
      break;
    case 3:
      digitalWrite(digit3, DIGIT_ON);
      lightNumber(11); // display degree symbol
      delayMicroseconds(DISPLAY_BRIGHTNESS); 
      break;
    case 4:
      digitalWrite(digit4, DIGIT_ON);
      lightNumber(12); // display C letter
      delayMicroseconds(DISPLAY_BRIGHTNESS); 
      break;
    }
     //Turn off all segments
    lightNumber(10); 

    //Turn off all digits
    digitalWrite(digit1, DIGIT_OFF);
    digitalWrite(digit2, DIGIT_OFF);
    digitalWrite(digit3, DIGIT_OFF);
    digitalWrite(digit4, DIGIT_OFF);
}
} 

// display humidity
void displayNumber1(int toDisplay) {
  for(int digit = 4 ; digit > 0 ; digit--) {

    //Turn on a digit for a short amount of time
    switch(digit) {
    case 1:
      digitalWrite(digit1, DIGIT_ON);
      lightNumber(toDisplay % 10);
      toDisplay /= 10;
      delayMicroseconds(DISPLAY_BRIGHTNESS); 
      break;
   case 2:
      digitalWrite(digit2, DIGIT_ON);
      lightNumber(toDisplay % 10);
      toDisplay /= 10;
      delayMicroseconds(DISPLAY_BRIGHTNESS); 
      break;
    case 3:
      digitalWrite(digit3, DIGIT_ON);
      lightNumber(10); // display off
      delayMicroseconds(DISPLAY_BRIGHTNESS); 
      break;
    case 4:
      digitalWrite(digit4, DIGIT_ON);
      lightNumber(13); // display H letter
      delayMicroseconds(DISPLAY_BRIGHTNESS); 
      break;
    }
     //Turn off all segments
    lightNumber(10); 

    //Turn off all digits
    digitalWrite(digit1, DIGIT_OFF);
    digitalWrite(digit2, DIGIT_OFF);
    digitalWrite(digit3, DIGIT_OFF);
    digitalWrite(digit4, DIGIT_OFF);
}
} 

// hour clock adjust
void displayNumber20(int toDisplay) {
  for(int digit = 4 ; digit > 0 ; digit--) {

    //Turn on a digit for a short amount of time
    switch(digit) {
    case 1:
      digitalWrite(digit1, DIGIT_ON);
      lightNumber(toDisplay % 10);
      toDisplay /= 10;
      digitalWrite(segDP, SEGMENT_OFF);
      delayMicroseconds(DISPLAY_BRIGHTNESS); 
      break;
    case 2:
      digitalWrite(digit2, DIGIT_ON);
      lightNumber(toDisplay % 10);
      toDisplay /= 10;
      digitalWrite(segDP, SEGMENT_ON);
      delayMicroseconds(DISPLAY_BRIGHTNESS); 
      break;
    case 3:
      digitalWrite(digit3, DIGIT_ON);
      lightNumber(12);   // C letter 
      digitalWrite(segDP, SEGMENT_OFF);
      delayMicroseconds(DISPLAY_BRIGHTNESS); 
      break;
   case 4:
      digitalWrite(digit4, DIGIT_ON);
      lightNumber(14);   // L letter 
      digitalWrite(segDP, SEGMENT_OFF);
      delayMicroseconds(DISPLAY_BRIGHTNESS); 
      break;
    }
     //Turn off all segments
    lightNumber(10); 

    //Turn off all digits
    digitalWrite(digit1, DIGIT_OFF);
    digitalWrite(digit2, DIGIT_OFF);
    digitalWrite(digit3, DIGIT_OFF);
    digitalWrite(digit4, DIGIT_OFF);
    digitalWrite(segDP, SEGMENT_OFF);
}
} 


// minute clock adjust
void displayNumber21(int toDisplay) {
  for(int digit = 4 ; digit > 0 ; digit--) {

    //Turn on a digit for a short amount of time
    switch(digit) {
    case 1:
      digitalWrite(digit1, DIGIT_ON);
      lightNumber(12);   // C letter 
      digitalWrite(segDP, SEGMENT_OFF);
      delayMicroseconds(DISPLAY_BRIGHTNESS); 
      break;
   case 2:
      digitalWrite(digit2, DIGIT_ON);
      lightNumber(14);   // L letter 
      digitalWrite(segDP, SEGMENT_ON);
      delayMicroseconds(DISPLAY_BRIGHTNESS); 
      break;
    case 3:
      digitalWrite(digit3, DIGIT_ON);
      lightNumber(toDisplay % 10);
      toDisplay /= 10;
      digitalWrite(segDP, SEGMENT_OFF);
      delayMicroseconds(DISPLAY_BRIGHTNESS); 
      break;
    case 4:
      digitalWrite(digit4, DIGIT_ON);
      lightNumber(toDisplay % 10);
      toDisplay /= 10;
      digitalWrite(segDP, SEGMENT_OFF);
      delayMicroseconds(DISPLAY_BRIGHTNESS); 
      break;
    }
     //Turn off all segments
    lightNumber(10); 

    //Turn off all digits
    digitalWrite(digit1, DIGIT_OFF);
    digitalWrite(digit2, DIGIT_OFF);
    digitalWrite(digit3, DIGIT_OFF);
    digitalWrite(digit4, DIGIT_OFF);
    digitalWrite(segDP, SEGMENT_OFF);
}
} 
