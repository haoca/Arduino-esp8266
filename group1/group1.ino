//接受oled和舵机消息,微信发送命令从dht11温湿度模块接受消息
#include <ESP8266WiFi.h>          // 本程序使用ESP8266WiFi库
#include <ESP8266WiFiMulti.h>   // 本程序使用ESP8266WiFiMulti库
#include <Servo.h>
ESP8266WiFiMulti wifiMulti;     // 建立ESP8266WiFiMulti对象,对象名称是'wifiMulti'
#include <PubSubClient.h>  //安装PubSubClient库
#include <ArduinoJson.h>   //json  V5版本

#include "aliyun_mqtt.h"
#include <Wire.h>
#include <Ticker.h>
//需要安装crypto库

#include <SimpleDHT.h>
int pinDHT11 = D3;
SimpleDHT11 dht11(pinDHT11);
#define Trig D6 //引脚Tring 连接 IO D2
#define Echo D7 //引脚Echo 连接 IO D3 

float cm; //距离变量
float temp; // 

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
#define ALINK_TOPIC_PROP_SET_oled      "/"PRODUCT_KEY "/" DEVICE_NAME"/user/3" //这个就是我们接收消息的TOPIC        
//------------------------------接收消息的TOPIC ------------------------------------------ 

//***************************这个是上传数据的TOPIC******************************************
#define ALINK_TOPIC_PROP_POST_oled       "/"PRODUCT_KEY "/" DEVICE_NAME"/user/1" //这个是上传数据的TOPIC
//****************************这个是上传数据的TOPIC****************************************   

//-----------------------------接收消息的TOPIC -------------------------------------------
#define ALINK_TOPIC_PROP_SET_servo      "/"PRODUCT_KEY "/" DEVICE_NAME"/user/2" //这个就是我们接收消息的TOPIC        
//------------------------------接收消息的TOPIC ------------------------------------------ 

//***************************这个是上传数据的TOPIC******************************************
#define ALINK_TOPIC_PROP_POST_servo       "/"PRODUCT_KEY "/" DEVICE_NAME"/user/1" //这个是上传数据的TOPIC
//****************************这个是上传数据的TOPIC****************************************   

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display

unsigned long lastMsMain = 0;
int pos = 0,flag=0;
const unsigned char bilibilitv_24u[] U8X8_PROGMEM = {0x00, 0x00, 0x02, 0x00, 0x00, 0x03, 0x30, 0x00, 0x01, 0xe0, 0x80, 0x01,
                                                     0x80, 0xc3, 0x00, 0x00, 0xef, 0x00, 0xff, 0xff, 0xff, 0x03, 0x00, 0xc0, 0xf9, 0xff, 0xdf, 0x09, 0x00, 0xd0, 0x09, 0x00, 0xd0, 0x89, 0xc1,
                                                     0xd1, 0xe9, 0x81, 0xd3, 0x69, 0x00, 0xd6, 0x09, 0x91, 0xd0, 0x09, 0xdb, 0xd0, 0x09, 0x7e, 0xd0, 0x0d, 0x00, 0xd0, 0x4d, 0x89, 0xdb, 0xfb,
                                                     0xff, 0xdf, 0x03, 0x00, 0xc0, 0xff, 0xff, 0xff, 0x78, 0x00, 0x1e, 0x30, 0x00, 0x0c
                                                    };
unsigned long lastMs = 0;

char str1[1024] = "\0",str2[100] = "\0",str3[100] = "\0";    
WiFiClient   espClient;
PubSubClient mqttClient(espClient);
Servo myServo;

void light(int fla,char top_flag){
  if(!(fla==flag)){
   
   
   digitalWrite(D4,LOW);
   delay(200);
   digitalWrite(D4,HIGH);
   int pos=(fla-48)*20;
   
  char jsonBuf[1024];
  char cNumber= (char) (pos+'0');
  //jsonBuf[0]=(char) (pos+'0');
  //Serial.print(cNumber);
  char param[512];
    
    
 
    
    //sprintf(param, "{\"发送方\":%s,\"输入角度\":%d,\"CurrentTemperature\":%d}", top_flag,pos,random(0,55));
    // sprintf(jsonBuf, ALINK_BODY_FORMAT, ALINK_METHOD_PROP_POST, param);
    //sprintf(jsonBuf, ALINK_BODY_FORMAT, ALINK_METHOD_PROP_POST, param);
   // Serial.println(jsonBuf);
    
    if(top_flag=='3'){
    //mqttClient.publish(ALINK_TOPIC_PROP_POST_oled, jsonBuf); //这个是上传数据的topic,jsonBuf这个是上传的数据
    }
    if(top_flag=='2'){
      sprintf(param, "{\"发送方\":%c,\"输入角度\":%d,\"CurrentTemperature\":%d}", top_flag,pos,random(0,55));
      sprintf(jsonBuf, ALINK_BODY_FORMAT, ALINK_METHOD_PROP_POST, param);
    Serial.println(param);
    mqttClient.publish(ALINK_TOPIC_PROP_POST_servo, jsonBuf); //这个是上传数据的topic,jsonBuf这个是上传的数据}
    }
   flag=fla;
   
  
}}

