#define BLINKER_WIFI

#include <Blinker.h>
#include <SimpleDHT.h>
int pinDHT11 = D3;
SimpleDHT11 dht11(pinDHT11);

#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display
const unsigned char bilibilitv_24u[] U8X8_PROGMEM = {0x00, 0x00, 0x02, 0x00, 0x00, 0x03, 0x30, 0x00, 0x01, 0xe0, 0x80, 0x01,
                                                     0x80, 0xc3, 0x00, 0x00, 0xef, 0x00, 0xff, 0xff, 0xff, 0x03, 0x00, 0xc0, 0xf9, 0xff, 0xdf, 0x09, 0x00, 0xd0, 0x09, 0x00, 0xd0, 0x89, 0xc1,
                                                     0xd1, 0xe9, 0x81, 0xd3, 0x69, 0x00, 0xd6, 0x09, 0x91, 0xd0, 0x09, 0xdb, 0xd0, 0x09, 0x7e, 0xd0, 0x0d, 0x00, 0xd0, 0x4d, 0x89, 0xdb, 0xfb,
                                                     0xff, 0xdf, 0x03, 0x00, 0xc0, 0xff, 0xff, 0xff, 0x78, 0x00, 0x1e, 0x30, 0x00, 0x0c
                                                    };
                                                    
char auth[] = "42287f3f745a";
char ssid[] = "TP666666";
char pswd[] = "qwe123456789";
int humi_read = 0, temp_read = 0;
// 新建组件对象
BlinkerButton Button0("btn-abc");
BlinkerButton Button1("btn-up");
BlinkerButton Button2("btn-down");
BlinkerButton Button3("btn-left");
BlinkerButton Button4("btn-right");

BlinkerNumber HUMI("humi");
BlinkerNumber TEMP("temp");

// 按下按键即会执行该函数
void button0_callback(const String & state) {
    BLINKER_LOG("get button state: ", state);
    digitalWrite(D4, !digitalRead(D4));
    Blinker.vibrate();
}
void button1_callback(const String & state) {
    BLINKER_LOG("get button state: ", state);
    digitalWrite(D4, !digitalRead(D4));
    Blinker.vibrate();
}
void button2_callback(const String & state) {
    BLINKER_LOG("get button state: ", state);
    digitalWrite(D4, !digitalRead(D4));
    Blinker.vibrate();
}
void button3_callback(const String & state) {
    BLINKER_LOG("get button state: ", state);
    digitalWrite(D4, !digitalRead(D4));
    Blinker.vibrate();
}
void button4_callback(const String & state) {
    BLINKER_LOG("get button state: ", state);
    digitalWrite(D4, !digitalRead(D4));
    Blinker.vibrate();
}
byte temperature = 0;
byte humidity = 0;
void heartbeat()
{
    HUMI.print(humi_read);
    TEMP.print(temp_read);
}
void setup() {
    // 初始化串口，并开启调试信息
    Serial.begin(115200);    
    BLINKER_DEBUG.stream(Serial);
    // 初始化有LED的IO
    pinMode(D4, OUTPUT);
    digitalWrite(D4, LOW);
    // 初始化blinker
    Blinker.begin(auth, ssid, pswd);
    Button0.attach(button0_callback);
    Button1.attach(button1_callback);
    Button2.attach(button2_callback);
    Button3.attach(button3_callback);
    Button4.attach(button4_callback);
    Blinker.attachHeartbeat(heartbeat);
    //u8g2gegin();
}

void loop() {
    
    /*u8g2.firstPage();
   do
      {
        
    


        
       Blinker.run();
       temp_humidity();
        
        //Serial.println(str1);
        //mqttClient.loop();
        //temp_humidity();

  
      } while (u8g2.nextPage());// put your main code here, to run repeatedly:*/
      Blinker.run();
      temp_humidity();
}

void temp_humidity(){
  
  int err = SimpleDHTErrSuccess;
  
  
  err = dht11.read(&temperature, &humidity, NULL);
  int t =(int)temperature;
  int h = (int)humidity;
  BLINKER_LOG("Humidity: ", h, " %");
        BLINKER_LOG("Temperature: ", t, " *C");
        humi_read = h;
        temp_read = t;
        display(t,h);
  Blinker.delay(2000);
  
  
}

void u8g2gegin(){
  u8g2.begin();
  u8g2.enableUTF8Print();
  u8g2.clearDisplay();
  u8g2.setFont(u8g2_font_wqy12_t_gb2312a);
  u8g2.drawXBMP( 16 , 9 , 24 , 24 , bilibilitv_24u );
  u8g2.setCursor(45, 19);
  u8g2.print("Powered by");
  u8g2.setCursor(45, 31);
  u8g2.print("@haostart");
  u8g2.setFont(u8g2_font_wqy12_t_gb2312a);
  u8g2.setCursor(10, 50);
  u8g2.print("");
  u8g2.sendBuffer();
  Blinker.delay(4000);
    
  u8g2.setFont(u8g2_font_wqy12_t_gb2312b);
  u8g2.setFontPosTop();
  u8g2.clearDisplay();
}
void display(int t,int h)
{
  //u8g2.clearDisplay();
  u8g2.setCursor(5, 22);
  //u8g2.print("粉丝数："+ String(str)+ String(follower));
  u8g2.print("收到的信息：");
  u8g2.setCursor(5, 37);
  u8g2.print("温度："+String(t));
  //delay(500);
  u8g2.setCursor(5, 52);
  u8g2.print("湿度："+String(h));
  //u8g2.print("获赞数：" + String(likes));
  //u8g2.print(String(str1));
  //u8g2.print("获赞数：" + String(likes));
  
  u8g2.setCursor(5, 7);
  u8g2.print("bilibili@haost"); //改成自己的名字
}
