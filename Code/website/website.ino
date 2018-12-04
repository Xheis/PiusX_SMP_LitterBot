#include <string.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "website.h"


/* Set these to your desired credentials. */
const char *ssid = "LitterBot";
const char *password = "LitterBot";


//These are our dc motor pins

#define PinPWMA D1
#define PinPWMB D2
#define PinDirA1 D4
#define PinDirA2 D6
#define PinDirB1 D3
#define PinDirB2 D5


ESP8266WebServer server(80);

// Setup is where we "set up" our program. We start the website, send out the wifi, and get ready to accept button clicks!
void setup() 
{
  delay(1000);
  ESP.eraseConfig();
  delay(1000);
  initMotors();
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.on("/forward", HTTP_POST, Forward);  // If we go to 192.168.4.1/forward, we'll run the handleForward() code
  server.on("/back", HTTP_POST, Back);
  server.on("/left", HTTP_POST, Left);
  server.on("/right", HTTP_POST, Right);
  server.begin();
  Serial.println("HTTP server started");
}


//This is our loop. This is what we'll do forever until we turn the controller off!
void loop() {
  server.handleClient();
}

  
/* Just a little test message.  Go to http://192.168.4.1 in a web browser
   connected to this access point to see it.
*/
void handleRoot() { 
 String s = ourWebsite; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
//server.send(200, "text/html", "<form action=\"/forward\" method=\"POST\"><input type=\"submit\" style=\"background-color: #c14547;\" value=\"\&#x2191\"></form>");
}


void ResetButton()
{
  server.sendHeader("Location","/");        // Add a header to respond with a new location for the browser to go to the home page again
  server.send(303);                         // Send it back to the browser with an HTTP status 303 (See Other) to redirect
}


void Forward()
{
  SetDirection("forward");
  StartMotors(100,1.5);
  ResetButton();
}

void Back()
{
  SetDirection("back");
  StartMotors(50,1.5);
  ResetButton();
}

void Left()
{
  SetDirection("left");
  StartMotors(50,1.5);
  ResetButton();
}

void Right()
{
  SetDirection("right");
  StartMotors(50,1.5);
  ResetButton();
}

void StartMotors(int speed, float timeInSeconds)
{
  if (speed < 0)
  {
    speed = 0;
  }
  if (speed > 100)
  {
    speed = 1023;
  }
  int PWMspeed = speed*(1023.0/100.0);
  int delayTime = timeInSeconds*1000;
  Serial.println(PWMspeed);
  Serial.println(timeInSeconds);
  analogWrite(PinPWMA, PWMspeed);
  analogWrite(PinPWMB, PWMspeed);
  delay(delayTime);
  analogWrite(PinPWMA, 0);
  analogWrite(PinPWMB, 0);
}

void initMotors()
{
  //Tells our arduino that our 2 PWM pins & 4 Direction pins will be OUTPUTING a signal, not RECIEVING a signal
  pinMode(PinPWMA, OUTPUT); 
  pinMode(PinPWMB, OUTPUT);
  pinMode(PinDirA1, OUTPUT); 
  pinMode(PinDirA2, OUTPUT); 
  pinMode(PinDirB1, OUTPUT);
  pinMode(PinDirB2, OUTPUT); 

//set speeds to 0
  digitalWrite(PinPWMA, LOW); 
  digitalWrite(PinPWMB, LOW);


//set directions
  SetDirection("forward"); //set our direction so we're ready to go!
}



void SetDirection(char* direction)
{

  if (direction == "forward")
  {
     LeftMotorMode("clockwise");
     RightMotorMode("anticlockwise");
  }
  else if (direction == "back")
  {
     LeftMotorMode("anticlockwise");
     RightMotorMode("clockwise");
  }
  else if (direction == "left")
  {
     LeftMotorMode("clockwise");
     RightMotorMode("clockwise");
  }
  else if (direction == "right")
  {
     LeftMotorMode("anticlockwise");
     RightMotorMode("anticlockwise");
  }
}

// LeftMotorMode(mode) is our function that will make our left motor turn clockwise, anticlockwise or stop all together.
// It does this by taking what we tell it, e.g. LeftMotorMode("clockwise"), and it 
/*
 * Truth table from the manufacturer
          A1  A2
          ______
STOP    |  0   0
FORWARD |  1   0
REVERSE |  0   1

*/
void LeftMotorMode(char* mode)
{
    if ( mode == "clockwise") //clockwise is "FORWARD", so we need A1 = 1, A2 = 0
     {
        digitalWrite(PinDirA1, HIGH); //A1 = 1
        digitalWrite(PinDirA2, LOW);  //A2 = 0
     } 
     if (mode == "anticlockwise") //anticlockwise is "REVERSE", so we need A1 = 0, A2 = 1
     {
        digitalWrite(PinDirA1, LOW);  //A1 = 0
        digitalWrite(PinDirA2, HIGH); //A2 = 1
     }
     else if (mode == "stop")
     {
        digitalWrite(PinDirA1, LOW); //A1 = 0
        digitalWrite(PinDirA2, LOW); //A2 = 0
     }
}

/// Same as above, but for B
void RightMotorMode(char* mode)
{
    if ( mode == "clockwise")
     {
        digitalWrite(PinDirB1, LOW); 
        digitalWrite(PinDirB2, HIGH); 
     } 
     else if (mode == "anticlockwise")
     {
        digitalWrite(PinDirB1, HIGH); 
        digitalWrite(PinDirB2, LOW); 
     }
     else if (mode == "stop")
     {
        digitalWrite(PinDirB1, LOW); 
        digitalWrite(PinDirB2, LOW); 
     }
}
