#include <Boards.h>
#include <Firmata.h>

//blinky
#include <FastLED.h>
#include <animation.h>
#include "angry.h"
#include "bluebackward.h"
#include "blueforward.h"
#include "blueleft.h"
#include "blueright.h"
#include "robohacks.h"
//#include "orangeforward.h"
//#include "orangeleft.h"
//#include "orangeright.h"   //for automated
#include "stop.h"

//range
#define echoPin 13
#define trigPin 12 
#define LEDPin 6 


int RSPD = 5;
int LSPD = 6;
int RDIR = 7;
int LDIR = 8;
//String test[] = {"stpstpstp", "fwdspd255", "fwdrht255", "fwdlft255", "bwdspd255", "bwdrht255", "bwdlft255"};
String dir;
String action;
String pwm;
char buff[9];

int i = 0;

//blinky
#define LED_COUNT 8*8
struct CRGB leds[LED_COUNT];
#define LED_OUT      3  //MODIFY
uint8_t brightness = 64;
Animation* an;
int frameDelay  = 100;

//range
int maximumRange = 50;
int minimumRange = 0;
long duration, distance; 
bool tooClose = false;


void setup() {

  Serial.begin(9600);
  pinMode(RSPD, OUTPUT);
  pinMode(RDIR, OUTPUT);
  pinMode(LSPD, OUTPUT);
  pinMode(LDIR, OUTPUT);

  //blinky
  LEDS.addLeds <WS2812B, LED_OUT, GRB> (leds, LED_COUNT);
  LEDS.showColor(CRGB(0, 0, 0));
  LEDS.setBrightness(brightness);
  LEDS.show();
  an = new Animation (13, robohacksData, Animation::Encoding::RGB24, 64);

  //range
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

}

void loop() {
  
  //range
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration/58.2;
  
  if (distance >= maximumRange || distance <= minimumRange || !(( dir == "fwd")&&(action == "spd")) ){
    tooClose = false;
  }else{
    tooClose = true;
    digitalWrite(RDIR, HIGH);
    digitalWrite(LDIR, HIGH);
    analogWrite(RSPD, 0);
    analogWrite(LSPD, 0);
    
    //blinky
    free(an);
    an = new Animation (2, angryData, Animation::Encoding::RGB24, 64);
    an->draw(leds);
  }

  
  if(Serial.available()>0){
    Serial.readBytes(buff, 9);

    String myo(buff);    

    //myo = test[i%7];
    //i++;
    
    //myo = test;
    dir = myo.substring(0,3);
    action = myo.substring(3,6);
    pwm = myo.substring(6,9);
    Serial.println("here");
    Serial.println(dir);
    Serial.println(action);
    Serial.println(pwm);
    if (dir == "fwd"){
    
      if ((action == "rht")&& !tooClose){
        digitalWrite(RDIR, LOW);
        digitalWrite(LDIR, HIGH);
        analogWrite(RSPD, 153);
        analogWrite(LSPD, 153);
        
        //blinky
        free(an);
        an = new Animation (13, blueRightData, Animation::Encoding::RGB24, 64);
         for (int i=0; i<13;i++){
            an->draw(leds);
            delay(frameDelay);
          }
        
      }else if ((action == "lft") && !tooClose){
        digitalWrite(RDIR, HIGH);
        digitalWrite(LDIR, LOW);
        analogWrite(RSPD, 153);
        analogWrite(LSPD, 153);

        //blinky
        free(an);
        an = new Animation (13, blueLeftData, Animation::Encoding::RGB24, 64);
        for (int i=0; i<13;i++){
            an->draw(leds);
            delay(frameDelay);
        }

      }else if (!tooClose){
        digitalWrite(RDIR, HIGH);
        digitalWrite(LDIR, HIGH);
        analogWrite(RSPD, pwm.toInt());
        analogWrite(LSPD, pwm.toInt());


        //blinky
        free(an);
        an = new Animation (13, blueForwardData, Animation::Encoding::RGB24, 64);
        for (int i=0; i<13;i++){
            an->draw(leds);
            delay(frameDelay);
        }
        
      }
    }else if (dir == "bwd"){
      
      if (action == "rht"){
        digitalWrite(RDIR, HIGH);
        digitalWrite(LDIR, LOW);
        analogWrite(RSPD, 153);
        analogWrite(LSPD, 153);

        //blinky
        free(an);
        an = new Animation (13, blueRightData, Animation::Encoding::RGB24, 64);
        for (int i=0; i<13;i++){
            an->draw(leds);
            delay(frameDelay);
        }        
       
      }else if (action == "lft"){
        digitalWrite(RDIR, LOW);
        digitalWrite(LDIR, HIGH);
        analogWrite(RSPD, 153);
        analogWrite(LSPD, 153);
        
        //blinky
        free(an);
        an = new Animation (13, blueLeftData, Animation::Encoding::RGB24, 64);
        for (int i=0; i<13;i++){
            an->draw(leds);
            delay(frameDelay);
        }

        
      }else{
        digitalWrite(RDIR, LOW);
        digitalWrite(LDIR, LOW);
        analogWrite(RSPD, pwm.toInt());
        analogWrite(LSPD, pwm.toInt());

        //blinky
        free(an);
        an = new Animation (13, blueBackwardData, Animation::Encoding::RGB24, 64);
        for (int i=0; i<13;i++){
            an->draw(leds);
            delay(frameDelay);
        }
        
      }
    }else{
        digitalWrite(RDIR, HIGH);
        digitalWrite(LDIR, HIGH);
        analogWrite(RSPD, 0);
        analogWrite(LSPD, 0);

        //blinky
        free(an);
        an = new Animation (20, stopData, Animation::Encoding::RGB24, 64);
        for (int i=0; i<13;i++){
            an->draw(leds);
            delay(frameDelay);
        }
    }
  }else if (!tooClose){
    //blinky
    free(an);
    an = new Animation (20, robohacksData, Animation::Encoding::RGB24, 64);
    an->draw(leds);
  }


}