void mqtt_interval_post(float cm)
{
    char param[512];
    char jsonBuf[1024];
    
 
    //sprintf(param, "{\"MotionAlarmState\":%d}", digitalRead(13));
    //sprintf(param, "{\"LightSwitch\":%d,\"CurrentTemperature\":%d}", digitalRead(D4),random(0,55));
    sprintf(param, "{\"距离为\":%2f cm}",cm );
    sprintf(jsonBuf, ALINK_BODY_FORMAT, ALINK_METHOD_PROP_POST, param);
    Serial.println(jsonBuf);
    mqttClient.publish(ALINK_TOPIC_PROP_POST_oled, jsonBuf); //这个是上传数据的topic,jsonBuf这个是上传的数据
    
}
void post(char *cm)
{
    char param[512];
    char jsonBuf[1024];
    
 
    //sprintf(param, "{\"MotionAlarmState\":%d}", digitalRead(13));
    //sprintf(param, "{\"LightSwitch\":%d,\"CurrentTemperature\":%d}", digitalRead(D4),random(0,55));
    //sprintf(param, "{\"距离为\":%2f cm}",cm );
    //sprintf(jsonBuf, ALINK_BODY_FORMAT, ALINK_METHOD_PROP_POST, param);
    Serial.println(cm);
    mqttClient.publish(ALINK_TOPIC_PROP_POST_oled, cm); //这个是上传数据的topic,jsonBuf这个是上传的数据
    
}
void mqtt_callback(char *topic, byte *payload, unsigned int length) //mqtt回调函数“byte *payload”这东西是个指针
{
  char a2;
  char a3;
  int b2=48,b3=0;
    if(topic[24]=='2'){
      Serial.println("\n\n这是2号舵机消息");
      a2 = *payload;
      b2 = a2;  
      //Serial.print(b2);
        
    }
    else if(topic[24]=='3'){
      
      Serial.println("\n\n这是3号oled显示屏消息");
      a3 = *payload;
      //b3 = a3;  
    }
    
    else {
      Serial.println("\n\n这是");Serial.print(topic[24]);Serial.print("号消息");
    }
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] \n");
    
    payload[length] = '\0';

   // https://arduinojson.org/v5/assistant/  json数据解析网站

    
  
  //Serial.println((char *)payload);
  
                               //这样我们就可以把指针里的东西拿出来给a
      
        //Serial.println("接收到的a:");
       //Serial.println(a);
                           
       Serial.print("接受的数据(字符串)为：");
       Serial.println((char *)payload);

       
      if(topic[24]=='2'){
        
                                             //这个是PWM功能
       servo(b2);
      //light(b2,(char ) topic[24]);//a,b无所谓
      int c = myServo.read();
      Serial.print("2号舵机已运行,当前角度为[");Serial.print(c); Serial.print("]度."); 
       
    }
    else if(topic[24]=='3'){
      strcpy(str1, (char *)payload);
      if(strcmp((char *)payload, "\"温度\"")==0){
        Serial.print("温度命令接受\n");
        temp_humidity();
        
      }
      if(strcmp((char *)payload, "\"点灯\"")==0){
        Serial.print("点灯命令接受\n");
        digitalWrite(D4, !digitalRead(D4));
        
      }
      if(strcmp((char *)payload, "\"距离\"")==0){
        Serial.print("距离命令接受\n");
        
        distance();
      }
      //Serial.println("3号oled显示消息");
      //light(b3,(char ) topic[24]);//a,b无所谓
    }
    
    else {
      
       Serial.print("未知消息-");Serial.print(topic[24]);
    }
       
  
       
       //IO(a);                                               //这个是PWM功能
       
       }
 

