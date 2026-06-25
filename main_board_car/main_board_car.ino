
#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFiClient.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Stepper.h>
#include <Firebase_ESP_Client.h>
//#include <SPI.h>
//#include <SD.h>

// #include <EEPROM.h>
// #define EEPROM_SIZE 512

const int stepsPerRevolution=200;

WebServer server(80);

#define FIREBASE_HOST "https://vipcar-v1-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "O1ZcCeCnVjrqX4J44FEygwIpyrpEVI5ZjJxRIwFw"
FirebaseData fbdo;
FirebaseConfig config;
FirebaseAuth auth;

#define ser 14
#define clk 12
#define latch 15
#define shld 16
#define clk2 18
#define clkin 17
#define qh 13

#include <stdio.h>
#include <stdlib.h>

Adafruit_PWMServoDriver pwm;
const int serverPort = 80; // Port number for the web server

const int stepPin = 22; // توصيلة الخطوة (Step pin)
const int dirPin = 23;  // توصيلة الاتجاه (Direction pin)
const int ena1=27;
//const int ena2=2;
// const int ena3=23;
 const int ena4=2;
const int ena5=21;
bool dir=true;
bool endstop1=0;
bool endstop_1=0;
bool  shuttersWork=0;
int   firebaseMillis=0;
bool value=false;
bool esp=true;
/////////////switchfunction///////////////
bool whiteLight=0;
bool deepLed=1;
bool DirTv=0;
bool activeTv=false;
int  boolshutter1=1;
int  boolshutter2=1;
int  boolshutter3=1;
int  boolshutter4=1;
int  boolshutter5=1;
int boolWhiteColor=1;
int boolrgbColor=1;

int boolTv=1;
int boolTvLoop=1;
/////////////////////////////
// Define the servo settings
const int servoPin1 = 0;
const int servoPin2 = 1;
const int servoPin3 = 2;
const int servoPin4 = 3;
const int servoPin5 = 4; // The PCA9685 channel connected to the servo
const int servoMin = 150; // Minimum servo pulse length
const int servoMax = 600; // Maximum servo pulse lengt
////
 byte rec1;
 byte rec2;
 byte rec3;
 byte rec4;
 byte rec5;

//////////////////////
 
unsigned long previousStepTime = 0;
const unsigned long stepDelay = 600;  // تأخير بين كل خطوة

int currentStep = 0;  // المتغير الذي يحسب عدد الخطوات
const int totalSteps = 9000;

bool movingForward = false;  // معرفة اتجاه الحركة
bool motorActive = false;    // لمعرفة حالة تشغيل المحرك

////////////////////////

int times=0;

bool in[40];

bool stopDownTable1=in[0];
bool stopUpTable1=in[1];

bool stopDownTable2=in[2];
bool stopUpTable2=in[3];

bool stopDownTable3=in[4];
bool stopUpTable3=in[5];

bool stopDownTable4=in[6];
bool stopUpTable4=in[7];

bool stopDownTable5=in[8];
bool stopUpTable5=in[9];

bool tUp1=0;
bool tDown1=0;

bool tUp2=0;
bool tDown2=0;

bool tUp3=0;
bool tDown3=0;

bool tUp4=0;
bool tDown4=0;

bool tUp5=0;
bool tDown5=0;

int C=0;
 String output1;
byte p1=B00000000;
byte p2=B00000000;
byte p3=B00000000;
byte p4=B00000000;
byte p5=B00000000;
//#define D1 (p5,4)
#define pin1 22
#define pin2 23
#define pin3 27
#define pin4 32

/////////////////////////////////////////////////////////////
Stepper myStepper1(stepsPerRevolution,pin1,pin2,pin3,pin4);
// Stepper myStepper2(stepsPerRevolution,4,5,19,21);
// Stepper myStepper3(stepsPerRevolution,2,33,35,34);
// Stepper myStepper4(stepsPerRevolution,3,0,25,26);


int stepCount1=0;
int stepCount2=0;
int stepCount3=0;
int stepCount4=0;

int dirStep1=1;
int dirStep2=500;
int dirStep3=500;
int dirStep4=500;
//////////////////////////////////////////////////////////////
int t1=millis();
String name1="";

int k=0;
 int L=0;
 int R=0;
int n=0;
double red=0;
double green=0;
double blue=0;
int r=0; 
int g=0; 
int b=0;
int kd=10;
int grad=0;
  //int kolordev=10;
 //int delyk=100;
long kolors[] = {
    0xFFFF0000, 0xFFFF1100, 0xFFFF2200, 0xFFFF3300, 0xFFFF4400,
    0xFFFF5500, 0xFFFF6600, 0xFFFF7700, 0xFFFF8800, 0xFFFF9900,
    0xFFFF0033, 0xFFFF0066, 0xFFFF0099, 0xFFFF00CC, 0xFFFF00FF,
    0xFFCC00FF, 0xFF9900FF, 0xFF6600FF, 0xFF3300FF, 0xFF0000FF,
    0xFFFF0033, 0xFFFF0066, 0xFFFF0099, 0xFFFF00CC, 0xFFFF00FF,
    0xFFFF0066, 0xFFFF0099, 0xFFFF00CC, 0xFFFF00FF, 0xFFCC00FF, 0xFF9900FF,0xFF6600FF,
    0xFFFF00CC, 0xFFFF00FF, 0xFFCC00FF, 0xFF9900FF, 0xFF6600FF, 0xFF3300FF,0xFF0000FF
};
 /////////////////////////////////////////////////////////////////////////
  String   IPaddress;

    
