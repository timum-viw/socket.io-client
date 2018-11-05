#include <WiFi.h>
#include <SocketIoClient.h>

/////////////////////////////////////
////// USER DEFINED VARIABLES //////
///////////////////////////////////
/// WIFI Settings ///
const char* ssid     = "yourSSID";
const char* password = "yourWiFiPassword";

/// Socket.IO Settings ///
char host[] = "192.168.1.2"; // Socket.IO Server Address
int port = 5000; // Socket.IO Port Address
char path[] = "/socket.io/?transport=websocket"; // Socket.IO Base Path
bool useSSL = false; // Use SSL Authentication
const char * sslFingerprint = "";  // SSL Certificate Fingerprint
bool useAuth = false; // use Socket.IO Authentication
const char * serverUsername = "socketIOUsername";
const char * serverPassword = "socketIOPassword";

/// Pin Settings ///
int LEDPin = 2;
int buttonPin = 0;


/////////////////////////////////////
////// ESP32 Socket.IO Client //////
///////////////////////////////////

SocketIoClient webSocket;
WiFiClient client;

bool LEDState = false;


void socket_Connected(const char * payload, size_t length) {
  Serial.println("Socket.IO Connected!");
}

void socket_statusCheck(const char * payload, size_t length) {
  char* message = "\"OFF\"";
  if (!LEDState) {
    message = "\"ON\"";
  }
  webSocket.emit("status", message);
}

void socket_event(const char * payload, size_t length) {
  Serial.print("got message: ");
  Serial.println(payload);
}

void socket_pushButton(const char * payload, size_t length) {
  LEDStateChange(!LEDState);
}

void LEDStateChange(const bool newState) {
  char* message = "\"OFF\"";
  if (!newState) {
    message = "\"ON\"";
  } 
  webSocket.emit("state_change", message);
  LEDState = newState;
  Serial.print("LED state has changed: ");
  Serial.println(message);
}

void checkLEDState() {
  digitalWrite(LEDPin, LEDState);
  const bool newState = digitalRead(buttonPin); // See if button is physically pushed
  if (!newState) {
    LEDStateChange(!LEDState);
    delay(500);
  }
}

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(LEDPin, OUTPUT);
  pinMode(buttonPin, INPUT);

  // We start by connecting to a WiFi network

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
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Setup 'on' listen events
  webSocket.on("connect", socket_Connected);
  webSocket.on("event", socket_event);
  webSocket.on("status", socket_statusCheck);
  webSocket.on("state_change_request", socket_pushButton);

  // Setup Connection
  if (useSSL) {
    webSocket.beginSSL(host, port, path, sslFingerprint);
  } else {
    webSocket.begin(host, port, path);
  }
  
  // Handle Authentication
  if (useAuth) {
    webSocket.setAuthorization(serverUsername, serverPassword);
  }
}

void loop() {
  webSocket.loop();
  checkLEDState();
}
