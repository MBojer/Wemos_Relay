/*
Pins
  D0 = 16
  D1 = 5 - Relay 1
  D2 = 4 - Relay 2
  D3 = Not used
  D4 =  Not used (BUILTIN_LED)
  D5 = 14 - Relay 3
  D6 = 12 - Relay 4
  D7 = 13 - Relay 5
  D8 = 15 -


WiFi.status() =
  0 : WL_IDLE_STATUS when Wi-Fi is in process of changing between statuses
  1 : WL_NO_SSID_AVAILin case configured SSID cannot be reached
  3 : WL_CONNECTED after successful connection is established
  4 : WL_CONNECT_FAILED if password is incorrect
  6 : WL_DISCONNECTED if module is not configured in station mode
*/

#include <Arduino.h>

// ---------------------------------------- WiFi ----------------------------------------
#include <ESP8266WiFi.h>

WiFiClient WiFi_Client;

const char* WiFi_SSID = "NoInternetHere";
const char* WiFi_Password = "NoPassword1!";
String WiFi_Hostname = "MiniRelay1";


// ---------------------------------------- MQTT ----------------------------------------
#include <PubSubClient.h>
// MQTT Library ==> https://github.com/knolleary/pubsubclient.git

PubSubClient MQTT_Client(WiFi_Client);

IPAddress MQTT_Broker(192, 168, 0, 2);

String MQTT_Device_ID = WiFi_Hostname;
const char* MQTT_Username = "DasBoot";
const char* MQTT_Password = "NoSinking";

unsigned long MQTT_KeepAlive_Delay = 60000;
unsigned long MQTT_KeepAlive_At = MQTT_KeepAlive_Delay;

const char* MQTT_Subscribe_Topic = "/Boat/Relay/2";


// ---------------------------------------- Relay ----------------------------------------
const int Relay_Number_Of = 5;
const int Relay_Pin[Relay_Number_Of] {D1, D2, D5, D6, D7};

bool Relay_State[Relay_Number_Of];

bool Relay_On_State = LOW;


// // ---------------------------------------- WebServer ----------------------------------------
// WiFiServer server(80);
//
//
//
// // ---------------------------------------- HTTP_GET() ----------------------------------------
// String HTTP_GET(String Server, int Port, String URL) {
//
//   // Attempt to make a connection to the remote server
//   if ( !WiFi_Client.connect(Server, Port) ) {
//     return "Connection FAILED";
//   }
//
//   // This will send the request to the server
//   WiFi_Client.print(String("GET /") + URL + " HTTP/1.1\r\n" +
//                "Host: " + Server + "\r\n" +
//                "Connection: close\r\n\r\n");
//   delay(50);
//
//   // Read all the lines of the reply from server and print them to Serial
//   String Reply;
//   while(WiFi_Client.available()){
//     Reply += WiFi_Client.readStringUntil('\r');
//   }
//
//   return Reply;
// }
//
//
// // ---------------------------------------- WebServer() ----------------------------------------
// void WebServer() {
//
//   // Status check
//   if (server.status() == false) {
//     if (WiFi.status() == 3) {
//       // Start the server
//       server.begin();
//       Serial.println("Server started");
//
//       // Print the IP address
//       Serial.print("Use this URL : ");
//       Serial.print("http://");
//       Serial.print(WiFi.localIP());
//       Serial.println("/");
//     }
//   }
//
//   else if (server.status() == true) {
//     if (WiFi.status() != 3) {
//       server.stop();
//     }
//   }
//
//   // Check if a client has connected
//   WiFiClient client = server.available();
//   if (!client) {
//     return;
//   }
//
//   // Wait until the client sends some data
//   while(!client.available()){
//     delay(1);
//   }
//
//   // Read the first line of the request
//   String request = client.readStringUntil('\r');
//   client.flush();
//
//   // Match the request
//   if (request.indexOf("GET /Relay_") != -1) {
//
//     request.replace("GET /Relay_", "");
//     request.replace(" HTTP/1.1", "");
//
//     byte Selected_Relay = request.substring(0, request.indexOf("-")).toInt();
//
//     byte Selected_State = request.substring(request.indexOf("-") + 1, request.length()).toInt();
//
//     if (Selected_Relay <= Relay_Number_Of) {
//       Selected_Relay = Selected_Relay - 1; // Done to compensate for the array number beaingg -1 in relation to relay number
//
//       Relay_State[Selected_Relay] = Selected_State;
//
//       // If Relay_On_State is low relays require low as on signal
//       if (Relay_On_State != HIGH) Selected_State = !Selected_State;
//
//
//       digitalWrite(Relay_Pin[Selected_Relay], Selected_State);
//
//       Serial.print("Relay " + String(Selected_Relay + 1) + ": ");
//       if (Relay_State[Selected_Relay] == 0) Serial.println("OFF");
//       else Serial.println("ON");
//     }
//
//     else if (Selected_Relay == 99) {
//       for (byte i = 0; i < 6; i++) {
//         Relay_State[i] = 0;
//         digitalWrite(Relay_Pin[i], 0);
//       }
//       Serial.println("All OFF");
//     }
//   }
//
//   else Serial.println(request);
//
//   // Return the response
//   client.println("HTTP/1.1 200 OK");
//   client.println("Content-Type: text/html");
//   client.println(""); //  do not forget this one
//   client.println("<!DOCTYPE HTML>");
//   client.println("<html>");
//
//   client.println("Wemos Relay v0.1");
//   client.println("<br><br>");
//
//   for (byte i = 0; i < Relay_Number_Of; i++) {
//     client.print("Relay " + String(i + 1) + ": ");
//     if (Relay_State[i] == LOW) client.print("OFF");
//     else client.print("ON");
//     client.println("<br>");
//   }
//
//   client.println("</html>");
//
// } // WebServer()