#ifndef STASSID
#define STASSID  "Galaxy A26"// "ZTE_A9E74E"
#define STAPSK "solidworks" // "68549470" 
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;
 
/////////////////////////////////////////////////////////////////////
#define SDA_PIN 25
#define SCL_PIN 26
// // Interrupt service routine code
// const int interruptPin1 = GPIO_NUM_34;
// const int interruptPin2 = GPIO_NUM_35;
// // Interrupt service routine for RISING
// void IRAM_ATTR handleRisingInterrupt() {
 
// }
// // Interrupt service routine for FALLING
// void IRAM_ATTR handleFallingInterrupt() {
 
// }
////////////////////////////
 
void writeOutput(){
  
 digitalWrite(latch,LOW);
shiftOut(ser,clk,MSBFIRST,p5);
shiftOut(ser,clk,MSBFIRST,p4);
shiftOut(ser,clk,MSBFIRST,p3);
shiftOut(ser,clk,MSBFIRST,p2);
shiftOut(ser,clk,MSBFIRST,p1);
digitalWrite(latch,HIGH);
  Serial.println();
  Serial.print("output  ");
  for(int i=0; i<8; i++){
    bool k=bitRead(p1,i);
    
     Serial.print(k);
  }
  
     Serial.print("   ");

     for(int i=0; i<8; i++){
    bool k=bitRead(p2,i);
     Serial.print(k);
  }
     Serial.print("   ");

   for(int i=0; i<8; i++){
    bool k=bitRead(p3,i);
     Serial.print(k);
  }
     Serial.print("   ");

   for(int i=0; i<8; i++){
    bool k=bitRead(p4,i);
     Serial.print(k);
  }
   Serial.print("   ");
   for(int i=0; i<8; i++){
    bool k=bitRead(p5,i);

     Serial.print(k);
  }
}
void inputBus1(){
  digitalWrite(shld,LOW);
 delayMicroseconds(5); 
 digitalWrite(shld,HIGH);
  delayMicroseconds(5); 
 digitalWrite(clkin,HIGH);
 digitalWrite(clk2,HIGH);
 digitalWrite(clkin,LOW);
 

byte rec1=shiftIn(qh,clk2,MSBFIRST);
byte rec2=shiftIn(qh,clk2,MSBFIRST);
byte rec3=shiftIn(qh,clk2,MSBFIRST);
byte rec4=shiftIn(qh,clk2,MSBFIRST);
byte rec5=shiftIn(qh,clk2,MSBFIRST);
 
  
  for (int i=0;i<8;i++)
  {
   bool b= bitRead(rec1,i);
  
    in[i]= b;
    
   
    }
}
void inputBus2(){
  digitalWrite(shld,LOW);
 delayMicroseconds(5); 
 digitalWrite(shld,HIGH);
  delayMicroseconds(5); 
 digitalWrite(clkin,HIGH);
 digitalWrite(clk2,HIGH);
 digitalWrite(clkin,LOW);
 

byte rec1=shiftIn(qh,clk2,MSBFIRST);
byte rec2=shiftIn(qh,clk2,MSBFIRST);
byte rec3=shiftIn(qh,clk2,MSBFIRST);
byte rec4=shiftIn(qh,clk2,MSBFIRST);
byte rec5=shiftIn(qh,clk2,MSBFIRST);
 
  
  for (int i=0;i<8;i++)
  {
   bool b= bitRead(rec2,i);
  
    in[i+8]= b;
    
   
    }
   
}
void inputBus3(){
  digitalWrite(shld,LOW);
 delayMicroseconds(5); 
 digitalWrite(shld,HIGH);
  delayMicroseconds(5); 
 digitalWrite(clkin,HIGH);
 digitalWrite(clk2,HIGH);
 digitalWrite(clkin,LOW);
 

byte rec1=shiftIn(qh,clk2,MSBFIRST);
byte rec2=shiftIn(qh,clk2,MSBFIRST);
byte rec3=shiftIn(qh,clk2,MSBFIRST);
byte rec4=shiftIn(qh,clk2,MSBFIRST);
byte rec5=shiftIn(qh,clk2,MSBFIRST);
 
  
  for (int i=0;i<8;i++)
  {
   bool b= bitRead(rec3,i);
  
    in[i+16]= b;
    
   
    }
}
void inputBus4(){
  digitalWrite(shld,LOW);
 delayMicroseconds(5); 
 digitalWrite(shld,HIGH);
  delayMicroseconds(5); 
 digitalWrite(clkin,HIGH);
 digitalWrite(clk2,HIGH);
 digitalWrite(clkin,LOW);
 

byte rec1=shiftIn(qh,clk2,MSBFIRST);
byte rec2=shiftIn(qh,clk2,MSBFIRST);
byte rec3=shiftIn(qh,clk2,MSBFIRST);
byte rec4=shiftIn(qh,clk2,MSBFIRST);
byte rec5=shiftIn(qh,clk2,MSBFIRST);
 
  
  for (int i=0;i<8;i++)
  {
   bool b= bitRead(rec4,i);
  
    in[i+24]= b;
    
   
    }
}
void inputBus5(){
  digitalWrite(shld,LOW);
 delayMicroseconds(5); 
 digitalWrite(shld,HIGH);
  delayMicroseconds(5); 
 digitalWrite(clkin,HIGH);
 digitalWrite(clk2,HIGH);
 digitalWrite(clkin,LOW);
 

byte rec1=shiftIn(qh,clk2,MSBFIRST);
byte rec2=shiftIn(qh,clk2,MSBFIRST);
byte rec3=shiftIn(qh,clk2,MSBFIRST);
byte rec4=shiftIn(qh,clk2,MSBFIRST);
byte rec5=shiftIn(qh,clk2,MSBFIRST);
 
  
  for (int i=0;i<8;i++)
  {
   bool b= bitRead(rec5,i);
  
    in[i+32]= b;
    
   
    }
}
 void inputBus(){ 
 digitalWrite(shld,LOW);
 delayMicroseconds(5); 
 digitalWrite(shld,HIGH);
  delayMicroseconds(5); 
 digitalWrite(clkin,HIGH);
 digitalWrite(clk2,HIGH);
 digitalWrite(clkin,LOW);
 

byte rec1=shiftIn(qh,clk2,MSBFIRST);
byte rec2=shiftIn(qh,clk2,MSBFIRST);
byte rec3=shiftIn(qh,clk2,MSBFIRST);
byte rec4=shiftIn(qh,clk2,MSBFIRST);
byte rec5=shiftIn(qh,clk2,MSBFIRST);

   Serial.println();
    Serial.println();
 Serial.print("input   ");
  for (int i=0;i<8;i++)
  {
   bool b= bitRead(rec1,i);
  
    in[i]= b;
    
    Serial.print(b);
    }
       Serial.print("   ");

 for (int i=0;i<8;i++)
  {
    bool b= bitRead(rec2,i);
    
    in[i+8]= b;
    
    Serial.print(b);
    }
       Serial.print("   ");

 for (int i=0;i<8;i++)
  {
    bool b= bitRead(rec3,i);
  
    in[i+16]= b;
    
     Serial.print(b);
    }
      Serial.print("   ");

    for (int i=0;i<8;i++)
  {
    bool b= bitRead(rec4,i);
   
    in[i+24]= b;
    
     Serial.print(b);
    }
       Serial.print("   ");

     for (int i=0;i<8;i++)
  {
    bool b= bitRead(rec5,i);
    
    in[i+32]= b;
    
     Serial.print(b);
          
    }
      Serial.print("   ");        
 }
