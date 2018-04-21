/*
    This sketch demonstrates how to set up a simple HTTP-like server.
    The server will set a GPIO pin depending on the request
      http://server_ip/gpio/0 will set the GPIO2 low,
      http://server_ip/gpio/1 will set the GPIO2 high
    server_ip is the IP address of the ESP8266 module, will be
    printed to Serial when the module is connected.
*/




////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// wifi things /////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
#include <ESP8266WiFi.h>

const char* ssid = "yourWiFiNetwork";
const char* password = "yourWiFiPassword";

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

void setupwifi(){
  Serial.begin(115200);
  delay(10);

  // prepare GPIO2
  pinMode(2, OUTPUT);
  digitalWrite(2, 0);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

int requests(){
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return -1;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  for (int i = 0; !client.available(); i++) {
    if (i>100){ // Wait 1000 miliseconds for data
      //return -2;
    }
    delay(1);
  }

  // Read url request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  // Match the request
  if (req.indexOf("/gpio/0") != -1) {
    newGoal = 0;
  } else if (req.indexOf("/gpio/1") != -1) {
    newGoal = 4096;
  } else {
    Serial.println("invalid request");
    client.stop();
    return -3;
  }

  // Set GPIO2 according to the request
  Serial.println(newGoal);


  client.flush();

  
  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO is now ";
  s += (newGoal) ? "high" : "low";
  s += "</html>\n";
 

  /*
  String s="<!doctype html><html><body onload=\"alert('AmbientLightSensor' in window);\">";
  */
  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");
  
  return newGoal;
}


////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// stepper things //////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

#define IN1  15 // blue
#define IN2  13 // pink
#define IN3  12 // yellow
#define IN4  14 // orange
#define stepsPrRev 4096
#define timeDelay 750

int Steps = 0;
boolean Direction = true;// gre
unsigned long last_time;
unsigned long currentMillis ;
int steps_left = stepsPrRev -1;
long time2;

const int phases1[] = {0, 0, 0, 0, 0, 1, 1, 1};
const int phases2[] = {0, 0, 0, 1, 1, 1, 0, 0};
const int phases3[] = {0, 1, 1, 1, 0, 0, 0, 0};
const int phases4[] = {1, 1, 0, 0, 0, 0, 0, 1};
int Phase = 0;

void setupstepper()
{
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
}

void stepper(int xw) {
    for (int x = 0; x < xw; x++) {
        switch (Steps) {
            case 0:
                digitalWrite(IN1, LOW);
                digitalWrite(IN2, LOW);
                digitalWrite(IN3, LOW);
                digitalWrite(IN4, HIGH);
                break;
            case 1:
                digitalWrite(IN1, LOW);
                digitalWrite(IN2, LOW);
                digitalWrite(IN3, HIGH);
                digitalWrite(IN4, HIGH);
                break;
            case 2:
                digitalWrite(IN1, LOW);
                digitalWrite(IN2, LOW);
                digitalWrite(IN3, HIGH);
                digitalWrite(IN4, LOW);
                break;
            case 3:
                digitalWrite(IN1, LOW);
                digitalWrite(IN2, HIGH);
                digitalWrite(IN3, HIGH);
                digitalWrite(IN4, LOW);
                break;
            case 4:
                digitalWrite(IN1, LOW);
                digitalWrite(IN2, HIGH);
                digitalWrite(IN3, LOW);
                digitalWrite(IN4, LOW);
                break;
            case 5:
                digitalWrite(IN1, HIGH);
                digitalWrite(IN2, HIGH);
                digitalWrite(IN3, LOW);
                digitalWrite(IN4, LOW);
                break;
            case 6:
                digitalWrite(IN1, HIGH);
                digitalWrite(IN2, LOW);
                digitalWrite(IN3, LOW);
                digitalWrite(IN4, LOW);
                break;
            case 7:
                digitalWrite(IN1, HIGH);
                digitalWrite(IN2, LOW);
                digitalWrite(IN3, LOW);
                digitalWrite(IN4, HIGH);
                break;
            default:
                digitalWrite(IN1, LOW);
                digitalWrite(IN2, LOW);
                digitalWrite(IN3, LOW);
                digitalWrite(IN4, LOW);
                break;
        }
        delayMicroseconds(timeDelay);
        SetDirection();
    }
}
void SetDirection() {
    if (Direction == 1) {
        Steps++;
    }
    if (Direction == 0) {
        Steps--;
    }
    if (Steps > 7) {
        Steps = 0;
    }
    if (Steps < 0) {
        Steps = 7;
    }
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// main functions //////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

int goal;
int pos; 

void setup() {
  setupstepper();
  setupwifi();
}

void loop() {
    while(goal < 0){
        goal = requests();  
    }
    while (steps_left > 0 && ) {
        currentMillis = micros();
        if (currentMillis - last_time >= 1000) {
            stepper(1);
            time2 = time2 + micros() - last_time;
            last_time = micros();
            steps_left--;
        }
        goal = requests();
    }
}


