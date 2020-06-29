#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#define LED 2
#define RELAY 0 //PIN D3

#define SSID "[[INSERT SSID HERE]]"
#define PASSWD "[[INSERT PASSWORD HERE]]"

#define PAGE_START "<!DOCTYPE html>\
<head>\
    <title>ESP8266 SmartSwitch</title>\
    <!--Sytlesheets-->\
    <link rel='stylesheet' type='text/css' href='https://cdnjs.cloudflare.com/ajax/libs/semantic-ui/2.4.1/semantic.min.css' />\
\
    <!--Scripts-->\
    <script src='https://cdnjs.cloudflare.com/ajax/libs/semantic-ui/2.4.1/semantic.min.js'></script>\
    <script src='https://cdnjs.cloudflare.com/ajax/libs/jquery/3.4.1/jquery.min.js'></script>\
</head>\
<body>\
    <div class='ui fluid container' style='margin-top: 10%;'>\n"

#define PAGE_ON "<h1 class='ui centered green header'>Status: ON</h1><br>\
    <a href='on' class='ui huge fluid green disabled button' style='padding: 80px;'>ON</a><br>\
    <a href='off' class='ui huge fluid red button' style='padding: 80px;'>OFF</a><br>\n"

#define PAGE_OFF "<h1 class='ui centered red header'>Status: OFF</h1><br>\
    <a href='on' class='ui huge fluid green button' style='padding: 80px;'>ON</a><br>\
    <a href='off' class='ui huge fluid red disabled button' style='padding: 80px;'>OFF</a><br>\n"

#define PAGE_END "</div>\
</body>"

ESP8266WebServer server(80);

bool ledState;

void handleRoot();
void handleOn();
void handleOff();
void handleNotFound();

void setup(){
  pinMode(LED, OUTPUT);
  pinMode(RELAY, OUTPUT);
  Serial.begin(115200);
  delay(10);

  Serial.print("Connecting to ");
  Serial.print(SSID);
  digitalWrite(LED, LOW);
  digitalWrite(RELAY, HIGH);

  WiFi.begin(SSID, PASSWD);
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  delay(500);
  digitalWrite(LED, HIGH);
  Serial.print("\nConnected with IP ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop(){
  server.handleClient(); 

  ledState = digitalRead(LED);
  if(ledState) //High means LED off
  {
    delay(1100);
  }
  else
  {
    delay(50);
  }
  //Toggle LED
  digitalWrite(LED, !ledState);
}

void handleRoot() {
  String page = PAGE_START;

  if(digitalRead(RELAY)) //HIGH Means it's off
  {
    page += PAGE_OFF; 
  }
  else
  {
    page += PAGE_ON;
  }

  page += PAGE_END;
  
  server.send(200, "text/html", page);
}

void handleOn(){
  digitalWrite(RELAY, LOW);
  delay(100);
  handleRoot();
}

void handleOff(){
  digitalWrite(RELAY, HIGH);
  delay(100);
  handleRoot();
}

void handleNotFound(){
  server.send(404, "text/plain", "404");
}