/////////////////////////////////////////swichFun()//////////////////////////////////////////////

 void swichFun(){
  ////////////   TV    ////////////////////
  if(in[23]==1){
  delay(300);
  if(in[23]==1){
   handleActiveTv();
  }
  }
  ////////////tables/////////////////
  if(in[15]==1){
    
    handlespresso();
    }
   if(in[27]==1){
    
    handletable1();}
              //// ////
   if(in[28]==1){
   handletable2();
   }
               ////////
   if(in[31]==1){
   handletable3();
    }
              ////////
   if(in[34]==1){
   handletable4();
   }
   ///////////////////////////////////////


         ///////////shutter///////////
  if(in[22]==1){
 switch (boolshutter1){
  case 1:
  handleshr1();
     boolshutter1++;
    break;
  case 2:
     handleshs1();
         boolshutter1++;
    break;
  case 3:     
   
     handleshl1();    
      boolshutter1++;
   break;
    case 4:
     handleshs1();
         boolshutter1=1;
    break;
   default:
   break;
      
  }
   delay(500);
  }
      ///////////////////
   if(in[26]==1){
 switch (boolshutter2){
  case 1:
  handleshr2();
     boolshutter2++;
    break;
  case 2:
     handleshs2();
         boolshutter2++;
    break;
  case 3:     
   
     handleshl2();    
      boolshutter2++;
   break;
    case 4:
     handleshs2();
         boolshutter2=1;
    break;
   default:
   break;
      
  }
   delay(500);
 }
 ////////////////
  if(in[29]==1){
     switch (boolshutter3){
  case 1:
  handleshr3();
     boolshutter3++;
    break;
  case 2:
     handleshs3();
         boolshutter3++;
    break;
  case 3:     
   
     handleshl3();    
      boolshutter3++;
   break;
   case 4:
     handleshs3();
         boolshutter3=1;
    break;
   default:
   break;
      
  }
   delay(500);
 }
 ////////////
  if(in[32]==1){
     switch (boolshutter4){
  case 1:
  handleshr4();
     boolshutter4++;
    break;
  case 2:
     handleshs4();
         boolshutter4++;
    break;
  case 3:     
   
     handleshl4();    
      boolshutter4++;
   break;
   case 4:
     handleshs4();
         boolshutter4=1;
    break;
   default:
   break;
      
  }
   delay(500);
 }
 //////////////
  if(in[30]==1 || in[33]==1){
     switch (boolshutter5){
  case 1:
  handleshr5();
     boolshutter5++;
    break;
  case 2:
     handleshs5();
         boolshutter5++;
    break;
  case 3:     
   
     handleshl5();    
      boolshutter5++;
   break;
    case 4:
     handleshs5();
         boolshutter5=1;
    break;
   default:
   break;
      
  }
   delay(500);
 }
 /////////////////////////////////////lights////////////////////////////////////
  if(in[24]==1) {
    grad=0;
    switch (boolWhiteColor){
    case 1:
     bitWrite(p1,0,1);
     bitWrite(p1,1,1);
     bitWrite(p1,2,0);
      analogWrite(4, 0);
      analogWrite(5, 0);
      analogWrite(19, 0);
      boolWhiteColor++;
      break;
      case 2:
       bitWrite(p1,0,0);
     bitWrite(p1,1,0);
     bitWrite(p1,2,0);
      analogWrite(4, 0);
      analogWrite(5, 0);
      analogWrite(19, 0);
      boolWhiteColor++;
     break;
     case 3:
      bitWrite(p1,0,0);
     bitWrite(p1,1,0);
     bitWrite(p1,2,1);
      analogWrite(4, 0);
      analogWrite(5, 0);
      analogWrite(19, 0);
      boolWhiteColor++;
      break;
       case 4:
      bitWrite(p1,0,1);
     bitWrite(p1,1,1);
     bitWrite(p1,2,1);
      analogWrite(4, 0);
      analogWrite(5, 0);
      analogWrite(19, 0);
      boolWhiteColor=1;
      break;
      default:
      break;
    }
 //whiteLight=!whiteLight;
 
 delay(500);
  }
           /////////////////
    if(in[25]==1)  {
      grad=0;
      bitWrite(p1,0,0); 
       bitWrite(p1,2,1); 
  switch (boolrgbColor){
    case 1:
  analogWrite(4, 255);
  analogWrite(5, 0);
  analogWrite(19, 0);
  boolrgbColor++;
   break;
   case 2:    
  analogWrite(4, 255);
  analogWrite(5, 255);
  analogWrite(19, 0);
  boolrgbColor++;
   break;
   case 3:
  analogWrite(4, 0);
  analogWrite(5, 255);
  analogWrite(19, 0);
  boolrgbColor++;
   break;
   case 4:
  analogWrite(4, 0);
  analogWrite(5, 255);
  analogWrite(19, 255);
  boolrgbColor++;
  break;
  case 5:
  analogWrite(4, 0);
  analogWrite(5, 0);
  analogWrite(19, 255);
  boolrgbColor++;
   break;
   case 6:
  analogWrite(4, 255);
  analogWrite(5, 0);
  analogWrite(19, 255);
  boolrgbColor++;
    break;
    case 7:
  analogWrite(4, 0);
  analogWrite(5, 0);
  analogWrite(19, 0);
   bitWrite(p1,2,0); 
  boolrgbColor=1;
   break;
   
   default:
   break;
    } 
    delay(500);    
 }
 }
 void handledeepLight(){
  bitWrite(p1,2,deepLed);
  deepLed=!deepLed;
 }

