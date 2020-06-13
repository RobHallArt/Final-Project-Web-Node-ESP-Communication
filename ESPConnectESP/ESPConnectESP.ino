/*******************Esp8266_Websocket.ino****************************************/
#include <ESP8266WiFi.h>
#include <WebSocketClient.h>
boolean handshakeFailed=0;
String data= "";
char path[] = "/";   //identifier of this device
const char* ssid     = "showAP";
const char* password = "4000HZ-AP";
char* host = "10.196.251.105";  //replace this ip address with the ip address of your Node.Js server
const int espport= 3000;
  
WebSocketClient webSocketClient;
unsigned long previousMillis = 0;
unsigned long currentMillis;
unsigned long interval=300; //interval for sending data to the websocket server in ms
// Use WiFiClient class to create TCP connections
WiFiClient client;
void setup() {
  Serial.begin(115200);
    pinMode(D0, OUTPUT);
    pinMode(D1, OUTPUT);
    pinMode(D2, OUTPUT);
    pinMode(D3, OUTPUT);
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  delay(1000);
  
wsconnect();
//  wifi_set_sleep_type(LIGHT_SLEEP_T);
}

void loop() {
  if (client.connected()) {
    currentMillis=millis(); 
    webSocketClient.getData(data); 
    //Serial.println(data);
    if (data.length() > 0) {
      //Serial.println("WAGWAN");
      Serial.println(data);
      if(data == "broadcast: A"){
        Serial.println("Flashed 1");
        digitalWrite(D0, HIGH);
        delay(300);
        digitalWrite(D0, LOW);
      }
      if(data == "broadcast: B"){
        Serial.println("Flashed 2");
        digitalWrite(D1, HIGH);
        delay(300);
        digitalWrite(D1, LOW);
      }
      if(data == "broadcast: C"){
        Serial.println("Flashed 3");
        digitalWrite(D2, HIGH);
        delay(300);
        digitalWrite(D2, LOW);
      }
      if(data == "broadcast: D"){
        Serial.println("Flashed 4");
        digitalWrite(D3, HIGH);
        delay(300);
        digitalWrite(D3, LOW);
      }
      data = "";
      //*************send log data to server in certain interval************************************
      //currentMillis=millis();   
      /*
      if (abs(currentMillis - previousMillis) >= interval) {
          previousMillis = currentMillis;
          data= (String) analogRead(A0); //read adc values, this will give random value, since no sensor is connected. 
          //For this project we are pretending that these random values are sensor values
          webSocketClient.sendData(data);//send sensor data to websocket server
      }
      */
    }
    else{
      
    }
  delay(5);
  }
}
//*********************************************************************************************************************
//***************function definitions**********************************************************************************
void wsconnect(){
  // Connect to the websocket server
  if (client.connect(host, espport)) {
    Serial.println("Connected");
  } else {
    Serial.println("Connection failed.");
      delay(1000);  
   
   if(handshakeFailed){
    handshakeFailed=0;
    ESP.restart();
    }
    handshakeFailed=1;
  }
  // Handshake with the server
  webSocketClient.path = path;
  webSocketClient.host = host;
  if (webSocketClient.handshake(client)) {
    Serial.println("Handshake successful");
  } else {
    
    Serial.println("Handshake failed.");
   delay(4000);  
   
   if(handshakeFailed){
    handshakeFailed=0;
    ESP.restart();
    }
    handshakeFailed=1;
  }
}
