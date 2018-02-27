#include <ESP8266WiFi.h>

#define LED_ON digitalWrite(LED_BUILTIN, LOW)
#define LED_OFF digitalWrite(LED_BUILTIN, HIGH)

const char* ssid = "Robocon";
const char* pswd = "duoguanriben8";
const char* host = "192.168.2.105";
const int port = 8266;
const char* streamID = "";
const char* privateKey = "";

WiFiClient client;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT); 
  LED_ON;
  Serial.begin(115200);
  delay(100);
  //Serial.println("");
  //Serial.print("Connecting to ");
  //Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pswd);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }
  //Serial.println("");
  //Serial.println("WiFi Connected");
  //Serial.print("IP Address : ");
  //Serial.println(WiFi.localIP());
  delay(10);
  // put your main code here, to run repeatedly:
  //Serial.print("Connecting to host : ");
  //.println(host);

  if (!client.connect(host, port)) {
    //Serial.println("host connection failed");
    return ;
  }
  else {
    //Serial.println("Connect to host success!");
    LED_OFF;
  }
}

String tcp_rx_buf;
String uart_rx_buf;

bool readFromUART() {
  bool uart_rx_flag = false;
  bool first_in = true;
  while (1) {
    if (Serial.available()) { ///< 如果串口有输入
      if (first_in) {
        first_in = false;
        uart_rx_buf = ""; ///< 对第一个字符，清空rx_buf
      }
      uart_rx_flag = true;
      char rx_char = Serial.read();
      uart_rx_buf += rx_char;
    }
    else {
      break;
    }
  }
  if (uart_rx_flag) {
    return true;
  }
  return false;
}

bool readFromTCP() {
  bool tcp_rx_flag = false;
  bool first_in = true;
  while (1) {
    if (client.available()) { ///< 从TCP服务器中收到消息
      if (first_in == true) { ///< 如果是第一个字节
        first_in = false;
        tcp_rx_buf = "";  ///< 就先清空接收缓存
      }
      tcp_rx_flag = true;
      char rx_char = client.read();
      tcp_rx_buf += rx_char;
    }
    else {  ///< 当没有可接受的消息，则退出循环
      break;
    }
  }
  if (tcp_rx_flag == true) {
    return true;
  }
  return false;
}
void loop() {
  if(readFromUART()){
    client.print(uart_rx_buf);  
  }
  if (readFromTCP()) {
    Serial.println(tcp_rx_buf);
  }
}
