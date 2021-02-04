//接受消息，oled
#include <ESP8266WiFi.h>          // 本程序使用ESP8266WiFi库
#include <ESP8266WiFiMulti.h>   // 本程序使用ESP8266WiFiMulti库

ESP8266WiFiMulti wifiMulti;     // 建立ESP8266WiFiMulti对象,对象名称是'wifiMulti'
#include <PubSubClient.h>  //安装PubSubClient库
#include <ArduinoJson.h>   //json  V5版本

#include "aliyun_mqtt.h"
#include <Wire.h>
#include <Ticker.h>
//需要安装crypto库

#include <U8g2lib.h>

#define WIFI_SSID        "千户"//替换自己的WIFI
#define WIFI_PASSWD      "13307023186"//替换自己的WIFI

#define PRODUCT_KEY      "a1N8FKV28Xa" //替换自己的PRODUCT_KEY
#define DEVICE_NAME      "text2" //替换自己的DEVICE_NAME
#define DEVICE_SECRET    "d63a68c4a626d69fb40855935e4d1f4f"//替换自己的DEVICE_SECRET

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

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display

unsigned long lastMsMain = 0;

const unsigned char bilibilitv_24u[] U8X8_PROGMEM = {0x00, 0x00, 0x02, 0x00, 0x00, 0x03, 0x30, 0x00, 0x01, 0xe0, 0x80, 0x01,
                                                     0x80, 0xc3, 0x00, 0x00, 0xef, 0x00, 0xff, 0xff, 0xff, 0x03, 0x00, 0xc0, 0xf9, 0xff, 0xdf, 0x09, 0x00, 0xd0, 0x09, 0x00, 0xd0, 0x89, 0xc1,
                                                     0xd1, 0xe9, 0x81, 0xd3, 0x69, 0x00, 0xd6, 0x09, 0x91, 0xd0, 0x09, 0xdb, 0xd0, 0x09, 0x7e, 0xd0, 0x0d, 0x00, 0xd0, 0x4d, 0x89, 0xdb, 0xfb,
                                                     0xff, 0xdf, 0x03, 0x00, 0xc0, 0xff, 0xff, 0xff, 0x78, 0x00, 0x1e, 0x30, 0x00, 0x0c
                                                    };
unsigned long lastMs = 0;
int pos = 90;
char str1[100] = "\0",str2[100] = "\0",str3[100] = "\0";    
WiFiClient   espClient;
PubSubClient mqttClient(espClient);

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

void mqtt_callback(char *topic, byte *payload, unsigned int length) //mqtt回调函数“byte *payload”这东西是个指针
{
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    payload[length] = '\0';

   // https://arduinojson.org/v5/assistant/  json数据解析网站

    
  
  //Serial.println((char *)payload);
  
       /*char a = *payload;                        //这样我们就可以把指针里的东西拿出来给a
      
        Serial.print("接收到的a:");
       Serial.println(a);
       //int b = *payload;  */                        
       Serial.print("接受的数据为：");
       Serial.println((char *)payload);
       strcpy(str1, (char *)payload);
       /*Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    Serial.println((char *)payload);*/
       
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

void wifi_con(){
  wifiMulti.addAP("TP666666", "qwe123456789");  // 这三条语句通过调用函数addAP来记录3个不同的WiFi网络信息。
  wifiMulti.addAP("千户", "13307023186"); // 这3个WiFi网络名称分别是taichi-maker, taichi-maker2, taichi-maker3。
  //wifiMulti.addAP("taichi-maker3", "13572468"); // 这3个网络的密码分别是123456789，87654321，13572468。
                                                // 此处WiFi信息只是示例，请在使用时将需要连接的WiFi信息填入相应位置。
                                                // 另外这里只存储了3个WiFi信息，您可以存储更多的WiFi信息在此处。
                                                
  Serial.println("Connecting ...");         // 通过串口监视器输出信息告知用户NodeMCU正在尝试连接WiFi
  int i = 0;                                 
  while (wifiMulti.run() != WL_CONNECTED) {  // 此处的wifiMulti.run()是重点。通过wifiMulti.run()，NodeMCU将会在当前
    delay(1000);                             // 环境中搜索addAP函数所存储的WiFi。如果搜到多个存储的WiFi那么NodeMCU
    Serial.print('.');                       // 将会连接信号最强的那一个WiFi信号。
  }                                           // 一旦连接WiFI成功，wifiMulti.run()将会返回“WL_CONNECTED”。这也是
                                              // 此处while循环判断是否跳出循环的条件。
 
  
  Serial.println('\n');                     // WiFi连接成功后
  Serial.print("Connected to ");            // NodeMCU将通过串口监视器输出。
  Serial.println(WiFi.SSID());              // 连接的WiFI名称
  Serial.print("IP address:\t");            // 以及
  Serial.println(WiFi.localIP());           // NodeMCU的IP地址
}
void setup()
{
  
    //连接到wifi
  Serial.begin(9600);
  //init_wifi(WIFI_SSID, WIFI_PASSWD);
  wifi_con();
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
  
  

  

    mqttClient.setCallback(mqtt_callback);

  //初始化 iot，需传入 wifi 的 client，和设备产品信息
  
  Serial.println("MQTT connect succeed!");
  
  Serial.println("subscribe done");
  //绑定一个设备属性回调，当远程修改此属性，会触发LED函数
  
  
}



void loop()
{
  int a=1,b=2,c=3;
  u8g2.firstPage();
   do
      {
        if (millis() - lastMsMain >= 2000)  
    {
        lastMsMain = millis();
        mqtt_check_connect();
    }
    


        
       
        display(a, b, c);
        //Serial.println(str1);
        mqttClient.loop();
        

  
      } while (u8g2.nextPage());// put your main code here, to run repeatedly:
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
 
 


void display(long follower, long likes, long view)
{
  //u8g2.clearDisplay();
  u8g2.setCursor(5, 22);
  //u8g2.print("粉丝数："+ String(str)+ String(follower));
  u8g2.print("收到的信息：");
  u8g2.setCursor(5, 37);
  u8g2.print(String(str1));
  //delay(500);
  
  //u8g2.print("获赞数：" + String(likes));
  //u8g2.print(String(str1));
  //u8g2.print("获赞数：" + String(likes));
  
  u8g2.setCursor(5, 7);
  u8g2.print("bilibili@haost"); //改成自己的名字
}
  
