#include <ESP8266WiFi.h>//安装esp8266arduino开发环境
static WiFiClient espClient;
#include "aliyun_mqtt.h"

#include <ArduinoJson.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <Ticker.h>

//#include <AliyunIoTSDK.h>//引入阿里云 IoT SDK
//需要安装crypto库、PubSubClient库


//设置产品和设备的信息，从阿里云设备信息里查看
#define PRODUCT_KEY     "a1N8FKV28Xa"//替换自己的PRODUCT_KEY
#define DEVICE_NAME     "text2"//替换自己的DEVICE_NAME
#define DEVICE_SECRET   "d63a68c4a626d69fb40855935e4d1f4f"//替换自己的DEVICE_SECRET
#define REGION_ID       "cn-shanghai"//默认cn-shanghai

#define WIFI_SSID       "千户"//替换自己的WIFI
#define WIFI_PASSWD     "13307023186"//替换自己的WIFI

unsigned long lastMsMain = 0;

#define DEV_VERSION       "S-TH-WIFI-v1.0-20190220"        //固件版本信息
#define ALINK_BODY_FORMAT         "{\"id\":\"123\",\"version\":\"1.0\",\"method\":\"%s\",\"params\":%s}"

//*************  
#define ALINK_TOPIC_PROP_POSTRSP  "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/event/property/post_reply"
                          
#define ALINK_METHOD_PROP_POST    "thing.service.property.set"                        
#define ALINK_TOPIC_DEV_INFO      "/ota/device/inform/" PRODUCT_KEY "/" DEVICE_NAME ""    
#define ALINK_VERSION_FROMA      "{\"id\": 123,\"params\": {\"version\": \"%s\"}}"
//-----------------------------接收消息的TOPIC -------------------------------------------
#define ALINK_TOPIC_PROP_SET      "/"PRODUCT_KEY "/" DEVICE_NAME"/user/3" //这个就是我们接收消息的TOPIC        
//------------------------------接收消息的TOPIC ------------------------------------------ 

//***************************这个是上传数据的TOPIC******************************************
#define ALINK_TOPIC_PROP_POST       "/"PRODUCT_KEY "/" DEVICE_NAME"/user/3" //这个是上传数据的TOPIC
//****************************这个是上传数据的TOPIC****************************************   

unsigned long lastMs = 0;
int pos = 90;


PubSubClient mqttClient(espClient);

// 显示屏  如果引脚不同需要在这里修改
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display



const unsigned char bilibilitv_24u[] U8X8_PROGMEM = {0x00, 0x00, 0x02, 0x00, 0x00, 0x03, 0x30, 0x00, 0x01, 0xe0, 0x80, 0x01,
                                                     0x80, 0xc3, 0x00, 0x00, 0xef, 0x00, 0xff, 0xff, 0xff, 0x03, 0x00, 0xc0, 0xf9, 0xff, 0xdf, 0x09, 0x00, 0xd0, 0x09, 0x00, 0xd0, 0x89, 0xc1,
                                                     0xd1, 0xe9, 0x81, 0xd3, 0x69, 0x00, 0xd6, 0x09, 0x91, 0xd0, 0x09, 0xdb, 0xd0, 0x09, 0x7e, 0xd0, 0x0d, 0x00, 0xd0, 0x4d, 0x89, 0xdb, 0xfb,
                                                     0xff, 0xdf, 0x03, 0x00, 0xc0, 0xff, 0xff, 0xff, 0x78, 0x00, 0x1e, 0x30, 0x00, 0x0c
                                                    };
int DISA=3,DISB=3,DISC=3;
char str1[100] = "a",str2[100] = "b",str3[100] = "c";    
struct json1
{
    char a[100];
    char b[100];
    char c[100];
} json1 = {"a", "b", "c"};   
struct json1 json2;   
void init_wifi(const char *ssid, const char *password)      //连接WiFi
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("WiFi does not connect, try again ...");
        delay(500);
    }

    Serial.println("Wifi is connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

#define WIFI_SSID        "千户"//替换自己的WIFI
#define WIFI_PASSWD      "13307023186"//替换自己的WIFI

void setup()
{
  Serial.begin(9600);
  init_wifi(WIFI_SSID, WIFI_PASSWD);
  //wifiInit(WIFI_SSID, WIFI_PASSWD);
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
  
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  //连接到wifi
  

  //初始化 iot，需传入 wifi 的 client，和设备产品信息
  //AliyunIoTSDK::begin(espClient, PRODUCT_KEY, DEVICE_NAME, DEVICE_SECRET, REGION_ID);
  Serial.println("MQTT connect succeed!");
  //mqttClient.subscribe(ALINK_TOPIC_PROP_SET);          //这个就是引用开始定义的topic
  Serial.println("subscribe done");
  //绑定一个设备属性回调，当远程修改此属性，会触发LED函数
  mqttClient.setCallback(mqtt_callback);
  //AliyunIoTSDK::bindData("DIS", DIS);
  //AliyunIoTSDK::bindData("text2", text2);
  //AliyunIoTSDK::bindData("text2", text2);
  //AliyunIoTSDK::bindData("text3", text3);
  //AliyunIoTSDK::bindData("json", json);

  
}

void loop()
{
  
  int a=10,b=100,c=1000;
  u8g2.firstPage();
  //mqttClient.setCallback(mqtt_callback);
      do
      {
        if (millis() - lastMs >= 5000)  
    {
        lastMs = millis();
        mqtt_check_connect();
    }
    //mqttClient.loop();


        
        a=DISA;
        display(a, b, c);
        mqttClient.loop();
        

  /*if (millis() - lastMsMain >= 2000)//每2秒发送一次
  {
    lastMsMain = millis();
    mqtt_check_connect();

    //发送LED状态到云平台（高电平：1；低电平：0）
    //AliyunIoTSDK::send("LEDPIN状态", digitalRead(LED_BUILTIN));
    //AliyunIoTSDK::send("str1状态",str1);
    //AliyunIoTSDK::send(str1);
    //AliyunIoTSDK::send("str2状态", str2);
  }
  mqttClient.loop();*/
  //AliyunIoTSDK::loop();//必要函数
      } while (u8g2.nextPage());// put your main code here, to run repeatedly:
}

//wifi 连接
void wifiInit(const char *ssid, const char *passphrase)
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, passphrase);
  while (WiFi.status() != WL_CONNECTED)
  {
    
    delay(2000);
    Serial.println("WiFi not Connect");
    
  }
  Serial.println("Connected to AP......");
  Serial.println("IP Adress:");
  Serial.println(WiFi.localIP());
}