void handlelighton(){
  analogWrite(4, 0);
  analogWrite(5, 0);
  analogWrite(19, 0);  
  bitWrite(p1,0,1);
  bitWrite(p1,1,1);
  bitWrite(p1,2,0);
 grad=0;
 
 whiteLight=0;
}
void handlelightoff(){
  analogWrite(4, 0);
  analogWrite(5, 0);
  analogWrite(19, 0); 
  bitWrite(p1,0,0);
  bitWrite(p1,1,0);
  bitWrite(p1,2,0);
 grad=0;
  
 whiteLight=1;
}
void handlegrad(){
   bitWrite(p1,0,0);
   bitWrite(p1,1,0);
  bitWrite(p1,2,1);
  grad=1;
  
  analogWrite(4, 0);
  analogWrite(5, 0);
  analogWrite(19, 0);
  for(int r=0; r<256; r++){
    g=0;
    b=0;
        
  analogWrite(4, r);
  analogWrite(5, g);
  analogWrite(19, b);
  delay(kd);
    
  }
      for(int g=0; g<256; g++){
        r=255;
        b=0;
       
  analogWrite(4, r);
  analogWrite(5, g);
  analogWrite(19, b);
  delay(kd);
    
  }
          for(int r=255; r>=0; r--){
            g=255;
            b=0;
          
  analogWrite(4, r);
  analogWrite(5, g);
  analogWrite(19, b);
  delay(kd);
    
  }
              for(int b=0; b<256; b++){
                g=255;
                r=0;
              
  analogWrite(4, r);
  analogWrite(5, g);
  analogWrite(19, b);
  delay(kd);
    
  }
                  for(int g=255; g>=0; g--){
                    r=0;
                    b=255;
                  
  analogWrite(4, r);
  analogWrite(5, g);
  analogWrite(19, b);
  delay(kd);
    
  }
                      for(int r=0; r<256; r++){
                        g=0;
                       b=255;
                      
  analogWrite(4, r);
  analogWrite(5, g);
  analogWrite(19, b);
  delay(kd);
    
  }
                       for(int b=255; b>=30; b--){
                       g=0;
                       r=255;
     
  analogWrite(4, r);
  analogWrite(5, g);
  analogWrite(19, b);
  delay(kd);
    
  }                  
  
  delay(2000);
 
}


