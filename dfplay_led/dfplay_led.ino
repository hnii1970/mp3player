/***************************************************
 DFPlayer - A Mini MP3 Player For Arduino
 <https://www.dfrobot.com/index.php?route=product/product&product_id=1121>
 
 ***************************************************
 This example shows the all the function of library for DFPlayer.
 
 Created 2016-12-07
 By [Angelo qiao](Angelo.qiao@dfrobot.com)
 
 GNU Lesser General Public License.
 See <http://www.gnu.org/licenses/> for details.
 All above must be included in any redistribution
 ****************************************************/

/***********Notice and Trouble shooting***************
 1.Connection and Diagram can be found here
<https://www.dfrobot.com/wiki/index.php/DFPlayer_Mini_SKU:DFR0299#Connection_Diagram>
 2.This code is tested on Arduino Uno, Leonardo, Mega boards.
 ****************************************************/
/* h.hnii 2018/8/15 for kenji mp3 player */

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySoftwareSerial(11, 12); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);
#define LED1 3
#define LED2 5
#define LED3 6
#define LED4 9
#define LED5 10
#define SW1 A0
#define SW2 A1
#define SW3 A2
#define SW4 A3
#define SW5 A4

int Switch,Switch_prev;
int Wait, Vol;
void setup()
{
  pinMode(SW1, INPUT_PULLUP);
  pinMode(SW2, INPUT_PULLUP);
  pinMode(SW3, INPUT_PULLUP);
  pinMode(SW4, INPUT_PULLUP);
  pinMode(SW5, INPUT_PULLUP);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  digitalWrite(LED3,1);  
  Switch = 0;Switch_prev=0;
  Wait = 0;Vol = 20;
  mySoftwareSerial.begin(9600);
  Serial.begin(250000);
  
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));
  
  //----Set volume----
  myDFPlayer.volume(10);  //Set volume value (0~30).
  //----Set different EQ----
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
//  myDFPlayer.EQ(DFPLAYER_EQ_POP);
//  myDFPlayer.EQ(DFPLAYER_EQ_ROCK);
//  myDFPlayer.EQ(DFPLAYER_EQ_JAZZ);
//  myDFPlayer.EQ(DFPLAYER_EQ_CLASSIC);
//  myDFPlayer.EQ(DFPLAYER_EQ_BASS);
  
  //----Set device we use SD as default----
//  myDFPlayer.outputDevice(DFPLAYER_DEVICE_U_DISK);
//  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
//  myDFPlayer.outputDevice(DFPLAYER_DEVICE_AUX);
//  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SLEEP);
//  myDFPlayer.outputDevice(DFPLAYER_DEVICE_FLASH);
  
  //----Mp3 control----
//  myDFPlayer.sleep();     //sleep
//  myDFPlayer.reset();     //Reset the module
//  myDFPlayer.enableDAC();  //Enable On-chip DAC
//  myDFPlayer.disableDAC();  //Disable On-chip DAC
//  myDFPlayer.outputSetting(true, 15); //output setting, enable the output and set the gain to 15
  myDFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms
  Serial.println(F("OK"));

}
void test(){
  //----Mp3 play----
  myDFPlayer.next();  //Play next mp3
  delay(1000);
  myDFPlayer.previous();  //Play previous mp3
  delay(1000);
  myDFPlayer.play(1);  //Play the first mp3
  delay(1000);
  myDFPlayer.loop(1);  //Loop the first mp3
  delay(1000);
  myDFPlayer.pause();  //pause the mp3
  delay(1000);
  myDFPlayer.start();  //start the mp3 from the pause
  delay(1000);
  myDFPlayer.playFolder(15, 4);  //play specific mp3 in SD:/15/004.mp3; Folder Name(1~99); File Name(1~255)
  delay(1000);
  myDFPlayer.enableLoopAll(); //loop all mp3 files.
  delay(1000);
  myDFPlayer.disableLoopAll(); //stop loop all mp3 files.
  delay(1000);
  myDFPlayer.playMp3Folder(4); //play specific mp3 in SD:/MP3/0004.mp3; File Name(0~65535)
  delay(1000);
  myDFPlayer.advertise(3); //advertise specific mp3 in SD:/ADVERT/0003.mp3; File Name(0~65535)
  delay(1000);
  myDFPlayer.stopAdvertise(); //stop advertise
  delay(1000);
  myDFPlayer.playLargeFolder(2, 999); //play specific mp3 in SD:/02/004.mp3; Folder Name(1~10); File Name(1~1000)
  delay(1000);
  myDFPlayer.loopFolder(5); //loop all mp3 files in folder SD:/05.
  delay(1000);
  myDFPlayer.randomAll(); //Random play all the mp3.
  delay(1000);
  myDFPlayer.enableLoop(); //enable loop.
  delay(1000);
  myDFPlayer.disableLoop(); //disable loop.
  delay(1000);

  //----Read imformation----
  Serial.println(myDFPlayer.readState()); //read mp3 state
  Serial.println(myDFPlayer.readVolume()); //read current volume
  Serial.println(myDFPlayer.readEQ()); //read EQ setting
  Serial.println(myDFPlayer.readFileCounts()); //read all file counts in SD card
  Serial.println(myDFPlayer.readCurrentFileNumber()); //read current play file number
  Serial.println(myDFPlayer.readFileCountsInFolder(3)); //read fill counts in folder SD:/03
}

void loop()
{
  int i;
  delay(50);
  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }
  //Serial.println(myDFPlayer.readState()); //read mp3 state
  Serial.print(".");
  i = checkSW();
  if(((Wait%20)==0)&&(i==0)) ShowLED(Switch_prev);
  if((i>0)&&(Switch==0)) {
    if(Wait>10) {
      //myDFPlayer.play(i);
      myDFPlayer.volume(Vol);
      myDFPlayer.playFolder(1,i);
      Wait=0;
      Serial.println("PLAY");
    }
  }
  if(i==-2) {
    myDFPlayer.pause();
    Wait=0;
    Switch_prev=0;
    Serial.println("PAUSE");
  } else if(i==-3) {
    myDFPlayer.volume(10);
    Vol = 10;
    Wait=0;
  } else if(i==-4) {
    myDFPlayer.volume(20);
    Vol=20;
    Wait=0;
  } else if(i==-5) {
    Serial.println("roulette");
    ShowRoulette();
    myDFPlayer.playMp3Folder(1);
    delay(1000);
    while((myDFPlayer.readState()&0x01)!=0) {
      delay(1000);
    }
    Wait=0;
  }
  if(i>0) Switch_prev = i;
  Switch= i;
  if(Wait<1001) Wait++;
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
