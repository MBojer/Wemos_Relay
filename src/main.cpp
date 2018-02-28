
#include <Arduino.h>
#include <ESP8266WiFi.h>

// Wemos Relay

const char* ssid = "NoInternetHere";
// const char* ssid = "NoInternetHereEither";
const char* password = "NoPassword1!";

const int Dimmer_1 = 5;
const int Dimmer_2 = 4;
const int Dimmer_3 = 14;
const int Dimmer_4 = 12;
const int Dimmer_5 = 13;

unsigned int Light_Strength = 0;

unsigned int Light_Strength_Procent = 0;

byte Strenght_Dimmer_1 = 0;
byte Strenght_Dimmer_2 = 0;
byte Strenght_Dimmer_3 = 0;
byte Strenght_Dimmer_4 = 0;
byte Strenght_Dimmer_5 = 0;


byte Fade_Jump = 25;

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
  pinMode(Dimmer_1, OUTPUT);
  pinMode(Dimmer_2, OUTPUT);
  pinMode(Dimmer_3, OUTPUT);
  pinMode(Dimmer_4, OUTPUT);
  pinMode(Dimmer_5, OUTPUT);


  analogWrite(Dimmer_1, 0);
  analogWrite(Dimmer_2, 0);
  analogWrite(Dimmer_3, 0);
  analogWrite(Dimmer_4, 0);
  analogWrite(Dimmer_5, 0);





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

  if (request.indexOf("GET /Dimmer_") != -1) {

    request.replace("GET /", "");
    request.replace(" HTTP/1.1", "");

    // don't remove "Dimmer_" and make define statements matching dimmer_1 to something then use them to set pinf numbers, not possible it seems

    String Selected_Dimmer = request.substring(0, request.indexOf("-"));

    Light_Strength_Procent = request.substring(request.indexOf("-") + 1, request.length()).toInt();

    Light_Strength = (Light_Strength_Procent * 0.01) * 1023;
    Serial.print("Light_Strength: ");
    Serial.println(Light_Strength);

    if (Selected_Dimmer == "Dimmer_1") {
      analogWrite(Dimmer_1, Light_Strength);
      Strenght_Dimmer_1 = Light_Strength_Procent;
      Serial.print("Dimmer 1: ");
      Serial.println(Light_Strength);
    }
    else if (Selected_Dimmer == "Dimmer_2") {
      analogWrite(Dimmer_2, Light_Strength);
      Strenght_Dimmer_2 = Light_Strength_Procent;
      Serial.print("Dimmer 2: ");
      Serial.println(Light_Strength);
    }
    else if (Selected_Dimmer == "Dimmer_3") {
      analogWrite(Dimmer_3, Light_Strength);
      Strenght_Dimmer_3 = Light_Strength_Procent;
      Serial.print("Dimmer 3: ");
      Serial.println(Light_Strength);
    }
    else if (Selected_Dimmer == "Dimmer_4") {
      analogWrite(Dimmer_4, Light_Strength);
      Strenght_Dimmer_4 = Light_Strength_Procent;
      Serial.print("Dimmer 4: ");
      Serial.println(Light_Strength);
    }
    else if (Selected_Dimmer == "Dimmer_5") {
      analogWrite(Dimmer_5, Light_Strength);
      Strenght_Dimmer_5 = Light_Strength_Procent;
      Serial.print("Dimmer 5: ");
      Serial.println(Light_Strength);
    }
    else if (Selected_Dimmer == "Dimmer_99") {
      analogWrite(Dimmer_1, 0);
      Strenght_Dimmer_1 = 0;
      analogWrite(Dimmer_2, 0);
      Strenght_Dimmer_2 = 0;
      analogWrite(Dimmer_3, 0);
      Strenght_Dimmer_3 = 0;
      analogWrite(Dimmer_4, 0);
      Strenght_Dimmer_4 = 0;
      analogWrite(Dimmer_5, 0);
      Strenght_Dimmer_5 = 0;
      Serial.println("All OFF");
    }
  }


  else Serial.println(request);

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");

  client.println("Wemos Dimmer v2.0");
  client.println("<br><br>");

  client.print("Dimmer 1: ");
  client.print(Strenght_Dimmer_1);
  client.println("&#37<br>");

  client.print("Dimmer 2: ");
  client.print(Strenght_Dimmer_2);
  client.println("&#37<br>");

  client.print("Dimmer 3: ");
  client.print(Strenght_Dimmer_3);
  client.println("&#37<br>");

  client.print("Dimmer 4: ");
  client.print(Strenght_Dimmer_4);
  client.println("&#37<br>");

  client.print("Dimmer 5: ");
  client.print(Strenght_Dimmer_5);
  client.println("&#37<br>");

  client.println("<br><br>");
  client.println("<a href=\"/i2c\">i2c Test");


  //
  //
  // if (Light_Strength == 0) {
  //   client.print("0 procent");
  // }
  // else if (Light_Strength == 767) {
  //   client.print("25 procent");
  // }
  // else if (Light_Strength == 512) {
  //   client.print("50 procent");
  // }
  // else if (Light_Strength == 255) {
  //   client.print("75 procent");
  // }
  // else if (Light_Strength == 255) {
  //   client.print("100 procent");
  // }
  //
  //
  // client.println("<br><br>");
  // client.println("<a href=\"/Light=25\">Change light strenght to: 25 procent");
  // client.println("<br><br>");
  // client.println("<a href=\"/Light=50\">Change light strenght to: 50 procent");
  // client.println("<br><br>");
  // client.println("<a href=\"/Light=75\">Change light strenght to: 75 procent");
  // client.println("<br><br>");
  // client.println("<a href=\"/Light=100\">Change light strenght to: 100 procent");
  client.println("</html>");

  delay(1);

}