void handlergbcolor() {
 grad=0;
    if (server.method() == HTTP_POST) {
    String requestPayload = server.arg("plain");
    
    // Parse the JSON request to extract the color value
    const size_t capacity = JSON_OBJECT_SIZE(1) + 20;
    DynamicJsonDocument doc(capacity);
    deserializeJson(doc, requestPayload);
    String hexColor = doc["color"].as<String>();
     String volume = doc["_volume"].as<String>();
   
    // Convert hex color to RGB values
    long colorValue = strtol(&hexColor[1], NULL, 16);
    double colorValume =atof(volume.c_str());;

     red = (colorValue >> 16) & 0xFF;
     green = (colorValue >> 8) & 0xFF;
     blue = colorValue & 0xFF;
       red=red*colorValume;
    green=green*colorValume;
    blue=blue*colorValume;

  if(red==255 && green== 255 && blue==255){
 // digitalWrite(21, HIGH);
  bitWrite(p1,0,1);
   bitWrite(p1,1,1);
 }else{
 // digitalWrite(21, LOW);
    bitWrite(p1,0,0);
    bitWrite(p1,1,0);
    bitWrite(p1,2,1);
    // Do something with the RGB values
    //  control an RGB LED using PWM:
    analogWrite(4, red);
    analogWrite(5, green);
    analogWrite(19, blue);
   
 }
}
}
void handleTv(){
 
  switch(boolTv){
    case 1:
     DirTv=!DirTv;
     bitWrite(p1,4,0);
     bitWrite(p1,5,0);
    activeTv=true;
     boolTv++;
     if(boolTvLoop>>4){
      boolTvLoop=1;
     }
    break;
    case 2:
     activeTv=false;
       bitWrite(p1,4,0);
       bitWrite(p1,5,0);
      boolTv=1;
  }

}
void handleActiveTv(){
   if(shuttersWork==0){
    activeTv=true;
    if(DirTv==1){   
   inputBus();
    bitWrite(p1,4,1);
    delay(50);  
     if(in[35]==1 || in[23]==1){
      bitWrite(p1,4,0);
        activeTv=false;    
     } 
      }
    else{   
      inputBus();
    bitWrite(p1,5,1);
     delay(50);  
    if(in[36]==1 || in[23]==1){
       bitWrite(p1,5,0);
         activeTv=false;     
    }
   
    }
    if(activeTv==false){
       bitWrite(p1,4,0);
       bitWrite(p1,5,0);
        DirTv=!DirTv;
    }
  //   if(in[23]==1){
  //     if(boolTvLoop<<4){
  // boolTvLoop++;
  // delay(500);
  //     }else{
  //       boolTvLoop=1;
  //     }
  // }
  //   switch(boolTvLoop){
  // case 1:
  // activeTv=true;
  // bitWrite(p1,4,0);
  // delay(50);
  // bitWrite(p1,5,1);
  
  // break;
  // case 2:
  // bitWrite(p1,5,0); 
  // bitWrite(p1,4,0);
  // activeTv=false;
 
  // break;
  // case 3:
  //  activeTv=true;
  // bitWrite(p1,5,0);
  // delay(100);
  //  bitWrite(p1,4,1);
   
  //  break;
  //  case 4:
  //   activeTv=false;
  //   bitWrite(p1,5,0);
  //    bitWrite(p1,4,0);
  //    break;
  //    default:
  //    break;
    
  // }
  writeOutput();
 
   }
} 
                ///////////////////////////////
void handletable1(){
  if(shuttersWork==0){
   digitalWrite(ena1, LOW);
    bitWrite(p5,0,0);
    writeOutput();
  dir=!dir;
   digitalWrite(dirPin,dir);
   if(dir==HIGH){
for (int i = 0; i < 8700; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(600);  // تعديل هذه القيمة إذا كنت ترغب في تغيير سرعة الدوران
    digitalWrite(stepPin, LOW);
    delayMicroseconds(600);  // تعديل هذه القيمة إذا كنت ترغب في تغيير سرعة الدوران
    inputBus2();
  endstop1=in[11];
   if(endstop1==HIGH){
        break;
    }
}

}
  else{
    for (int i = 0; i < 8700; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(600);  // تعديل هذه القيمة إذا كنت ترغب في تغيير سرعة الدوران
    digitalWrite(stepPin, LOW);
    delayMicroseconds(600);  // تعديل هذه القيمة إذا كنت ترغب في تغيير سرعة الدوران
     inputBus2();
  endstop_1=in[12];
   if(endstop_1==HIGH){
        break;
    }
}
  }
   digitalWrite(ena1, HIGH);
     bitWrite(p5,0,1);
  }
}


                     ///////////////////////////

void handletable2(){
  if(shuttersWork==0){
   bitWrite(p5,1,0);
    writeOutput();
  dir=!dir;
   digitalWrite(dirPin,dir);
   if(dir==HIGH){
for (int i = 0; i < 6800; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(700);  // تعديل هذه القيمة إذا كنت ترغب في تغيير سرعة الدوران
    digitalWrite(stepPin, LOW);
    delayMicroseconds(700);  // تعديل هذه القيمة إذا كنت ترغب في تغيير سرعة الدوران
     inputBus2();
  endstop1=in[13];
   if(endstop1==HIGH){
        break;
    }
}

}
  else{
    for (int i = 0; i < 6800; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(700);  // تعديل هذه القيمة إذا كنت ترغب في تغيير سرعة الدوران
    digitalWrite(stepPin, LOW);
    delayMicroseconds(700);  // تعديل هذه القيمة إذا كنت ترغب في تغيير سرعة الدوران
     inputBus2();
  endstop_1=in[14];
   if(endstop_1==HIGH){
        break;
    }
}
  }
   bitWrite(p5,1,1);
   //digitalWrite(ena2, HIGH);
  }

}
                              /////////////////////
