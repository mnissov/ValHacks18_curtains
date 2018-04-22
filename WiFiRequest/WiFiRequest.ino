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

long requests() {

    // Check if a client has connected
    WiFiClient client = server.available();
    if (!client) {
        return -1;
    }

    // Wait until the client sends some data
    Serial.println("new client");
    for (int i = 0; !client.available(); i++) {
        if (i > 100) { // Wait 1000 miliseconds for data
            return -2;
        }
        delay(1);
    }

    // Read url request
    String req = client.readStringUntil('\r');
    Serial.println(req);
    client.flush();

    // Match the request
    long iValue;
    String sValue = "";
    int newGoal;
    String temp = "/goal=";
    int index = req.indexOf(temp)+temp.length();
    Serial.println(index);
    if (index < 0) {
        newGoal = -1;
    } else if (req[index] >= '0' && req[index] <= '9') {
        int i = 1;
        sValue += req[index];
        while (req[index + i] >= '0' && req[index + i] <= '9') {
            sValue += req[index + i];
            i++;
        }
        iValue = sValue.toInt();
        Serial.println(sValue);
        if (iValue == 0)
            return -3;
    } else {
        Serial.println("invalid request");
        client.stop();
        return -4;
    }

    // Set GPIO2 according to the request
    Serial.println(iValue);


    client.flush();


    // Prepare the response
    String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGoal is now ";
    s += sValue;


    /*
        String s="<!doctype html><html><body onload=\"alert('AmbientLightSensor' in window);\">";
    */
    // Send the response to the client
    client.print(s);
    delay(1);
    Serial.println("Client disonnected");

    return iValue;
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// main functions //////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

int goal = -1;
int pos;
#define LIMIT 2

void setup() {
    setupwifi();
}


void loop() {
    requests();
}