void mqtt_check_connect(){                                        
    while (!mqttClient.connected())
    {
        while (connect_aliyun_mqtt(mqttClient, PRODUCT_KEY, DEVICE_NAME, DEVICE_SECRET))
        {
            Serial.println("MQTT connect succeed!");
            mqttClient.subscribe(ALINK_TOPIC_PROP_SET_oled);          //这个就是引用开始定义的topic
             mqttClient.subscribe(ALINK_TOPIC_PROP_SET_servo);
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
  Serial.print("提示：温湿度传感器接D3 // 舵机接D5 // 引脚Echo 连接 D3 // 引脚Tring 连接 D2 \n");
}


void servo(int a){
  //Serial.print("432623623623626");  
  int pos=(a-48)*20;
  myServo.write(pos);
  
   
  char jsonBuf[1024];
  char cNumber= (char) (pos+'0');
  //jsonBuf[0]=(char) (pos+'0');
  //Serial.print(cNumber);
  char param[512];
    
    
 
    
    
    
    
 
      sprintf(param, "{\"输入角度\":%d 度\"当前角度\":%d 度}", pos,myServo.read());
      
    Serial.println(param);
    mqttClient.publish(ALINK_TOPIC_PROP_POST_servo,param); //这个是上传数据的topic,jsonBuf这个是上传的数据}
    
         
}

void setup()
{
  
    //连接到wifi
  Serial.begin(9600);
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  wifi_con();
  
  pinMode(D4, OUTPUT);//开灯low
  digitalWrite(D4,HIGH);
   myServo.attach(D5);
  int pos =  myServo.read();                      //设置电机的角度和D0这个电机相同
  
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
  delay(3000);
    
  u8g2.setFont(u8g2_font_wqy12_t_gb2312b);
  u8g2.setFontPosTop();
  u8g2.clearDisplay();
  
  

  
    mqttClient.setCallback(mqtt_callback);


  
  

  
  
}



void loop()
{
  //int a=1,b=2,c=3;
  u8g2.firstPage();
   do
      {
        if (millis() - lastMsMain >= 2000)  
    {
        lastMsMain = millis();
        mqtt_check_connect();
    }
    


        
       
        display();
        //Serial.println(str1);
        mqttClient.loop();
        //temp_humidity();

  
      } while (u8g2.nextPage());// put your main code here, to run repeatedly:
}


 
 


void display()
{
  
  u8g2.setCursor(5, 22);
  
  u8g2.print("收到的信息：");
  u8g2.setCursor(5, 37);
  u8g2.print(String(str1));
  
  
  u8g2.setCursor(5, 7);
  u8g2.print("bilibili@haost"); //改成自己的名字
  u8g2.setCursor(70, 7);
  u8g2.print("角度:"+String(myServo.read())+"度");
  //u8g2.print(myServo.read());u8g2.print("度");
  
}

void temp_humidity(){
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  /*if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err="); Serial.print(SimpleDHTErrCode(err));
    Serial.print(","); Serial.println(SimpleDHTErrDuration(err)); delay(1000);
    return;
  }*/
  int i=0;
  while((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess){
    
    delay(1500);
    i++;
    if(i>2) {
      Serial.print("超时！！可能是由于模块未安装上机\n");mqttClient.publish(ALINK_TOPIC_PROP_POST_oled, "超时！！可能是由于模块未安装上机");return;
    }
    
  }
  //mqtt_interval_post(cm);
  char cm[1024];
  sprintf(cm, "{\"温度\":%d  *C,\"湿度\":%d  H}", (int)temperature,(int)humidity);
  
  mqttClient.publish(ALINK_TOPIC_PROP_POST_oled, cm);
  Serial.print("从DHT11接受成功: \n");
  Serial.print("温度: "); Serial.print((int)temperature); Serial.print(" *C\n"); 
  
  Serial.print("湿度: "); Serial.print((int)humidity); Serial.println(" H");
  
}

void distance(){
  int i=0;
  float temp=0;
  while(int(temp)==0){
    digitalWrite(Trig, LOW); //给Trig发送一个低电平
  delayMicroseconds(2);    //等待 2微妙
  digitalWrite(Trig,HIGH); //给Trig发送一个高电平
  delayMicroseconds(10);    //等待 10微妙
  digitalWrite(Trig, LOW); //给Trig发送一个低电平
  
  temp = float(pulseIn(Echo, HIGH)); //存储回波等待时间,
  //pulseIn函数会等待引脚变为HIGH,开始计算时间,再等待变为LOW并停止计时
  //返回脉冲的长度
  
  
  //声速是:340m/1s 换算成 34000cm / 1000000μs => 34 / 1000
  //因为发送到接收,实际是相同距离走了2回,所以要除以2
  //距离(厘米)  =  (回波时间 * (34 / 1000)) / 2
  //简化后的计算公式为 (回波时间 * 17)/ 1000
  
  
  
  
  delayMicroseconds(50); 
  Serial.print(i);
    i++;
    if(i>7) {
      Serial.print("超时！！可能是由于模块未安装上机\n");mqttClient.publish(ALINK_TOPIC_PROP_POST_oled, "超时！！可能是由于模块未安装上机");return;
    }
  }
  cm = (temp * 17 )/1000; //把回波时间换算成cm
  char param[512];
    
  sprintf(param, "{\"距离为\":%2f cm}",cm );
  mqttClient.publish(ALINK_TOPIC_PROP_POST_oled, param); //这个是上传数据的topic,jsonBuf这个是上传的数据
  
  //mqtt_interval_post(cm);
  Serial.print("Echo =");
  Serial.print(temp);//串口输出等待时间的原始数据
  Serial.print(" | | Distance = ");
  Serial.print(cm);//串口输出距离换算成cm的结果
  Serial.println("cm");
  
}
  