void handletable3(){
 
 if(shuttersWork==0){
   bitWrite(p5,2,0);
    writeOutput();
  dir=!dir;
   digitalWrite(dirPin,dir);
   if(dir==HIGH){
for (int i = 0; i < 7000; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(700);  // تعديل هذه القيمة إذا كنت ترغب في تغيير سرعة الدوران
    digitalWrite(stepPin, LOW);
    delayMicroseconds(700);  // تعديل هذه القيمة إذا كنت ترغب في تغيير سرعة الدوران
  inputBus3();

     endstop1=in[16];
   if(endstop1==HIGH){
        break;
    }
}

}
  else{
    for (int i = 0; i < 7000; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(700);  // تعديل هذه القيمة إذا كنت ترغب في تغيير سرعة الدوران
    digitalWrite(stepPin, LOW);
    delayMicroseconds(700);  // تعديل هذه القيمة إذا كنت ترغب في تغيير سرعة الدوران
   
    inputBus3();
     endstop_1=in[17];
   if(endstop_1==HIGH){
        break;
    }
}
  }
   bitWrite(p5,2,1);
   //digitalWrite(ena3, HIGH);
 }
}
            //////////////////////////////
void handletable4(){
  if(shuttersWork==0){
   digitalWrite(ena4, LOW);
    bitWrite(p5,3,0);
    writeOutput();
  dir=!dir;
   digitalWrite(dirPin,dir);
   if(dir==HIGH){

for (int i = 0; i < 8750; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(600);  // تعديل هذه القيمة إذا كنت ترغب في تغيير سرعة الدوران
    digitalWrite(stepPin, LOW);
    delayMicroseconds(600);  // تعديل هذه القيمة إذا كنت ترغب في تغيير سرعة الدوران
     inputBus3();
  endstop1=in[18];
   if(endstop1==HIGH){
        break;
    }
}

}
  else{
   

    for (int i = 0; i < 8750; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(600);  // تعديل هذه القيمة إذا كنت ترغب في تغيير سرعة الدوران
    digitalWrite(stepPin, LOW);
    delayMicroseconds(600);  // تعديل هذه القيمة إذا كنت ترغب في تغيير سرعة الدوران
     inputBus3();
  endstop_1=in[19];
   if(endstop_1==HIGH){
        break;
    }
}

  }
   digitalWrite(ena4, HIGH);
     bitWrite(p5,3,1);
  }
}
                         /////////////////////
void handlespresso(){ 
  if(shuttersWork==0){
   digitalWrite(ena5, LOW);
    bitWrite(p5,4,0);
    writeOutput();
  dir=!dir;
   digitalWrite(dirPin,dir);
   if(dir==HIGH){
for (int i = 0; i <11000; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(600);  // تعديل هذه القيمة إذا كنت ترغب في تغيير سرعة الدوران
    digitalWrite(stepPin, LOW);
    delayMicroseconds(600);  // تعديل هذه القيمة إذا كنت ترغب في تغيير سرعة الدوران
     inputBus3();
  endstop1=in[20];
   if(endstop1==HIGH){
        break;
    }
}

}
  else{
    for (int i = 0; i < 11000; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(600);  // تعديل هذه القيمة إذا كنت ترغب في تغيير سرعة الدوران
    digitalWrite(stepPin, LOW);
    delayMicroseconds(600);  // تعديل هذه القيمة إذا كنت ترغب في تغيير سرعة الدوران
     inputBus3();
  endstop_1=in[21];
   if(endstop_1==HIGH){
        break;
    }
}
  }
   digitalWrite(ena5, HIGH);
     bitWrite(p5,4,1);
  }
}



////////////////////////////////////////

void handleshr1(){
  shuttersWork=1;
   stopUpTable1=in[1];
  if (stopUpTable1==0){
   int pulse = map(100, -180, 180, servoMin, servoMax);
  pwm.setPWM(servoPin1, 0, pulse);
  tUp1=1;
  }
}
void handleshs1(){
  
  pwm.setPWM(servoPin1, 150, 150);
  shuttersWork=0;
  
}
void handleshl1(){
   shuttersWork=1;
  stopDownTable1=in[0];
  
  if (stopDownTable1==0){
   int pulse = map(-110, -180, 180, servoMin, servoMax);
  pwm.setPWM(servoPin1, 0, pulse);
 tDown1=1;
  }
}

void handleshr2(){
   shuttersWork=1;
   stopUpTable2=in[3];
  if (stopUpTable2==0){
   int pulse = map(100, -180, 180, servoMin, servoMax);
  pwm.setPWM(servoPin2, 0, pulse);
  tUp2=1;
  }
}
void handleshs2(){
  pwm.setPWM(servoPin2, 150, 150);
   shuttersWork=0;
  
}
void handleshl2(){
   shuttersWork=1;
  stopDownTable2=in[2];
  if (stopDownTable2==0){
 
   int pulse = map(-110, -180, 180, servoMin, servoMax);
  pwm.setPWM(servoPin2, 0, pulse);
 
 tDown2=1;
}}

