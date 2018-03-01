
#include <Arduino.h>
#include <ESP8266WiFi.h>

// Wemos Relay

const char* ssid = "NoInternetHere";
// const char* ssid = "NoInternetHereEither";
const char* password = "NoPassword1!";

const int Relay_Pin[] {5, 4, 14, 12, 13, 15};
/*
D3 = Not used
D4 =  Not used (BUILTIN_LED)
D0 = 16 -

3 - D1 = 5
4 - D2 = 4

5 - D5 = 14
6 - D6 = 12
7 - D7 = 13
8 - D8 = 15
*/

bool Relay_State[6];




byte WiFi_Connected = false;

WiFiServer server(80);


void Start_Web_Server() {
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.print("Use this URL : ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}


void WiFi_Connect() {

  // Connect to WiFi network
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




} // WiFi_Connect


void WiFi_Check() {

  if (WiFi.status() == 3 & WiFi_Connected != true) {

    WiFi_Connected = true;

  }


  if (WiFi.status() != 3) {

    Serial.print("WiFi.status(): ");
    Serial.println(WiFi.status());

     WiFi_Connected = false;

  }

  if (WiFi_Connected == false) {
    WiFi_Connect();
    Start_Web_Server();
  }

}


void setup() {





  Serial.begin(115200);
  delay(10);
  Serial.println();

  pinMode(BUILTIN_LED, OUTPUT);

  for (byte i = 0; i < 6; i++) {
    pinMode(Relay_Pin[i], OUTPUT);
    digitalWrite(Relay_Pin[i], 0);
  }



}

void loop() {

  WiFi_Check();




  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  while(!client.available()){
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // Match the request

  // remove "/light" and set light based on % number in get requist

  if (request.indexOf("GET /Relay_") != -1) {

    request.replace("GET /Relay_", "");
    request.replace(" HTTP/1.1", "");

    byte Selected_Relay = request.substring(0, request.indexOf("-")).toInt();

    byte Selected_State = request.substring(request.indexOf("-") + 1, request.length()).toInt();

    if (Selected_Relay == 99) {
      for (byte i = 0; i < 6; i++) {
        Relay_State[i] = 0;
        digitalWrite(Relay_Pin[i], 0);
      }
      Serial.println("All OFF");
    }

    else {
      Selected_Relay = Selected_Relay - 1; // Done to compensate for the array number beaingg -1 in relation to relay number

      Relay_State[Selected_Relay] = Selected_State;
      digitalWrite(Relay_Pin[Selected_Relay], Relay_State[Selected_Relay]);

      Serial.print("Relay " + String(Selected_Relay + 1) + ": ");
      if (Relay_State[Selected_Relay] == 0) Serial.println("OFF");
      else Serial.println("ON");

    }


  }


  else Serial.println(request);

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");

  client.println("Wemos Relay v0.1");
  client.println("<br><br>");

  for (byte i = 0; i < 6; i++) {
    client.print("Relay " + String(i + 1) + ": ");
    if (Relay_State[i] == LOW) client.print("OFF");
    else client.print("ON");
    client.println("<br>");
  }


  client.println("</html>");

  delay(1);

}
