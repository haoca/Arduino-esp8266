/*
digitalWrite()示例程序
v1.0
Created 204
by 太极创客
www.taichi-maker.com
 
This example code is in the public domain.
*/
 
int ledPin = 4;                 
int ledPin2 = 16;  
// 当Arduino控制器通电或复位后，setup函数会运行一次
void setup()
{
  // 将引脚4设置为输出（OUTPUT）模式
  pinMode(ledPin, OUTPUT);  
  pinMode(ledPin2, OUTPUT);    
}
 
// 当Arduino控制器通电或复位后，loop函数会反复运行
void loop()
{
  int val;
  val=digitalRead(ledPin);   // 引脚4高电平，点亮LED
  delay(1000);                  // 等待1秒
  digitalWrite(ledPin,HIGH);
  digitalWrite(ledPin2,HIGH);
  Serial.println(val); 
  digitalRead(ledPin);    // 引脚4底电平，关闭LED
  delay(1000);                  // 等待1秒
  digitalWrite(ledPin,LOW);
  digitalWrite(ledPin2,LOW);
}