void handleshr3(){
   shuttersWork=1;
  stopUpTable3=in[5];
   if (stopUpTable3==0){
   int pulse = map(105, -180, 180, servoMin, servoMax);
  pwm.setPWM(servoPin3, 0, pulse);
  tUp3=1;
}}
void handleshs3(){
  pwm.setPWM(servoPin3, 150, 150);
   shuttersWork=0;
}
void handleshl3(){
   shuttersWork=1;
  stopDownTable3=in[4];
  if (stopDownTable3==0){
   
   int pulse = map(-110, -180, 180, servoMin, servoMax);
  pwm.setPWM(servoPin3, 0, pulse);
  tDown3=1;
}}

void handleshr4(){
   shuttersWork=1;
 stopUpTable4=in[8];
  if (stopUpTable4==0){
   int pulse = map(100, -180, 180, servoMin, servoMax);
  pwm.setPWM(servoPin4, 0, pulse);
 tUp4=1;
}}
void handleshs4(){
  pwm.setPWM(servoPin4, 150, 150);
     shuttersWork=0; 
}
void handleshl4(){
   shuttersWork=1;
   stopDownTable4=in[6];
  if (stopDownTable4==0){
  
   int pulse = map(-110, -180, 180, servoMin, servoMax);
  pwm.setPWM(servoPin4, 0, pulse);
 
  tDown4=1;
}
}
void handleshr5(){
   shuttersWork=1;
  stopUpTable5=in[10];
  if (stopUpTable5==0){
   int pulse = map(100, -180, 180, servoMin, servoMax);
  pwm.setPWM(servoPin5, 0, pulse);

 tUp5=1;
}}
void handleshs5(){
  pwm.setPWM(servoPin5, 150, 150);
   shuttersWork=0;
   
}
void handleshl5(){
   shuttersWork=1;
  stopDownTable5=in[9];
  if (stopDownTable5==0){
  
   int pulse = map(-110, -180, 180, servoMin, servoMax);
  pwm.setPWM(servoPin5, 0, pulse);
 
  tDown5=1;
}}
void esptrue(){
 esp=true;
}
void espfalse(){
 esp=false;
}
/////////////////////
// مصفوفة أسماء الدوال
const char* functionNames[] = {"espfalse","esptrue","handletable1", "handletable2", "handletable3","handlelighton","handlelightoff","handleTv","handlespresso","handledeepLight"};

// مصفوفة المؤشرات إلى الدوال
void (*functions[])() = {esptrue,espfalse, handletable1, handletable2, handletable3,handlelighton,handlelightoff,handleTv,handlespresso,handledeepLight};

const int numFunctions = sizeof(functionNames) / sizeof(functionNames[0]);
//////////////////////////////////////firebase////////////////////////////
 /////////////////
void setup(){
    WiFi.begin(ssid, password);
      if(WiFi.status() !=WL_CONNECTED){
     Serial.print(".");
    delay(3000);  
    }
   
  pinMode(ser,OUTPUT);
  pinMode(latch,OUTPUT);
  pinMode(clk,OUTPUT);
  pinMode(shld,OUTPUT);
 pinMode(clk2,OUTPUT);
  pinMode(clkin,OUTPUT);
  pinMode(qh,INPUT);

 pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
   pinMode(ena1, OUTPUT);
  digitalWrite(ena1, HIGH);
   digitalWrite(dirPin, HIGH);
  // pinMode(ena2, OUTPUT);
 // digitalWrite(ena2, HIGH);
  // pinMode(ena3, OUTPUT);
 // digitalWrite(ena3, HIGH);
   pinMode(ena4, OUTPUT);
  digitalWrite(ena4, HIGH);
    pinMode(ena5, OUTPUT);
  digitalWrite(ena5, HIGH);
//  pinMode(32, OUTPUT);
//   digitalWrite(32, LOW);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(19,OUTPUT);
 // pinMode(33, INPUT);


  pinMode(25,OUTPUT);
  pinMode(26,OUTPUT);
  
   

  digitalWrite(4,LOW);
  digitalWrite(5,LOW);

   digitalWrite(19,LOW);
   
   
  

   digitalWrite(25,LOW);
   digitalWrite(26,LOW);
    bitWrite(p5,0,1);
     bitWrite(p5,1,1);
      bitWrite(p5,2,1);
     bitWrite(p5,3,1);
      bitWrite(p5,4,1);
  //  pinMode(GPIO_NUM_34, INPUT_PULLUP);
  // attachInterrupt(GPIO_NUM_34, handleRisingInterrupt, RISING);
  //   pinMode(GPIO_NUM_35, INPUT_PULLUP);
  // attachInterrupt(GPIO_NUM_35, handleFallingInterrupt, FALLING);

    Wire.begin(SDA_PIN,SCL_PIN);
     myStepper1.setSpeed(300);
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);
  Serial.begin(115200);
 
 //int  timeMillis=millis();
     //while(timeMillis-millis()<5000){
   
     
  
  
    
