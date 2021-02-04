#define ENB    0          // Enable/speed motors Left           GPIO 0(D3)
//#define ENB_2  2          // Enable/speed motors Left_2         GPIO 2(D4)
#define IN_1  14          // L298N in1 motors Right             GPIO14(D5)
#define IN_2  12          // L298N in2 motors Right             GPIO12(D6)
#define IN_3  13          // L298N in3 motors Left              GPIO13(D7)
#define IN_4  15          // L298N in4 motors Left              GPIO15(D8)
int speedCar = 200;         // 0 - 1023.
int speedInterval = 90;     //速度间隔
int speedTrun = 1.5;        //转弯系数


void setup() {
  pinMode(ENB, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  //pinMode(ENB, OUTPUT);
  //digitalWrite(ENB, 1);  
  Serial.begin(115200);
  
  
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
analogWrite(ENB, speedCar);
  //analogWrite(D0, speedCar);
//analogWrite(D1, speedCar);
//analogWrite(D2, speedCar);
analogWrite(D5, speedCar);

analogWrite(D6, speedCar);

Serial.println(speedCar);
Serial.println("  ppppp  \n");
 Serial.println("ENB+");Serial.println(analogRead(ENB));Serial.println("  ppppp  \n");
 //Serial.println("D0+"+analogRead(D0));Serial.println("  ppppp  \n");
 //Serial.println("D1+"+analogRead(D1));Serial.println("  ppppp  \n");
 //Serial.println("D2+"+analogRead(D2));Serial.println("  ppppp  \n");
 Serial.println("D5+");Serial.println(analogRead(D5));Serial.println("  ppppp  \n");
 Serial.println("D6+");Serial.println(analogRead(D6));Serial.println("  ppppp  \n");
 delay(2500);
}
