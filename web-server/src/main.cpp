#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

#define ssid "Andre"
#define password "andre123"
#define LED1 26
#define LED2 27

String output26State = "off";
String output27State = "off";

WebServer server(80);

void handleRoot()
{
  String html = "<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<link rel=\"icon\" href=\"data:,\">";
  html += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}";
  html += ".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px; text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}";
  html += ".button2 { background-color: #FF0000; }</style></head>";

  html += "<body><h1>ESP32 Web Server</h1>";

  html += "<p>GPIO 26 - State " + output26State + "</p>";
  if (output26State == "off")
  {
    html += "<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>";
  }
  else
  {
    html += "<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>";
  }

  html += "<p>GPIO 27 - State " + output27State + "</p>";
  if (output27State == "off")
  {
    html += "<p><a href=\"/27/on\"><button class=\"button\">ON</button></a></p>";
  }
  else
  {
    html += "<p><a href=\"/27/off\"><button class=\"button button2\">OFF</button></a></p>";
  }

  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleGPIO26On()
{
  output26State = "on";
  digitalWrite(LED1, HIGH);
  handleRoot();
}

void handleGPIO26Off()
{
  output26State = "off";
  digitalWrite(LED1, LOW);
  handleRoot();
}

void handleGPIO27On()
{
  output27State = "on";
  digitalWrite(LED2, HIGH);
  handleRoot();
}

void handleGPIO27Off()
{
  output27State = "off";
  digitalWrite(LED2, LOW);
  handleRoot();
}

void setup()
{
  Serial.begin(115200);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);

  Serial.print("Connecting to");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n Wifi Connected.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/26/on", handleGPIO26On);
  server.on("/26/off", handleGPIO26Off);
  server.on("/27/on", handleGPIO27On);
  server.on("/27/off", handleGPIO27Off);

  server.begin();
  Serial.println("HTTP Server Started");
}

void loop()
{
  server.handleClient();
}