// ---------------------------------------- IPtoString() ----------------------------------------
String IPtoString(IPAddress IP_Address) {

  String Temp_String = String(IP_Address[0]) + "." + String(IP_Address[1]) + "." + String(IP_Address[2]) + "." + String(IP_Address[3]);

  return Temp_String;

} // IPtoString


// ---------------------------------------- MQTT_Connect() ----------------------------------------
void MQTT_Connect() {

  if (MQTT_Client.connected()) return;

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi down!");
    return;
  }

  Serial.print("Connecting to MQTT Broker: " + IPtoString(MQTT_Broker) + " ... ");
  Serial.println(MQTT_Client.connect(String(WiFi_Hostname + String(random(0xffff), HEX)).c_str(), MQTT_Username, MQTT_Password) ? "OK" : "FAILED");

  Serial.print("Subscribing to MQTT Topic: " + String(MQTT_Subscribe_Topic) + " ... ");
  Serial.println(MQTT_Client.subscribe(MQTT_Subscribe_Topic, 0) ? "OK" : "FAILED");

} // MQTT_Connect


// ---------------------------------------- MQTT_Send() ----------------------------------------
void MQTT_Send(String Topic, String Message) {

  MQTT_Connect();

  Serial.print("MQTT Publish - Topic: " + Topic + " Message: " + Message + " ... ");
  Serial.println(MQTT_Client.publish(Topic.c_str(), Message.c_str()) ? "OK" : "FAILED");

} // MQTT_Send


// ---------------------------------------- MQTT_KeepAlive() ----------------------------------------
void MQTT_KeepAlive() {
  MQTT_Client.loop();

  if (MQTT_KeepAlive_At < millis()) {
    MQTT_Send("/Boat/KeepAlive/" + WiFi_Hostname, "Uptime in milliseconds: " + String(millis()));
    MQTT_KeepAlive_At = millis() + MQTT_KeepAlive_Delay;
  }
}


// ---------------------------------------- callback() ----------------------------------------
void MQTT_Callback(char* topic, byte* payload, unsigned int length) {

  String Payload;
  for (int i = 0; i < length; i++) {
    Payload += (char)payload[i];
  }

  byte Relay_Number = Payload.substring(0, Payload.indexOf("-")).toInt();
  bool State = Payload.substring(Payload.indexOf("-") + 1, Payload.length()).toInt();

  if (Relay_Number <= Relay_Number_Of && digitalRead(Relay_Number - 1) != State) {
    digitalWrite(Relay_Number - 1, State);
    Serial.println("Relay " + String(Relay_Number) + " changed state to: " + String(digitalRead(Relay_Number - 1)));
  }

} // callback()


void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Booting");

  // ------------------------------ Pins ------------------------------
  Serial.println("Configuring pins");
  for (byte i = 0; i < 6; i++) {
    pinMode(Relay_Pin[i], OUTPUT);
    digitalWrite(Relay_Pin[i], !Relay_On_State);
  }

  // ------------------------------ WiFi ------------------------------
  Serial.print("WiFi SSID: ");
  Serial.println(WiFi_SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WiFi_SSID, WiFi_Password);
  WiFi.hostname(WiFi_Hostname);

  unsigned long Connection_Timeout = millis() + 5000;

  Serial.print("Connecting to WiFi ");

  while (WiFi.status() != WL_CONNECTED) {
    if (Connection_Timeout > millis()) {
      Serial.print(".");
    }
    else {
      Serial.println(" TOIMEOUT");
      break;
    }
    delay(250);
  }

  if (WiFi.status() == WL_CONNECTED) Serial.println(" OK");



  // ------------------------------ MQTT ------------------------------
  MQTT_Client.setServer(MQTT_Broker,1883);
  MQTT_Client.setCallback(MQTT_Callback);


  // ------------------------------ Boot End ------------------------------
  MQTT_Send("/Boat/System/" + WiFi_Hostname, "Booting");
  Serial.println("Boot done");
}


void loop() {

  MQTT_KeepAlive();

} // loop()
