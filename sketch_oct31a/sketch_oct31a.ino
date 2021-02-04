/*************************************************************
  ESP8266 with 0.96inch OLED 引脚
      VCC    <--->    VCC
      GND    <--->    GND
      SDA    <--->    SDA(2)  
      SCL    <--->    SCL(14)         
 *************************************************************/
#if defined(ESP32) //ESP32
#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#elif defined(ESP8266) //ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#else
#error "Please check your mode setting,it must be esp8266 or esp32."
#endif

#include <ArduinoJson.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <Ticker.h>

// 定时器
Ticker timer;
int count = 0;
boolean flag = true;

// JSON
DynamicJsonBuffer jsonBuffer(256); // ArduinoJson V5

// 显示屏  如果引脚不同需要在这里修改
// U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 11, /* data=*/ 13);  
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display

// WiFi 名称与密码
const char *ssid = "Redmi K30 Ultra"; //这里填你家中的wifi名
const char *password = "13307023186";//这里填你家中的wifi密码

//24*24小电视的点阵图
const unsigned char bilibilitv_24u[] U8X8_PROGMEM = {0x00, 0x00, 0x02, 0x00, 0x00, 0x03, 0x30, 0x00, 0x01, 0xe0, 0x80, 0x01,
                                                     0x80, 0xc3, 0x00, 0x00, 0xef, 0x00, 0xff, 0xff, 0xff, 0x03, 0x00, 0xc0, 0xf9, 0xff, 0xdf, 0x09, 0x00, 0xd0, 0x09, 0x00, 0xd0, 0x89, 0xc1,
                                                     0xd1, 0xe9, 0x81, 0xd3, 0x69, 0x00, 0xd6, 0x09, 0x91, 0xd0, 0x09, 0xdb, 0xd0, 0x09, 0x7e, 0xd0, 0x0d, 0x00, 0xd0, 0x4d, 0x89, 0xdb, 0xfb,
                                                     0xff, 0xdf, 0x03, 0x00, 0xc0, 0xff, 0xff, 0xff, 0x78, 0x00, 0x1e, 0x30, 0x00, 0x0c
                                                    };

// B 站 API 网址: follower, view, likes
String NAME = "伟大的罗翔老师";  //改成自己的名字
String UID  = "517327498";  //改成自己的UID
String followerUrl = "http://api.bilibili.com/x/relation/stat?vmid=" + UID;   // 粉丝数
String viewAndLikesUrl = "http://api.bilibili.com/x/space/upstat?mid=" + UID; // 播放数、点赞数

long follower = 0;   // 粉丝数
long view = 0;       // 播放数
long likes = 0;      // 获赞数

void setup()
{
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

  timer.attach(60, timerCallback); // 每隔1min

  // 第一次调用获取数据函数，方便开机即可显示
  getFollower(followerUrl);
  getViewAndLikes(viewAndLikesUrl);
}

void loop()
{
  while (flag)
  {
    if (count == 0)
    {
      // display data
      Serial.println("count = 0, display data");
      u8g2.firstPage();
      do
      {
        display(follower, likes, view);
      } while (u8g2.nextPage());
      flag = false;
    } else if (count == 1) {
      // get follower
      Serial.println("count = 1, get follower");
      getFollower(followerUrl);
      flag = false;
    } else if (count == 2) {
      // get view and likes
      Serial.println("count = 2, get view and likes");
      getViewAndLikes(viewAndLikesUrl);
      flag = false;
    }
  }
}
// 定时器回调函数
void timerCallback()
{
  count++;
  if (count == 3)
  {
    count = 0;
  }
  flag = true;
}
// 获取 B 站粉丝数
void getFollower(String url)
{
  HTTPClient http;
  http.begin(url);

  int httpCode = http.GET();
  Serial.printf("[HTTP] GET... code: %d\n", httpCode);

  if (httpCode == 200)
  {
    Serial.println("Get OK");
    String resBuff = http.getString();
    // ---------- ArduinoJson V5 ----------
    JsonObject &root = jsonBuffer.parseObject(resBuff);
    if (!root.success())
    {
      Serial.println("parseObject() failed");
      return;
    }
    follower = root["data"]["follower"];
    Serial.print("Fans: ");
    Serial.println(follower);
  }
  else
  {
    Serial.printf("[HTTP] GET... failed, error: %d\n", httpCode);
  }

  http.end();
}

// 获取 B 站播放数与获赞数
void getViewAndLikes(String url)
{
  HTTPClient http;
  http.begin(url);

  int httpCode = http.GET();
  Serial.printf("[HTTP] GET... code: %d\n", httpCode);

  if (httpCode == 200)
  {
    Serial.println("Get OK");
    String resBuff = http.getString();

    // ---------- ArduinoJson V5 ----------
    JsonObject &root = jsonBuffer.parseObject(resBuff);
    if (!root.success())
    {
      Serial.println("parseObject() failed");
      return;
    }
    likes = root["data"]["likes"];
    view = root["data"]["archive"]["view"];
    Serial.print("Likes: ");
    Serial.println(likes);
    Serial.print("View: ");
    Serial.println(view);
  }
  else
  {
    Serial.printf("[HTTP] GET... failed, error: %d\n", httpCode);
  }

  http.end();
}

// OLED 显示数据
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
  u8g2.print("bilibili@" + String(NAME)); //改成自己的名字
}
