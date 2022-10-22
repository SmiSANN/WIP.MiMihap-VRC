#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <WebSocketsServer.h>

const int PWM_Hz = 5000;
const int PWM_level = 8;
const int led0 = 22;
const int led1 = 23;

int power0 = 0;
int power1 = 0;

static WebSocketsServer webSocket(81);

void handleRoot() {

  void handleNotFound() {
  }
  void setup(void) {

    pinMode(led0, OUTPUT);
    ledcSetup(0, PWM_Hz, PWM_level);
    ledcAttachPin(led0, 0);
    ledcWrite(0, 0);

    pinMode(led1, OUTPUT);
    ledcSetup(1, PWM_Hz, PWM_level);
    ledcAttachPin(led1, 1);
    ledcWrite(1, 0);


    Serial.begin(115200);

    WiFiManager wifiManager;
    WiFi.disconnect(true);
    wifiManager.autoConnect("AutoConnectAP");

    Serial.println("");
    Serial.print("Connected to ");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("MAC address");
    Serial.println(WiFi.macAddress());

    if (MDNS.begin("mimihp")) {
      Serial.println("MDNS responder started");
    }
  }

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  void loop(void) {
    webSocket.loop();
    ledcWrite(1, power1);
    ledcWrite(0, power0);
  }
