#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define RX 16
#define TX 17

const char *ssid = "Why not?";
const char *password = "Jgb19091";
const char *mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
const char *mqttUser = "";
const char *mqttPassword = "";
const char *topico = "gilson";
// const char* topico_temp = "cps/m3ef/temp";

WiFiClient espClient;
PubSubClient client(espClient);
char mensagem[30];

void reconectabroker();
void callback(char *, byte *, unsigned int);
void conectarWifi();

void setup()
{
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RX, TX);

  conectarWifi();
  client.setCallback(callback);
}

void loop()
{
  if (!client.connected())
  {
    reconectabroker();
  }

  if (Serial2.available())
  {
    String msg = Serial2.readStringUntil('\n');
    Serial.println("Vim do RX/TX: " + msg);
    client.publish(topico, msg.c_str());
  }

  client.loop();
}

void reconectabroker()
{
  client.setServer(mqttServer, mqttPort);
  while (!client.connected())
  {
    Serial.println("Conectando ao broker MQTT...");
    if (client.connect("", mqttUser, mqttPassword))
    {
      Serial.println("Conectado ao broker!");
      client.subscribe(topico);
    }
    else
    {
      Serial.print("Falha na conexao ao broker - Estado: ");
      Serial.print(client.state());
      delay(4000);
    }
  }
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.println("Chegou mensagem no topico: ");
  Serial.print(topic);
  Serial.print(". Mensagem: ");
  String messageTemp;

  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
    messageTemp += (char)payload[i];
  }
  Serial.println();

  if (String(topic) == topico)
  {
    Serial.println(messageTemp);
  }
}

void conectarWifi()
{
  WiFi.begin(ssid, password, 6);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Iniciando conexao com a rede WiFi..");
  }

  Serial.println("Conectado na rede WiFi!");
}
