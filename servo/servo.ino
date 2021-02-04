//接受oled和舵机消息,微信发送命令从dht11温湿度模块接受消息
#include <ESP8266WiFi.h>          // 本程序使用ESP8266WiFi库
#include <ESP8266WiFiMulti.h>   // 本程序使用ESP8266WiFiMulti库
#include <Servo.h>
ESP8266WiFiMulti wifiMulti;     // 建立ESP8266WiFiMulti对象,对象名称是'wifiMulti'
#include <PubSubClient.h>  //安装PubSubClient库
#include <ArduinoJson.h>   //json  V5版本


#include <Wire.h>
#include <Ticker.h>

unsigned long lastMsMain = 0;
int pos = 0,flag=0;

WiFiClient   espClient;
PubSubClient mqttClient(espClient);
Servo myServo;


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
  Serial.print("提示：温湿度传感器接D3,舵机接D0\n");
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
    
    
 
    
    
    
    
 
      sprintf(param, "{\"输入角度\":%d,\"当前角度\":%d \"度\"}", pos,myServo.read());
      
    Serial.println(param);
    //mqttClient.publish(ALINK_TOPIC_PROP_POST_servo,param); //这个是上传数据的topic,jsonBuf这个是上传的数据}
    
         
}

void setup()
{
  
    //连接到wifi
  Serial.begin(9600);
 
  wifi_con();
  
   myServo.attach(D5);
  int pos =  myServo.read();                      //设置电机的角度和D0这个电机相同
  
  
  

  
  
  

  
  
}

char cmd11;
String  cmd1 = "";
void loop()
{
   if (Serial.available()>0) {     // 检查串口缓存是否有数据等待传输
    delay(100);
  if (Serial.available()>0) {
  //cmd1 = Serial.read();        // 获取上位机电机标志位
   cmd1=Serial.readString();
   
  Serial.print(F("cmd1 = "));
  Serial.print(cmd1);   
    Serial.print(F(" , \n"));
    //Serial.print(Strtoint((const char*) cmd1));
    strr(cmd1);
    cmd1 = "";
    delay(10);}}
    //servo(cmd1);
  //delay(1000);Serial.print(myServo.read()+"\n");
}
/*
int Strtoint(const char* str)
{
    
    long long num = 0;
    if (str != NULL&&*str != '\0')
    {
        bool minus = false;
        if (*str == '+')
            str++;
        else if (*str == '-')
        {
            str++;
            minus = true;
        }
        if (*str != '\0')
            num = Strtointcode(str, minus);
            
    }
    return (int)num;
}
*/
int strr(String m){
  
  int n;
  sscanf(m,"%d",&n);
  printf("%d",n);
  return 0;
}

/*
 * //定义一个comdata字符串变量，赋初值为空值
    String comdata = "";
    //numdata是分拆之后的数字数组
    int numdata[6] = {0}, PWMPin[6] = {3, 5, 6, 9, 10, 11}, mark = 0;
    void setup()
    {
    //定义0~6脚是输出
      for(int i = 0; i < 6; i++) pinMode(PWMPin[i], OUTPUT);
      Serial.begin(9600);
    }
     
    void loop()
    {
    //j是分拆之后数字数组的位置记数
      int j = 0;
     
      //不断循环检测串口缓存，一个个读入字符串，
      while (Serial.available() > 0)
      {
      //读入之后将字符串，串接到comdata上面。
        comdata += char(Serial.read());
          //延时一会，让串口缓存准备好下一个数字，不延时会导致数据丢失，
        delay(2);
        //标记串口读过数据，如果没有数据的话，直接不执行这个while了。
        mark = 1;
      }
     
      if(mark == 1)  //如果接收到数据则执行comdata分析操作，否则什么都不做。
      {
      //显示刚才输入的字符串（可选语句）
        Serial.println(comdata);
          //显示刚才输入的字符串长度（可选语句）
        Serial.println(comdata.length());
     
    /*******************下面是重点*******************
    //以串口读取字符串长度循环，
        for(int i = 0; i < comdata.length() ; i++)
        {
        //逐个分析comdata[i]字符串的文字，如果碰到文字是分隔符（这里选择逗号分割）则将结果数组位置下移一位
        //即比如11,22,33,55开始的11记到numdata[0];碰到逗号就j等于1了，
        //再转换就转换到numdata[1];再碰到逗号就记到numdata[2];以此类推，直到字符串结束
          if(comdata[i] == ',')
          {
            j++;
          }
          else
          {
             //如果没有逗号的话，就将读到的数字*10加上以前读入的数字，
             //并且(comdata[i] - '0')就是将字符'0'的ASCII码转换成数字0（下面不再叙述此问题，直接视作数字0）。
             //比如输入数字是12345，有5次没有碰到逗号的机会，就会执行5次此语句。
             //因为左边的数字先获取到，并且numdata[0]等于0，
             //所以第一次循环是numdata[0] = 0*10+1 = 1
             //第二次numdata[0]等于1，循环是numdata[0] = 1*10+2 = 12
             //第三次是numdata[0]等于12，循环是numdata[0] = 12*10+3 = 123
             //第四次是numdata[0]等于123，循环是numdata[0] = 123*10+4 = 1234
             //如此类推，字符串将被变成数字0。
            numdata[j] = numdata[j] * 10 + (comdata[i] - '0');
          }
        }
        //comdata的字符串已经全部转换到numdata了，清空comdata以便下一次使用，
        //如果不请空的话，本次结果极有可能干扰下一次。
        comdata = String("");
     
     
        //循环输出numdata的内容，并且写到PWM引脚
        for(int i = 0; i < 6; i++)
        {
          Serial.print("Pin ");
          Serial.print(PWMPin[i]);
          Serial.print(" = ");
          Serial.println(numdata[i]);
          analogWrite(PWMPin[i], numdata[i]);
          numdata[i] = 0;
        }
        //输出之后必须将读到数据的mark置0，不置0下次循环就不能使用了。
        mark = 0;
      }
    }
    */
 

  
