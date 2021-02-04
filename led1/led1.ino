#include <ESP8266WiFi.h>   //安装esp8266arduino开发环境
void setup()
{

  
  
      //pinMode(D0, OUTPUT);
      //pinMode(D1, OUTPUT);
      //pinMode(D2, OUTPUT);
      //pinMode(D3, OUTPUT);
      
      pinMode(D4, OUTPUT);
      //pinMode(D5, OUTPUT);
      
      

      //digitalWrite(D0,LOW);
      //digitalWrite(D1,LOW);
      //digitalWrite(D2,LOW);
      //digitalWrite(D3,LOW);
      //digitalWrite(D4,LOW);
      
      //digitalWrite(D5,LOW);
      //digitalWrite(D6,LOW);
      
      //digitalWrite(D7,LOW);
      //digitalWrite(D8,LOW);
     
    Serial.begin(9600);

    Serial.println("Demo Start");

    
}



void loop()
{
   
   digitalWrite(D4,LOW);
   delay(1000);
   digitalWrite(D4,HIGH);
   delay(1000);
}
