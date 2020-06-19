
// Import relevant Libraries
#include <ESP8266WiFi.h>
#include <WebSocketClient.h>
// Initialise Variables
boolean handshakeFailed=0;
String data= "";
char path[] = "/";
// Set these to the SSID and PASSWORD of your Wifi and please don't steal my wifi...
const char* ssid     = "showAP";
const char* password = "4000HZ-AP";
char* host = "10.196.251.105";  // Replace this ip address with the ip address of your Node.Js local relay server as per instruction
const int espport= 3000; // Leave this as 3000 unless changed in localrelay

WebSocketClient webSocketClient;
unsigned long previousMillis = 0;
unsigned long currentMillis;
unsigned long interval=300; // Interval for sending data to the websocket server in ms

// Use WiFiClient class to create TCP connections
WiFiClient client;

void setup() {
  Serial.begin(115200); // Begin Serial Debug

  // Set PinOuts
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  delay(10);
  
  // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) { // While Wifi is not connected
    delay(500);
    Serial.print("."); // Print a '.' to the serial monitor every 500ms (Makes a kind of loading bar 
  }

  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  delay(1000);
  
  wsconnect(); // Call websocket setup function
}

void loop() {
  
  if (client.connected()) { // If the client (This Device) is connected

    // Set some defaults
    currentMillis=millis(); 
    webSocketClient.getData(data); 
    
    if (data.length() > 0) {
      Serial.println(data); // Print the recieved data to serial as a debug tool.

      // For each possible message A/B/C/D put the relevant pin high for 300ms then return to low.
      
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
      data = ""; // Empty data buffer once we are done so future messages aren't appended.
    }
  delay(5);
  }
}


void wsconnect(){
  // Connect to the websocket server
  if (client.connect(host, espport)) { // Connect using the host and port we set at the top.
    Serial.println("Connected"); // If we succeeded - log it!
  } else {
    Serial.println("Connection failed."); // If not - Log it but we'll investigate why.
      delay(1000);  
   
   if(handshakeFailed){ // If the reason is that the handshake fialed
    handshakeFailed=0; // Reset the variable
    ESP.restart(); // Restart and try again.
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
