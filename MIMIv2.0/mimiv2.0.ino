#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager

const int PWM_Hz = 5000;
const int PWM_level = 8;
const int led0 = 22;
const int led1 = 23;

int temp = 0;
int power0 = 0;
int power1 = 0;
int counter0 = 0;
int counter1 = 0;
int onTime0 = 600;
int onTime1 = 600;
bool onoff0 = false;
bool onoff1 = false;

WebServer server(80);

void handleRoot() {
  String lengthValue0 = server.arg("LENGTH0");
  String powerValue0 = server.arg("POWER0");
  String lengthValue1 = server.arg("LENGTH1");
  String powerValue1 = server.arg("POWER1");

  if(lengthValue0 == "")
  {
    server.send(200, "text/plain", "Skip Signal");
    return;
  }
    if(lengthValue1 == "")
  {
    server.send(200, "text/plain", "Skip Signal");
    return;
  }
  
  onoff0 = true;
  onTime0 = lengthValue0.toInt();
  counter0 = 0;
  
  onoff1 = true;
  onTime1 = lengthValue1.toInt();
  counter1 = 0;
  
  int powerInt0 = powerValue0.toInt();
  powerInt0 = constrain(powerInt0, 0, 255);
  power0 = powerInt0; 
  
  int powerInt1 = powerValue1.toInt();
  powerInt1 = constrain(powerInt1, 0, 255);
  power1 = powerInt1; 
  
  String message = "";
  message += "length0: " + lengthValue0 + "\n";
  message += " power0: " + powerValue0 + "\n";
  message += "length1: " + lengthValue1 + "\n";
  message += " power1: " + powerValue1;
  server.send(200, "text/plain", message);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void) {
  
  pinMode(led0, OUTPUT);
  ledcSetup(0, PWM_Hz, PWM_level);
  ledcAttachPin(led0,0);
  ledcWrite(0, 0);

  pinMode(led1,OUTPUT);
  ledcSetup(1, PWM_Hz, PWM_level);
  ledcAttachPin(led1,1);
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

  server.on("/", handleRoot);
  
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

}

void loop(void) {
  server.handleClient();

  if(onoff0)
  {
    //digitalWrite(led0, 1);
    ledcWrite(0,power0);
    counter0++;
    if(counter0 > onTime0)
    {
      counter0 = 0;
      onoff0 = false;
    }
  }
  else
  {
    //digitalWrite(led0, 0);
    ledcWrite(0, 0);
  }
  
if(onoff1)
  {
    //digitalWrite(led1, 1);
    ledcWrite(1,power1);
    counter1++;
    if(counter1 > onTime1)
    {
      counter1 = 0;
      onoff1 = false;
    }
  }
  else
  {
    //digitalWrite(led1, 1);
    ledcWrite(1, 0);
  }
}
