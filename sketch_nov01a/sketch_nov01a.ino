#if defined(ESP32) //ESP32
#include <WiFi.h>


#elif defined(ESP8266) //ESP8266
#include <ESP8266WiFi.h>


#else
#error "Please check your mode setting,it must be esp8266 or esp32."
#endif

#include <ArduinoJson.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <Ticker.h>


// JSON
DynamicJsonBuffer jsonBuffer(256); // ArduinoJson V5

// 显示屏  如果引脚不同需要在这里修改
// U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 11, /* data=*/ 13);  
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display

// WiFi 名称与密码
const char *ssid = "Redmi K30 Ultra"; //这里填你家中的wifi名
const char *password = "13307023186";//这里填你家中的wifi密码

const unsigned char bilibilitv_24u[] U8X8_PROGMEM = {0x00, 0x00, 0x02, 0x00, 0x00, 0x03, 0x30, 0x00, 0x01, 0xe0, 0x80, 0x01,
                                                     0x80, 0xc3, 0x00, 0x00, 0xef, 0x00, 0xff, 0xff, 0xff, 0x03, 0x00, 0xc0, 0xf9, 0xff, 0xdf, 0x09, 0x00, 0xd0, 0x09, 0x00, 0xd0, 0x89, 0xc1,
                                                     0xd1, 0xe9, 0x81, 0xd3, 0x69, 0x00, 0xd6, 0x09, 0x91, 0xd0, 0x09, 0xdb, 0xd0, 0x09, 0x7e, 0xd0, 0x0d, 0x00, 0xd0, 0x4d, 0x89, 0xdb, 0xfb,
                                                     0xff, 0xdf, 0x03, 0x00, 0xc0, 0xff, 0xff, 0xff, 0x78, 0x00, 0x1e, 0x30, 0x00, 0x0c
                                                    };
                                                    
void setup() {
  // OLED 初始化
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
  delay(5000);
    
  u8g2.setFont(u8g2_font_wqy12_t_gb2312b);
  u8g2.setFontPosTop();
  u8g2.clearDisplay();

  Serial.begin(115200);

  // WiFi 连接
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");



  // 第一次调用获取数据函数，方便开机即可显示
  

}

void loop() {
  long a=10,b=100,c=1000;
  u8g2.firstPage();
      do
      {
        display(a, b, c);
      } while (u8g2.nextPage());// put your main code here, to run repeatedly:

}

void display(long follower, long likes, long view)
{
  u8g2.clearDisplay();
  u8g2.setCursor(5, 25);
  u8g2.print("粉丝数：" + String(follower));
  u8g2.setCursor(5, 39);
  u8g2.print("获赞数：" + String(likes));
  u8g2.setCursor(5, 52);
  u8g2.print("播放数：" + String(view));
  u8g2.setCursor(5, 7);
  u8g2.print("bilibili@"); //改成自己的名字
}
