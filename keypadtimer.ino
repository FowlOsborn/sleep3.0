#include "Arduino.h"
#include "Keypad.h"
#include <Servo.h>
#include <LiquidCrystal.h>
#include <Time.h>
#include <TimeLib.h>
#define buzzer 13
LiquidCrystal lcd(A0,A1,A2,10,11,12);
Servo myservo;
int pos = 0;

const byte ROWS = 3; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'#','9','6','3'},
  {'0','8','5','2'},
  {'*','7','4','1'},
};
byte rowPins[ROWS] = {8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
  int decihours=0;
  int hours=0;
  int decimins=0;
  int mins=0;
  int nhours=0;
  char kill='p';
  bool dh=true;
  bool h=true;
  bool starcheck=true;
  bool dm=true;
  bool m=true;
  bool nh=true;
  bool timeset=false;
  bool firstflag=true;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  myservo.attach(9);
  lcd.begin(16,2);
  pinMode(buzzer,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
   if(firstflag){ 
    for (pos = 45; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  firstflag=false;
   }
  char key = keypad.getKey();
  if (key != NO_KEY && (dh || h || dm || m || (nh&&starcheck))){
    bool flag=true;
    if(dh&& flag)
      {decihours=key-'0';
      dh=false;
      flag=false;
    Serial.println(decihours);
      }    
    if(h&& flag)
      {hours=key-'0';
      h=false;
      flag=false;
    Serial.println(hours);
      }
    if(dm&& flag)
      {decimins=key-'0';
      dm=false;
      flag=false;
    Serial.println(decimins);
      }
        if(m&& flag)
      {mins=key-'0';
      m=false;
      flag=false;
    Serial.println(mins);
      }
    if(nh&& flag&& key != '*')
      {nhours=key-'0';
      nh=false;
      flag=false;
    Serial.println(nhours);
      }
     if(starcheck&&flag)
      {setTime(10*decihours+hours,decimins*10+mins,0,11,1,2020);
      starcheck=false;
      flag=false;
    Serial.println("time set");
    Serial.println(10*decihours+hours);
    Serial.println(hour()%10);
    lcd.setCursor(0,0);
  lcd.write("Time:");
  lcd.setCursor(0,1);
  lcd.write('0'+(hour()/10));
  lcd.write('0'+(hour()%10));
  lcd.write(':');
  lcd.write('0'+(minute()/10));
  lcd.write('0'+(minute()%10));
  dh=h=dm=m=nh=starcheck=timeset=true;
      } 
  } 
  if(key==NO_KEY && timeset)
  {
  lcd.setCursor(0,0);
  lcd.write("Time:");
  lcd.setCursor(0,1);
  lcd.write('0'+(hour()/10));
  lcd.write('0'+(hour()%10));
  lcd.write(':');
  lcd.write('0'+(minute()/10));
  lcd.write('0'+(minute()%10));
  }
  if(!h && !dm && !m && !nh)
  {
   /* time_now=millis();
  unsigned long sleeptimer=1000UL*3600*10*decihours+1000UL*3600*hours+1000UL*10*decimins+1000UL*60*mins; *///OVERFLOW, MINS NOT WORKING
  unsigned long waketimer=1000UL*3600*nhours;
  while((hour() < 10*decihours+hours)|| (minute() < decimins*10+mins))
  {
  lcd.setCursor(0,0);
  lcd.write("Time remaining:");
  lcd.setCursor(0,1);
  int tleftmin=60*((10*decihours+hours)-hour())-minute()+(decimins*10+mins);
    int tlefth=tleftmin/60;
    tleftmin=tleftmin%60;
  lcd.write('0'+tlefth/10);
  lcd.write('0'+ tlefth%10);
  lcd.write(':');
  lcd.write('0'+ tleftmin/10);
  lcd.write('0'+tleftmin%10);
  }
  lcd.clear();
  lcd.write("Good Night!");
  delay(3000);
//turn light off
wakeUpServo1();
  for (pos = 45; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  } 
lcd.clear();
delay(waketimer);
delay(2000);
//strobe
  while(kill !='#')
  {
    wakeUpServo1();
    wakeUpBuzzer();
    char key = keypad.getKey();
    if (key != NO_KEY)
    {
      kill=key;
    }
    lcd.clear();
    delay(500);
    lcd.write("WAKE UP WAKE UP");
    lcd.setCursor(0,1);
    lcd.write("WAKE UP WAKE UP");
  }
  for (pos = 45; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  lcd.clear();
  dh=h=dm=m=nh=true;
  kill='p';
}
}

  void wakeUpServo1(){
  for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(1);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 90; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(1);                       // waits 15ms for the servo to reach the position
  }
}

void wakeUpBuzzer(){
  digitalWrite(buzzer, HIGH); // Send 1KHz sound signal...
  delay(500);
  digitalWrite(buzzer, LOW);
  delay(500);
}
