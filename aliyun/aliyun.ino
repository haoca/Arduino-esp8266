#include <ESP8266WiFi.h>   //安装esp8266arduino开发环境
#include <PubSubClient.h>  //安装PubSubClient库
#include <ArduinoJson.h>   //json  V5版本
#include <Servo.h>
#include "aliyun_mqtt.h"
//需要安装crypto库

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
unsigned long lastMs = 0;
int pos = 90;

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

    
  
  Serial.println((char *)payload);
  
       char a = *payload;                        //这样我们就可以把指针里的东西拿出来给a
      
        Serial.print("接收到的a:");
       Serial.println(a);
       //int b = *payload;                          
       Serial.print("接受的数据为：");
       Serial.println((char *)payload);
       /*Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    Serial.println((char *)payload);*/
       
       IO(a);                                               //这个是PWM功能
       
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

/*void mqtt_interval_post()
{
    char param[512];
    char jsonBuf[1024];
    
 
    //sprintf(param, "{\"MotionAlarmState\":%d}", digitalRead(13));
    sprintf(param, "{\"LightSwitch\":%d,\"CurrentTemperature\":%d}", digitalRead(D4),random(0,55));
    sprintf(jsonBuf, ALINK_BODY_FORMAT, ALINK_METHOD_PROP_POST, param);
    Serial.println(jsonBuf);
    mqttClient.publish(ALINK_TOPIC_PROP_POST, jsonBuf); //这个是上传数据的topic,jsonBuf这个是上传的数据
}
*/

void setup()
{
  Serial.begin(9600);
   
    

    init_wifi(WIFI_SSID, WIFI_PASSWD);

    mqttClient.setCallback(mqtt_callback);
}



void loop()
{
   if (millis() - lastMs >= 5000)  
    {
        lastMs = millis();
        mqtt_check_connect();
    }
    mqttClient.loop();

   
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
 
 

  
