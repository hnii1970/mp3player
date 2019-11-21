/** VIVIPARTS MP3 player
 *  LOOP == 1: loop a sound file.
 *  write to SDcard under "01" directory
 *  the name of sound is "001.mp3" to "005.mp3"
 */

#define SERIAL_DFPLAYER /// enable connection of DFPLAYERmini if DFplayer mini is connected, or debug print
#define LED_ENABLE      /// enable this symbol if it shows the status of DFplayerMINI. light on: OK, blink: init fail

/* h.hnii 2018/8/15 for kenji mp3 player */

#include "Arduino.h"

#ifdef SERIAL_DFPLAYER
#include "DFRobotDFPlayerMini.h"
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);
#endif

#define LED1 3
#define LED2 5
#define LED3 6
#define LED4 9
#define LED5 10
#define DEBUG_LED 13
#define SW1 A0
#define SW2 A1
#define SW3 A2
#define SW4 A3
#define SW5 A4
#define PWR 2
#define BUSY_PIN A5

int Switch,Switch_prev;
int Wait, Vol;
void setup()
{
  pinMode(SW1, INPUT_PULLUP);
  pinMode(SW2, INPUT_PULLUP);
  pinMode(SW3, INPUT_PULLUP);
  pinMode(SW4, INPUT_PULLUP);
  pinMode(SW5, INPUT_PULLUP);
  pinMode(BUSY_PIN, INPUT_PULLUP);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(PWR, OUTPUT);
  digitalWrite(PWR, LOW);
  digitalWrite(LED3,1);  
#ifdef SERIAL_DFPLAYER
  Serial1.begin(9600);
#endif
#ifdef LED_ENABLE
  pinMode(DEBUG_LED,OUTPUT);
  digitalWrite(DEBUG_LED,LOW);
#endif
  Switch = 0;Switch_prev=0;
  Wait = 0;Vol = 20;
  Serial.begin(250000);
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
#ifdef SERIAL_DFPLAYER
  Serial.println("Initializing DFPlayer ... (May take 3~5 seconds)");
  delay(1000);
  if (!myDFPlayer.begin(Serial1)) {  //Use Serial1 to communicate with mp3.
    Serial.println("Unable to begin:");
    Serial.println("1.Please recheck the connection!");
    Serial.println("2.Please insert the SD card!");
    while(true) {
#ifdef LED_ENABLE
      digitalWrite(DEBUG_LED,HIGH);
      delay(1000);
      digitalWrite(DEBUG_LED,LOW);
      delay(1000);
#endif
    };
  }
#endif
  Serial.println(F("DFPlayer Mini online."));
#ifdef SERIAL_DFPLAYER
  //----Set volume----
  myDFPlayer.volume(10);  //Set volume value (0~30).
#endif
#ifdef SERIAL_DFPLAYER
  myDFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms
  Serial.println("OK");
#endif
 
}


void loop()
{
  int i;
  delay(50);
#ifdef SERIAL_DFPLAYER
  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }
#endif
  //Serial.println(myDFPlayer.readState()); //read mp3 state
  Serial.print(".");
  i = checkSW();
  if(((Wait%20)==0)&&(i==0)) ShowLED(Switch_prev);
  if((i>0)&&(Switch==0)) {
    if(Wait>10) {
      //myDFPlayer.play(i);
#ifdef SERIAL_DFPLAYER
      myDFPlayer.volume(Vol);
      myDFPlayer.playFolder(1,i);
#endif
      Wait=0;
      Serial.println("PLAY");
    }
  }
  if(i==-2) {
#ifdef SERIAL_DFPLAYER
    myDFPlayer.pause();
#endif
    Wait=0;
    Switch_prev=0;
    Serial.println("PAUSE");
  } else if(i==-3) {
#ifdef SERIAL_DFPLAYER
    myDFPlayer.volume(10);
#endif
    Vol = 10;
    Wait=0;
  } else if(i==-4) {
#ifdef SERIAL_DFPLAYER
    myDFPlayer.volume(20);
#endif
    Vol=20;
    Wait=0;
  } else if(i==-5) {
    Serial.println("roulette");
    ShowRoulette();
#ifdef SERIAL_DFPLAYER
    myDFPlayer.playMp3Folder(1);
    delay(1000);
    while((myDFPlayer.readState()&0x01)!=0) {
      delay(1000);
    }
#endif
    Wait=0;
  }
  if(i>0) Switch_prev = i;
  Switch= i;
  if(Wait<1001) {
    Wait++;
  } else {
    Serial.println("OFF");
    digitalWrite(PWR, HIGH);
    while(checkSW()==0) {
        delay(1000); 
        Serial.print("x");
    }
    digitalWrite(PWR, LOW);
  }
}

int checkSW() {
  int i,j;
  i = 0;
  j = 0;
  if(digitalRead(SW1)==LOW) {
    i = 1;
    j = j + 1;
    Serial.println("1");
    analogWrite(LED1, 64);
  } else {
    digitalWrite(LED1, LOW);
  }
  if(digitalRead(SW2)==LOW) {
    j = j + 1;
    i = 2;
    digitalWrite(LED2, HIGH);
  } else {
    digitalWrite(LED2, LOW);
  }
  if(digitalRead(SW3)==LOW) {
    j = j + 1;
    i = 3;
    digitalWrite(LED3, HIGH);
  } else {
    digitalWrite(LED3, LOW);
  }
  if(digitalRead(SW4)==LOW) {
    j = j + 1;
    i = 4;
    analogWrite(LED4, 32);
  } else {
    digitalWrite(LED4, LOW);
  }
  if(digitalRead(SW5)==LOW) {
    j = j + 1;
    i = 5;
    analogWrite(LED5, 127);
  } else {
    digitalWrite(LED5, LOW);
  }
  if(j>1) i = -j;
  return i;
}

void ShowLED(int num) {
  switch(num) {
    case 1:
      analogWrite(LED1, 64);
      break;
    case 2:
      digitalWrite(LED2, HIGH);
      break;
    case 3:
      digitalWrite(LED3, HIGH);
      break;
    case 4:
      analogWrite(LED4, 32);
      break;
    case 5:
      analogWrite(LED5, 127);
      break;
    default:
      digitalWrite(LED1,LOW);              
      digitalWrite(LED2,LOW);              
      digitalWrite(LED3,LOW);              
      digitalWrite(LED4,LOW);              
      digitalWrite(LED5,LOW);
  }
}
void ShowRoulette() {
  int i,ma,de;
  ma = random(30,50);
  for(i=0; i<ma; i++) {
    ShowLED(0);
    ShowLED(i%5+1);
    de = 100;
    if(i> ma - random(1,5)) de = 300;
    delay(de);
  }
  ShowLED(0);
  ShowLED(i%5+1);
}
#ifdef SERIAL_DFPLAYER
void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}
#endif
