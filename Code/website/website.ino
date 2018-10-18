
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "website.h"

/* Set these to your desired credentials. */
const char *ssid = "LitterBot";
const char *password = "LitterBot";


//These are our possible options for the dc motors
//new espduino 2 motors&16 servos
#define PinPWMA 14
#define PinPWMB 12
#define PinDirA 15
#define PinDirB 13

//old espduino 2 motors&16 servos
/*#define PinPWMA 3
#define PinPWMB 12
#define PinDirA 1
#define PinDirB 13*/

//nodemcu dev kit
/*#define PinPWMA 5
  #define PinPWMB 4
  #define PinDirA 0
  #define PinDirB 2*/





ESP8266WebServer server(80);
  
/* Just a little test message.  Go to http://192.168.4.1 in a web browser
   connected to this access point to see it.
*/
void handleRoot() { 
 String s = ourWebsite; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
//server.send(200, "text/html", "<form action=\"/forward\" method=\"POST\"><input type=\"submit\" style=\"background-color: #c14547;\" value=\"\&#x2191\"></form>");
}

/*
void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}*/

void setup() {
  delay(1000);
  ESP.eraseConfig();
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.on("/forward", HTTP_POST, handleForward);  // If we go to 192.168.4.1/forward, we'll run the handleForward() code
  server.on("/back", HTTP_POST, handleBack);
  server.on("/left", HTTP_POST, handleLeft);
  server.on("/right", HTTP_POST, handleRight);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

void handleForward()
{
  Forward();
  Serial.println("Forward!");
  // Tell our website to take us back to the controller homepage
  server.sendHeader("Location","/");        // Add a header to respond with a new location for the browser to go to the home page again
  server.send(303);                         // Send it back to the browser with an HTTP status 303 (See Other) to redirect
}

void handleBack()
{
  Back();
  Serial.println("Back!");
  // Tell our website to take us back to the controller homepage
  server.sendHeader("Location","/");        // Add a header to respond with a new location for the browser to go to the home page again
  server.send(303);                         // Send it back to the browser with an HTTP status 303 (See Other) to redirect
}

void handleRight()
{
  Right();
  Serial.println("Right!");
  // Tell our website to take us back to the controller homepage
  server.sendHeader("Location","/");        // Add a header to respond with a new location for the browser to go to the home page again
  server.send(303);                         // Send it back to the browser with an HTTP status 303 (See Other) to redirect
}

void handleLeft()
{
  Left();
  Serial.println("Left!");
  // Tell our website to take us back to the controller homepage
  server.sendHeader("Location","/");        // Add a header to respond with a new location for the browser to go to the home page again
  server.send(303);                         // Send it back to the browser with an HTTP status 303 (See Other) to redirect
}


void Forward()
{
  digitalWrite(PinDirA, LOW); 
  digitalWrite(PinDirB, HIGH);
  StartMotors();
}

void Back()
{
  digitalWrite(PinDirA, HIGH); 
  digitalWrite(PinDirB, LOW);
  StartMotors();
}

void Left()
{
  digitalWrite(PinDirA, LOW); 
  digitalWrite(PinDirB, LOW);
  StartMotors();
}

void Right()
{
  digitalWrite(PinDirA, HIGH); 
  digitalWrite(PinDirB, HIGH);
  StartMotors();
}

void StartMotors()
{
  analogWrite(PinPWMA, 1023);
  analogWrite(PinPWMB, 1023);
  delay(1000);
  analogWrite(PinPWMA, 0);
  analogWrite(PinPWMB, 0);
}

void initMotors()
{
  //set motors to outputs
  pinMode(PinPWMA, OUTPUT); 
  pinMode(PinPWMB, OUTPUT);

//set motor directions to outputs
  pinMode(PinDirA, OUTPUT); 
  pinMode(PinDirB, OUTPUT);

//set directions
  digitalWrite(PinDirA, HIGH); 
  digitalWrite(PinDirB, HIGH);

//set speeds to 0
  digitalWrite(PinPWMA, LOW); 
  digitalWrite(PinPWMB, LOW);
}