/////////////////////////////////////


    server.on("/shr1",handleshr1);
    server.on("/shs1",handleshs1);
    server.on("/shl1",handleshl1);

    server.on("/shr2",handleshr2);
    server.on("/shs2",handleshs2);
    server.on("/shl2",handleshl2);

    server.on("/shr3",handleshr3);
    server.on("/shs3",handleshs3);
    server.on("/shl3",handleshl3);

    server.on("/shr4",handleshr4);
    server.on("/shs4",handleshs4);
    server.on("/shl4",handleshl4);

    server.on("/shr5",handleshr5);
    server.on("/shs5",handleshs5);
    server.on("/shl5",handleshl5);

   
  
   server.on("/handlergbcolor", handlergbcolor);
   server.on("/handlelighton", handlelighton);
   server.on("/handlelightoff", handlelightoff);
   server.on("/handlegrad", handlegrad);
   server.on("/table2", handletable2); 
   server.on("/spresso", handlespresso); 
   server.on("/table4",handletable4);
   server.on("/table1",handletable1);
   server.on("/table3",handletable3);
   server.on("/deepLight",handledeepLight);
   server.on("/handleTv",handleTv);

server.begin();

////////////////////////////////////
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;

   Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  
  
  //  if(WiFi.status() ==WL_CONNECTED){
  //   IPaddress =  WiFi.localIP().toString();
  //  if (Firebase.ready()) {
  //  if (Firebase.RTDB.setString(&fbdo,  "/ESP/IPAddress", IPaddress)) {
  //  }
  
  //  }
  // }
 int   firebaseMillis=millis();
//////////////////////////////////////////////
// // تهيئة EEPROM
//   if (!EEPROM.begin(EEPROM_SIZE)) {
//     Serial.println("Failed to initialize EEPROM");
//     while (1);
//   }
//    // كتابة قيمة منطقية
//   writeBooleanToEEPROM(0, false);
 IPaddress =  WiFi.localIP().toString();
     Serial.println(IPaddress);
}
//////////////////////////////////////////////
void loop(){
 
 if(esp==true){
 server.handleClient();
 //if(activeTv==true){
 // handleActiveTv();
 //}
   writeOutput();
   delay(5);
   inputBus();
   swichFun();  
    
   
   
bool stopDownTable1=in[0];
bool stopUpTable1=in[1];

bool stopDownTable2=in[2];
bool stopUpTable2=in[3];

bool stopDownTable3=in[4];
bool stopUpTable3=in[5];

bool stopDownTable4=in[6];
bool stopUpTable4=in[8];

bool stopDownTable5=in[9];
bool stopUpTable5=in[10];

   if (tUp1==1){
    if (stopUpTable1==1){
       handleshr1();
     delay(300);
      handleshs1();
       tUp1=0;
       tDown1=0;
    }
   }
   if (tDown1==1){
    if (stopDownTable1==1){
      handleshs1();
      tDown1=0;
    }
   }
   /////
   if (tUp2==1){
    if (stopUpTable2==1){
      handleshl2();
     delay(20);
      handleshs2();
      tUp2=0;
      tDown2=0;
    }
   }
   if (tDown2==1){
    if (stopDownTable2==1){
      handleshs2();
      tDown2=0;
    }
   }
   ////
   if (tUp3==1){
    if (stopUpTable3==1){
      handleshl3();
     delay(20);
      handleshs3();
      tUp3=0;
       tDown3=0;
    }
   }
   if (tDown3==1){
    if (stopDownTable3==1){
       handleshr3();
     delay(20);
      handleshs3();
     tUp3=0;
       tDown3=0;
    }
   }
    /////
    
   if (tUp4==1){
    if (stopUpTable4==1){
    
      handleshs4();
      tUp4=0;
    }
   }
   if (tDown4==1){
    if (stopDownTable4==1){
         handleshr4();
     delay(300);
      handleshs4();
      tDown4=0;
       tUp4=0;
    }
   }
   /////
   
   if (tUp5==1){
    if (stopUpTable5==1){
      handleshs5();
      tUp5=0;
    }
   }
   if (tDown5==1){
    if (stopDownTable5==1){
      handleshs5();
      tDown5=0;
    }
   }
 
    if(grad==1){
   handlegrad();
   }
   
 }
// if(firebaseMillis-millis()>=1000){
  //  if(WiFi.status() ==WL_CONNECTED){
   //if (Firebase.ready()) {
   //firebaseData();
   //}
   //}
    //firebaseMillis=millis();
    //}
     delay(200);

}

void firebaseData(){
 

  if (Firebase.RTDB.getString(&fbdo, "/functionName")) {
    String functionName = fbdo.to<String>();
   // Serial.println("Function name from Firebase: " + functionName);
   if(functionName!="null"){
   
    for (int i = 0; i < numFunctions; i++) {
      if (functionName == functionNames[i]) {
       
        functions[i]();
        if (functionName!="esptrue"){
         if (Firebase.RTDB.setString(&fbdo,  "/functionName", "null")) {
   }}
        return;
      }
    }  
   }   
   // Serial.println("Function not found!");
  } else {
   // Serial.println("Failed to get function name from Firebase: " + fbdo.errorReason());
  }
 
}
// void espTrue(){
//   writeBooleanToEEPROM(0,true)
// }
// void espFalse(){
//   writeBooleanToEEPROM(0,false)
// }
// void writeBooleanToEEPROM(int address, bool value) {
//   EEPROM.write(address,value);
//   EEPROM.commit(); // تأكيد الكتابة
//   Serial.println("Boolean value written to EEPROM");
   
// }

////////////////////////////////////////     END    ////////////////////////////////
