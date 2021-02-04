/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink
*/
int i=1;
void setup() {
  Serial.begin(9600);
  // initialize digital pin LED_BUILTIN as an output.
  /*pinMode(LED_BUILTIN, OUTPUT);
  pinMode(13, OUTPUT);   //WIFI模块指示灯
  pinMode(D0, OUTPUT);    //用于连接EDP控制的发光二极管
  
  pinMode(1, 1);*/
  pinMode(D0, OUTPUT);
  //pinMode(13, OUTPUT);
  
 
  
}

// the loop function runs over and over again forever
void loop() {
  Serial.begin(9600);
  
   
   //digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(D0, HIGH);
  //digitalWrite(13, HIGH);
  //Serial.println("LED1：");Serial.println(digitalRead(1));
  //Serial.println("LED2：");Serial.println(digitalRead(13));
  delay(1000);                       // wait for a second
  //digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(D0, LOW);
  //digitalWrite(13, HIGH);
  //Serial.println("LED3：");Serial.println(digitalRead(1));
  //Serial.println("LED4：");Serial.println(digitalRead(13));
  
  
 
  
  
  delay(1000);      
  
  Serial.println("LED：");Serial.println(i);Serial.println("：");Serial.println(digitalRead(i));
  i=i+1;
  if(i==20)
  i=1;
  
  
  

    
}
