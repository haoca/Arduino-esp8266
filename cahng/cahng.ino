#include <ESP8266WiFi.h>//安装esp8266arduino开发环境
static WiFiClient espClient;


#include <ArduinoJson.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <Ticker.h>

#include <AliyunIoTSDK.h>//引入阿里云 IoT SDK
//需要安装crypto库、PubSubClient库

//设置产品和设备的信息，从阿里云设备信息里查看
#define PRODUCT_KEY     "a1N8FKV28Xa"//替换自己的PRODUCT_KEY
#define DEVICE_NAME     "text1"//替换自己的DEVICE_NAME
#define DEVICE_SECRET   "0cb2fa1cb5575e9a0232bcde9d15177b"//替换自己的DEVICE_SECRET
#define REGION_ID       "cn-shanghai"//默认cn-shanghai

#define WIFI_SSID       "千户"//替换自己的WIFI
#define WIFI_PASSWD     "13307023186"//替换自己的WIFI

unsigned long lastMsMain = 0;




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
void setup()
{
  Serial.begin(9600);
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
  wifiInit(WIFI_SSID, WIFI_PASSWD);

  //初始化 iot，需传入 wifi 的 client，和设备产品信息
  AliyunIoTSDK::begin(espClient, PRODUCT_KEY, DEVICE_NAME, DEVICE_SECRET, REGION_ID);

  //绑定一个设备属性回调，当远程修改此属性，会触发LED函数
  
  AliyunIoTSDK::bindData("DIS", DIS);
  AliyunIoTSDK::bindData("text1", text1);
  AliyunIoTSDK::bindData("text2", text2);
  AliyunIoTSDK::bindData("text3", text3);
  //AliyunIoTSDK::bindData("json", json);
}

void loop()
{
  
  int a=10,b=100,c=1000;
  u8g2.firstPage();
      do
      {
        a=DISA;
        display(a, b, c);
        AliyunIoTSDK::loop();//必要函数

  if (millis() - lastMsMain >= 2000)//每2秒发送一次
  {
    lastMsMain = millis();

    //发送LED状态到云平台（高电平：1；低电平：0）
    AliyunIoTSDK::send("LEDPIN状态", digitalRead(LED_BUILTIN));
  }
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
void OLED_ShowText(u8 x,u8 y,u8* str,u8 flag)
{
  u8 tempstr[3] = {'\0'};
  while(*str!='\0')
  {
    if(*str&0x80)
    {
      tempstr[0]=*str++;
      tempstr[1]=*str++;
      //OLED_ShowCHineseWord(x,y,tempstr,flag);
      u8g2.setCursor(5, 51);
      u8g2.print('\0');
      x+=16;
      if(*str&0x80)//判断下一个字符是中文还是英文
      {
        if(x>=112){y++;y++;x=0;}//修改地址 
      }
      else
      {
        if(x>=120){y++;y++;x=0;}//修改地址
      }

    }
    else
    {
      //OLED_ShowChar(x,y,*str++,flag);
      u8g2.setCursor(5, 51);
      u8g2.print(*str++);
      x+=8;
      if(*str&0x80)
      {
        if(x>=112){y++;y++;x=0;}//修改地址 
      }
      else
      {
        if(x>=120){y++;y++;x=0;}//修改地址
      }
    }
  }
}

void display(long follower, long likes, long view)
{
  //u8g2.clearDisplay();
  int len=5;
  len=5+strlen(str1);
  u8g2.setCursor(5, 21);
  u8g2.print("接收到的信息：");
  u8g2.setCursor(5, 36);
  
  u8g2.print(str1);
  //OLED_ShowText(5, 51,((u8 *)str1),1);
  //u8g2.print("粉丝数："+ String(str)+ String(follower));
  /*u8g2.print("粉丝数："+ String(str1));
  u8g2.setCursor(5, 39);
  u8g2.print("获赞数："+ String(str2));
  //u8g2.print("获赞数：" + String(likes));
  u8g2.setCursor(5, 53);
  u8g2.print("播放数："+ String(str3));
  //u8g2.print("播放数：" + String(view));*/
  u8g2.setCursor(5, 7);
  u8g2.print("bilibili@haost"); //改成自己的名字
}
