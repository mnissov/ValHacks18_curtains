/*
    This sketch demonstrates how to set up a simple HTTP-like server.
    The server will set a GPIO pin depending on the request
      http://server_ip/gpio/0 will set the GPIO2 low,
      http://server_ip/gpio/1 will set the GPIO2 high
    server_ip is the IP address of the ESP8266 module, will be
    printed to Serial when the module is connected.
*/

#include <ESP8266WiFi.h>

#define IN1  15 // blue
#define IN2  13 // pink
#define IN3  12 // yellow
#define IN4  14 // orange
#define FULL_ROTATION 4096
#define timeDelay 750
#define DIST 20*FULL_ROTATION
#define LIMIT 2

const char* ssid = "yourWiFiNetwork";
const char* password = "yourWiFiPassword";
const int phases1[] = {0, 0, 0, 0, 0, 1, 1, 1};
const int phases2[] = {0, 0, 0, 1, 1, 1, 0, 0};
const int phases3[] = {0, 1, 1, 1, 0, 0, 0, 0};
const int phases4[] = {1, 1, 0, 0, 0, 0, 0, 1};
int Phase = 0;
long count = FULL_ROTATION;
long dist = 0;
bool done = 0;
int goal = -1;


////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// wifi things /////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////


// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

void setupwifi() {
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

int requests() {

    // Check if a client has connected
    WiFiClient client = server.available();
    if (!client) {
        return -1;
    }

    // Wait until the client sends some data
    Serial.println("new client");
    for (int i = 0; !client.available(); i++) {
        if (i > 100) { // Wait 1000 miliseconds for data
            //return -2;
        }
        delay(1);
    }

    // Read url request
    String req = client.readStringUntil('\r');
    Serial.println(req);
    client.flush();

    // Match the request
    int newGoal;
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

void setupstepper()
{
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}

void stepper(long temp) {
    count = temp;
    long rotationDirection = count < 1 ? -1 : 1;
    count *= rotationDirection;
    while (count > 0)
    {
        digitalWrite(IN1, phases1[Phase]);
        digitalWrite(IN2, phases2[Phase]);
        digitalWrite(IN3, phases3[Phase]);
        digitalWrite(IN4, phases4[Phase]);
        IncrementPhase(rotationDirection);
        delayMicroseconds(timeDelay);
        count--;
        dist++;
    }
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}
void IncrementPhase(int rotationDirection)
{
    Phase += rotationDirection + 8; // non negative values
    Phase %= 8;
}

void setPos(int pos) {
    switch (pos) {
        case 0: //down
            done = 0;
            stepper(dist == 0 ? DIST : dist);
            break;
        case 1: //halfway
            done = 0;
            stepper(dist == 0 ? DIST / 2 : dist);
            break;
        case 2:
            done = 0;
            while (done == 0) {
                stepper(FULL_ROTATION);
            }
            break;
    }
}

unsigned long currentPosition = 0;
unsigned long targetPosition = 1*4096;
unsigned long maximumPosition = 0;

unsigned long previousMillis = 0;
unsigned long currentMillis = 0;
unsigned long delayMillis = 1000;

void stepper2() {
  currentMillis = micros();
  if (previousMillis + delayMillis <= currentMillis) {
    if (currentPosition < targetPosition) {
      setPhase(currentPosition & 7);
      currentPosition += 1;
    } else if (currentPosition > targetPosition) {
      setPhase(currentPosition & 7);
      currentPosition -= 1;
    }
    previousMillis = currentMillis;
  }
}

void setPhase(int Phase) {
  digitalWrite(IN1, phases1[Phase]);
  digitalWrite(IN2, phases2[Phase]);
  digitalWrite(IN3, phases3[Phase]);
  digitalWrite(IN4, phases4[Phase]);
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// main functions //////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

void setup() {
    setupstepper();
    delay(1);
    //setupwifi(); //fuck watchdog
    //delay(1);
    //pinMode(LIMIT, INPUT_PULLUP);
    //attachInterrupt(digitalPinToInterrupt(LIMIT), handleInterrupt, FALLING);
}

void handleInterrupt() {
    count = 0;
    done = 1;
}

void loop() {
    /*while (goal < 0) {
        goal = requests();
        delay(1);
    }*/
    stepper2();
    //stepper(FULL_ROTATION);
    //goal = -1;
}