void IO(char a){
  switch(a){
  case'0':Serial.println("成功！！");break;
  case'1':Serial.println("123445534！！");break;
  case'2':if(digitalRead(D2)){digitalWrite(D2,LOW);}else{digitalWrite(D2,HIGH);}break;
  case'3':if(digitalRead(D3)){digitalWrite(D3,LOW);}else{digitalWrite(D3,HIGH);}break;
  case'4':if(digitalRead(D4)){digitalWrite(D4,LOW);}else{digitalWrite(D4,HIGH);}break;
  case'5':if(digitalRead(D5)){digitalWrite(D5,LOW);}else{digitalWrite(D5,HIGH);}break;
  case'6':if(digitalRead(D6)){digitalWrite(D6,LOW);}else{digitalWrite(D6,HIGH);}break;
  case'7':if(digitalRead(D7)){digitalWrite(D7,LOW);}else{digitalWrite(D7,HIGH);}break;
  case'8':if(digitalRead(D8)){digitalWrite(D8,LOW);}else{digitalWrite(D8,HIGH);}break;
   }
  }
  
void mqtt_callback(char *topic, byte *payload, unsigned int length) //mqtt回调函数“byte *payload”这东西是个指针
{
    Serial.print("Message arrived [");
    //Serial.print("接受的数据为1：");
    strcpy(str1, (char *)payload);
    //Serial.println(str1);
    Serial.print(topic);
    Serial.print("] ");
    payload[length] = '\0';

   //Serial.print("接受的数据为2：");
  Serial.println((char *)payload);
  //Serial.print("接受的数据为3：");
       char a = *payload;                        //这样我们就可以把指针里的东西拿出来给a
      
        Serial.print("接收到的a:");
       Serial.println(a);
       //int b = *payload;                          
       Serial.print("接受的数据为：");
       Serial.println((char *)payload);
       strcpy(str1, (char *)payload);
       char sss[100]="ni";
       //strcpy(str1, sss);
       
       //IO(a);                                               //这个是PWM功能
       
       }
 

void mqtt_check_connect(){                                        
    while (!mqttClient.connected())
    {
        while (connect_aliyun_mqtt(mqttClient, PRODUCT_KEY, DEVICE_NAME, DEVICE_SECRET))
        {
            Serial.println("MQTT connect succeed!");
            mqttClient.subscribe(ALINK_TOPIC_PROP_SET);          //这个就是引用开始定义的topic
           Serial.println("subscribe done");
           
        }
    }
    
}

void DIS(JsonVariant L)//固定格式，修改参数l
{
  DISA = L["DIS"];//参数l
  
  Serial.printf("收到的DISA是："); Serial.println(DISA);
  //return DIS;
}
/*int back(int a){
  int a1;
  a1=a;
  return a1;
}*/
void text1(JsonVariant L)//固定格式，修改参数l
{
  //str2 = L["text1"];//参数l
  strcpy(str1, L["text1"]);
  Serial.printf("收到的str1是："); Serial.printf(L["text1"]);
  //return DIS;
}
void text2(JsonVariant L)//固定格式，修改参数l
{
  
  strcpy(str2, L["text2"]);
  Serial.printf("收到的str2是："); Serial.printf(L["text2"]);
  
}
void text3(JsonVariant L)//固定格式，修改参数l
{
  
  strcpy(str3, L["text3"]);
  Serial.printf("收到的str3是："); Serial.printf(L["text3"]);
  
}
/*void printBook( struct json1 *book )
{
   Serial.printf( "Book title : %s\n", book->a);
   
}
void json(JsonVariant L)//固定格式，修改参数l
{
  
  //strcpy(json1.a, (L["json"].a));
  Serial.printf("收到的json1.a是："); 
  printBook(&L["json"]);
  
  
}
*/
void display(long follower, long likes, long view)
{
  //u8g2.clearDisplay();
  u8g2.setCursor(5, 25);
  //u8g2.print("粉丝数："+ String(str)+ String(follower));
  u8g2.print("粉丝数："+ String(str1));
  u8g2.setCursor(5, 39);
  u8g2.print("获赞数："+ String(str2));
  //u8g2.print("获赞数：" + String(likes));
  u8g2.setCursor(5, 53);
  u8g2.print("播放数："+ String(str3));
  //u8g2.print("播放数：" + String(view));
  u8g2.setCursor(5, 7);
  u8g2.print("bilibili@haost"); //改成自己的名字
}
