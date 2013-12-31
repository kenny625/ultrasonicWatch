#include <Ultrasonic.h>

#define TRIGGER_PIN1  12
#define ECHO_PIN1     13
#define TRIGGER_PIN2  10
#define ECHO_PIN2     11
#define TRIGGER_PIN3  8
#define ECHO_PIN3     9

Ultrasonic ultrasonic1(TRIGGER_PIN1, ECHO_PIN1);
Ultrasonic ultrasonic2(TRIGGER_PIN2, ECHO_PIN2);
Ultrasonic ultrasonic3(TRIGGER_PIN3, ECHO_PIN3);

int RECV_PIN=2;
int motor = 3;
int PIN_POSITION[3] = {3,5,6};
int DELAY_BASE = 20;
//finger: 0:left, 1:middle, 2:right
int fingerCount[3];
int fingerCurrentType[3];
int fingerInputType[3];
int fingerRead[3];
int keyRead[3];
int buttonPin = A5;
int buttonValue = 0;
int mode = 0; //0 for xtion, 1 for ultrasonic
int ledPin = 2;

void setup()
  {
  Serial.begin(9600);
  for (int i = 0; i <3; i++){
    fingerCount[i]=0;
    fingerCurrentType[i]=-1;
  }
  }

void loop()
  {
    delay(100);
  float cmMsec1, cmMsec2, cmMsec3;
  float IRvalue1 = analogRead(A0);
  float IRvalue2 = analogRead(A1);
  float IRvalue3 = analogRead(A2);
  long microsec1 = ultrasonic1.timing();
  long microsec2 = ultrasonic2.timing();
  long microsec3 = ultrasonic3.timing();
  
  float ultrasonicCM[3];
  
  ultrasonicCM[0] = ultrasonic1.convert(microsec1, Ultrasonic::CM);
  ultrasonicCM[1] = ultrasonic2.convert(microsec2, Ultrasonic::CM);
  ultrasonicCM[2] = ultrasonic3.convert(microsec3, Ultrasonic::CM);
  
  buttonValue = analogRead(buttonPin);
  if(buttonValue < 600){
    // button pushed
    Serial.println("pushed");
    Serial.write('z');
  }
  Serial.println("button");
  Serial.println(buttonValue);

//  cmMsec1 = ultrasonic1.convert(microsec1, Ultrasonic::CM);
//  cmMsec2 = ultrasonic2.convert(microsec2, Ultrasonic::CM);
//  cmMsec3 = ultrasonic3.convert(microsec3, Ultrasonic::CM);
  Serial.print("CM1: ");
  Serial.println(ultrasonicCM[0]);
  Serial.print("CM2: ");
  Serial.println(ultrasonicCM[1]);
  Serial.print("CM3: ");
  Serial.println(ultrasonicCM[2]);
  Serial.print("IRvalue1: ");
  Serial.println(IRvalue1);
  Serial.print("IRvalue2: ");
  Serial.println(IRvalue2);
  Serial.print("IRvalue3: ");
  Serial.println(IRvalue3);

//  delay(1000);
    for(int i = 0; i < 3; i++){
      keyRead[i] = Serial.read();
      Serial.println(keyRead[i]);
      Serial.print("type ");
      Serial.println(fingerCurrentType[i]);
    }
    
    if(IRvalue1 < 800 || IRvalue2 < 800 || IRvalue3 < 800){
    digitalWrite(ledPin, LOW);
    for(int i = 0; i < 3; i++){
      if(ultrasonicCM[2-i] <= 50){
        keyRead[i] = 97 + i*4;
      }else if(ultrasonicCM[2-i] <= 100){
        keyRead[i] = 98 + i*4;
      }else if(ultrasonicCM[2-i] <= 150){
        keyRead[i] = 99 + i*4;
      }else{
        keyRead[i] = 100 + i*4;
      }
    
      
      Serial.println(keyRead[i]);
      Serial.print("type ");
      Serial.println(fingerCurrentType[i]);
    }
  }else{
    digitalWrite(ledPin, HIGH);
  }
    
    for(int i = 0; i < 3; i++){
      if( keyRead[i] >= 97 && keyRead[i] <= 100 ){
        fingerRead[2] =  keyRead[i] - 97;
      }else if ( keyRead[i]>=101 && keyRead[i]<=104 ){
        fingerRead[1] =  keyRead[i] - 101;
      }else if ( keyRead[i]>=105 && keyRead[i]<=108 ){
        fingerRead[0] = keyRead[i] - 105;
      }
    }
    for(int i = 0; i<3; i++){
      if( fingerCurrentType[i] == fingerRead[i] ){//一樣的時候，就能繼續計時
        fingerCount[i] ++;
       
        if( fingerRead[i] == 2 ){
          if( fingerCount[i] <= DELAY_BASE*.5){
            analogWrite(PIN_POSITION[i], 0);
          }else if( fingerCount[i] <= DELAY_BASE*.6){
            analogWrite(PIN_POSITION[i], 250); 
          }else if( fingerCount[i] <= DELAY_BASE ){
            analogWrite(PIN_POSITION[i], 0);
          }else{
            fingerCount[i] = 0;
          }
        }else if( fingerRead[i] == 1 ){
          
          if( fingerCount[i] <= DELAY_BASE*.2){
            analogWrite(PIN_POSITION[i], 0);
          }else if( fingerCount[i] > DELAY_BASE*.2 && fingerCount[i] <= DELAY_BASE*.3){
            analogWrite(PIN_POSITION[i], 250); 
          }else if( fingerCount[i] > DELAY_BASE*.3 && fingerCount[i] <= DELAY_BASE*.7){
            analogWrite(PIN_POSITION[i], 0);
          }else if( fingerCount[i] > DELAY_BASE*.7 && fingerCount[i] <= DELAY_BASE*.8){
            analogWrite(PIN_POSITION[i], 250); 
          }else if( fingerCount[i] > DELAY_BASE*.8 && fingerCount[i] <= DELAY_BASE){
            analogWrite(PIN_POSITION[i], 0);
          }else{
            fingerCount[i] = 0;
          }
        }else if( fingerRead[i] == 0 ){
          
          if( fingerCount[i] <= DELAY_BASE*.1){
            analogWrite(PIN_POSITION[i], 0);
          }else if( fingerCount[i] > DELAY_BASE*.1 && fingerCount[i] <= DELAY_BASE*.2){
            analogWrite(PIN_POSITION[i], 250); 
          }else if( fingerCount[i] > DELAY_BASE*.2 && fingerCount[i] <= DELAY_BASE*.3){
            analogWrite(PIN_POSITION[i], 0);
          }else if( fingerCount[i] > DELAY_BASE*.3 && fingerCount[i] <= DELAY_BASE*.4){
            analogWrite(PIN_POSITION[i], 250); 
          }else if( fingerCount[i] > DELAY_BASE*.4 && fingerCount[i] <= DELAY_BASE*.5){
            analogWrite(PIN_POSITION[i], 0);
          }else if( fingerCount[i] > DELAY_BASE*.5 && fingerCount[i] <= DELAY_BASE*.6){
            analogWrite(PIN_POSITION[i], 250); 
          }else if( fingerCount[i] > DELAY_BASE*.6 && fingerCount[i] <= DELAY_BASE*.7){
            analogWrite(PIN_POSITION[i], 0);
          }else if( fingerCount[i] > DELAY_BASE*.7 && fingerCount[i] <= DELAY_BASE*.8){
            analogWrite(PIN_POSITION[i], 250); 
          }else if( fingerCount[i] > DELAY_BASE*.8 && fingerCount[i] <= DELAY_BASE*.9){
            analogWrite(PIN_POSITION[i], 0);
          }else{
            fingerCount[i] = 0;
          }
          
          
        }else if( fingerRead[i] == 3 ){
            analogWrite(PIN_POSITION[i], 0);
        }

      }else{//不一樣的話，就換新的計時
        fingerCount[i] = 0;
        fingerCurrentType[i] = fingerRead[i];
        analogWrite(PIN_POSITION[i], 0);
        Serial.println("new count");
      }
    }
    
  }
