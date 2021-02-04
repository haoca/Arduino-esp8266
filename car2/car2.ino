#define ENA    5          // Enable/speed motors Right          GPIO 5(D1)
//#define ENA_2  4          // Enable/speed motors Right_2        GPIO 4(D2)
#define ENB    4          // Enable/speed motors Left           GPIO 0(D3)
//#define ENB_2  2          // Enable/speed motors Left_2         GPIO 2(D4)
#define IN_1  14          // L298N in1 motors Right             GPIO14(D5)
#define IN_2  12          // L298N in2 motors Right             GPIO12(D6)
#define IN_3  13          // L298N in3 motors Left              GPIO13(D7)
#define IN_4  15          // L298N in4 motors Left              GPIO15(D8)
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
 #include <ESP8266WiFiMulti.h>   // 本程序使用ESP8266WiFiMulti库

ESP8266WiFiMulti wifiMulti;     // 建立ESP8266WiFiMulti对象,对象名称是'wifiMulti'
/* Wifi ssid and pwd */
//const char *ssid = "千户";
//const char *password = "13307023186";
 
/* your car speed */
int speedCar = 300;         // 0 - 1023.
int speedInterval = 90;     //速度间隔
int speedTrun = 1.5;        //转弯系数
 
ESP8266WebServer server(80);
String webPage = "";
 void wifi_con(){
  //wifiMulti.addAP("TP666666", "qwe123456789");  // 这三条语句通过调用函数addAP来记录3个不同的WiFi网络信息。
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
/* 192.168.4.1 */
void handleRoot() {
  server.send(200, "text/html", "<h1>You are connected</h1>");
}
 
void setup() {
  wifi_con();
  webPage += "<div align=\"center\"><h1>ESP8266 CAR</h1>";
  webPage += "<a href=\"FL\"><button style=\"height:200px;width:200px\"><font size=\"20\">FL</font></button></a>";
  webPage += "<a href=\"F2\"><button style=\"height:200px;width:200px\"><font size=\"20\">FW</font></button></a>";
  webPage += "<a href=\"FR\"><button style=\"height:200px;width:200px\"><font size=\"20\">FR</font></button></a><br>";
  webPage += "<a href=\"L2\"><button style=\"height:200px;width:200px\"><font size=\"20\">LT</font></button></a>";
  webPage += "<a href=\"S2\"><button style=\"height:200px;width:200px\"><font size=\"20\">ST</font></button></a>";
  webPage += "<a href=\"R2\"><button style=\"height:200px;width:200px\"><font size=\"20\">RT</font></button></a><br>";
  webPage += "<a href=\"BL\"><button style=\"height:200px;width:200px\"><font size=\"20\">BL</font></button></a>";
  webPage += "<a href=\"B2\"><button style=\"height:200px;width:200px\"><font size=\"20\">BC</font></button></a>";
  webPage += "<a href=\"BR\"><button style=\"height:200px;width:200px\"><font size=\"20\">BR</font></button></a></div>";
  webPage += "<div align=\"center\"><h1>SPEED CHANGE</h1>";
  webPage += "<a href=\"SE1\"><button style=\"height:200px;width:200px\"><font size=\"20\">1</font></button></a>";
  webPage += "<a href=\"SE2\"><button style=\"height:200px;width:200px\"><font size=\"20\">2</font></button></a>";
  webPage += "<a href=\"SE3\"><button style=\"height:200px;width:200px\"><font size=\"20\">3</font></button></a><br>";
  webPage += "<a href=\"SE4\"><button style=\"height:200px;width:200px\"><font size=\"20\">4</font></button></a>";
  webPage += "<a href=\"SE5\"><button style=\"height:200px;width:200px\"><font size=\"20\">5</font></button></a>";
  webPage += "<a href=\"SE6\"><button style=\"height:200px;width:200px\"><font size=\"20\">6</font></button></a><br>";
  webPage += "<a href=\"SE7\"><button style=\"height:200px;width:200px\"><font size=\"20\">7</font></button></a>";
  webPage += "<a href=\"SE8\"><button style=\"height:200px;width:200px\"><font size=\"20\">8</font></button></a>";
  webPage += "<a href=\"SE9\"><button style=\"height:200px;width:200px\"><font size=\"20\">9</font></button></a>";
 
  // preparing GPIOs
  pinMode(ENA, OUTPUT);
  //digitalWrite(ENA, 1);
  pinMode(ENB, OUTPUT);
  //digitalWrite(ENB, 1);  
  pinMode(IN_1, OUTPUT);
  // digitalWrite(IN_1, 0);
  pinMode(IN_2, OUTPUT);
  // digitalWrite(IN_2, 0);
  pinMode(IN_3, OUTPUT);
  // digitalWrite(IN_3, 0);
  pinMode(IN_4, OUTPUT);
  // digitalWrite(IN_4, 0);
 
  Serial.begin(115200);
  //WiFi.softAP(ssid, password);
 //Serial.println(WiFi.SSID());              // 连接的WiFI名称
  //Serial.print("IP address:\t");            // 以及
  //Serial.println(WiFi.localIP());           // NodeMCU的IP地址
  server.on("/", []() {
    server.send(200, "text/html", webPage);
  });
  server.on("/FL", []() {
    server.send(200, "text/html", webPage);
    FL();
  });
  server.on("/F2", []() {
    server.send(200, "text/html", webPage);
    F2();
  });
  server.on("/FR", []() {
    server.send(200, "text/html", webPage);
    FR();
  });
  server.on("/L2", []() {
    server.send(200, "text/html", webPage);
    L2();
  });
  server.on("/S2", []() {
    server.send(200, "text/html", webPage);
    S2();
  });
  server.on("/R2", []() {
    server.send(200, "text/html", webPage);
    R2();
  });
  server.on("/BL", []() {
    server.send(200, "text/html", webPage);
    BL();
  });
  server.on("/B2", []() {
    server.send(200, "text/html", webPage);
    B2();
  });
  server.on("/BR", []() {
    server.send(200, "text/html", webPage);
    BR();
  });
 
  server.on("/SE1", []() {
    server.send(200, "text/html", webPage);
    CS(1);
  });
  server.on("/SE2", []() {
    server.send(200, "text/html", webPage);
    CS(2);
  });
  server.on("/SE3", []() {
    server.send(200, "text/html", webPage);
    CS(3);
  });
  server.on("/SE4", []() {
    server.send(200, "text/html", webPage);
    CS(4);
  });
  server.on("/SE5", []() {
    server.send(200, "text/html", webPage);
    CS(5);
  });
  server.on("/SE6", []() {
    server.send(200, "text/html", webPage);
    CS(6);
  });
  server.on("/SE7", []() {
    server.send(200, "text/html", webPage);
    CS(7);
  });
  server.on("/SE8", []() {
    server.send(200, "text/html", webPage);
    CS(8);
  });
  server.on("/SE9", []() {
    server.send(200, "text/html", webPage);
    CS (9);
  });
 
  server.begin();
  Serial.println("HTTP server started");
  Serial.println(WiFi.SSID());              // 连接的WiFI名称
  Serial.print("IP address:\t");            // 以及
  Serial.println(WiFi.localIP());           // NodeMCU的IP地址
}
 
/*******************************************/
void FL() {
  analogWrite(ENA, speedCar);
  analogWrite(ENB, speedCar / speedTrun);
  digitalWrite(IN_1, 0);
  digitalWrite(IN_2, 1);
 //val = analogRead(analogPin);
 Serial.println(analogRead(ENA));
  digitalWrite(IN_3, 0);
  digitalWrite(IN_4, 1);
}
 
void F2() {
  analogWrite(ENA, speedCar);
  analogWrite(ENB, speedCar);
  digitalWrite(IN_1, 0);
  digitalWrite(IN_2, 1);
 Serial.println(analogRead(ENA));Serial.println("  ppppp  \n");Serial.println(speedCar);
  digitalWrite(IN_3, 0);
  digitalWrite(IN_4, 1);
 
}
 
void FR() {
  analogWrite(ENA, speedCar / speedTrun);
  analogWrite(ENB, speedCar);
  digitalWrite(IN_1, 0);
  digitalWrite(IN_2, 1);
 Serial.println(analogRead(ENA));
  digitalWrite(IN_3, 0);
  digitalWrite(IN_4, 1);
}
 
void L2() {
  analogWrite(ENA, speedCar);
  analogWrite(ENB, speedCar);
  digitalWrite(IN_1, 0);
  digitalWrite(IN_2, 1);
 Serial.println(analogRead(ENA));
  digitalWrite(IN_3, 1);
  digitalWrite(IN_4, 0);
}
 
void S2() {
  digitalWrite(IN_1, 0);
  digitalWrite(IN_2, 0);
  digitalWrite(IN_3, 0);
  digitalWrite(IN_4, 0);
  Serial.println(analogRead(ENA));
}
 
void R2() {
  analogWrite(ENA, speedCar);
  analogWrite(ENB, speedCar);
  digitalWrite(IN_1, 1);
  digitalWrite(IN_2, 0);
 Serial.println(analogRead(ENA));
  digitalWrite(IN_3, 0);
  digitalWrite(IN_4, 1);
}
 
void BL() {
  analogWrite(ENA, speedCar);
  analogWrite(ENB, speedCar / speedTrun);
  digitalWrite(IN_1, 1);
  digitalWrite(IN_2, 0);
 Serial.println(analogRead(ENA));
  digitalWrite(IN_3, 1);
  digitalWrite(IN_4, 0);
}
 
void B2() {
  //speedCar=speedCar-200;
  analogWrite(ENA, speedCar);
  analogWrite(ENB, speedCar);
  digitalWrite(IN_1, 1);
  digitalWrite(IN_2, 0);
 Serial.println(analogRead(ENA));Serial.println("     ppp   ");Serial.println(speedCar);
  digitalWrite(IN_3, 1);
  digitalWrite(IN_4, 0);
}
 
void BR() {
  analogWrite(ENA, speedCar / speedTrun);
  analogWrite(ENB, speedCar);
  digitalWrite(IN_1, 1);
  digitalWrite(IN_2, 0);
 Serial.println(analogRead(ENA));
  digitalWrite(IN_3, 1);
  digitalWrite(IN_4, 0);
}
 
void CS(int NUM) {
  speedCar = 1000 - (9 - NUM) * speedInterval;
  analogWrite(ENA, speedCar);
  analogWrite(ENB, speedCar);
  Serial.println(analogRead(ENA));
}
 
/*******************************************/
 
void loop() {
  server.handleClient();
  
}
