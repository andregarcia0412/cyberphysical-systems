#include <WiFi.h>
#include <PubSubClient.h>

#define RX 16
#define TX 17

const char *ssid = "iPhone de Rafael";
const char *password = "Jorfraud06";
const char *mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
const char *mqttUser = "";
const char *mqttPassword = "";
const char *topico = "cps/equipe1/sensor/dht11";

WiFiClient espClient;
PubSubClient client(espClient);

char mensagem[128];

void reconectabroker();
void callback(char *, byte *, unsigned int);

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RX, TX);

  WiFi.begin(ssid, password, 6);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Iniciando conexao com a rede WiFi..");
  }

  Serial.println("Conectado na rede WiFi!");
  client.setCallback(callback);
}

void loop()
{
  if (Serial2.available())
  {
    String message = Serial2.readStringUntil('\n');

    int separator = message.indexOf(",");

    String temp = message.substring(0, separator);
    String hum = message.substring(separator + 1);
    Serial.print("Temperatura: ");
    Serial.println(temp);

    Serial.print("Umidade: ");
    Serial.println(hum);
  }

  if (!client.connected())
  {
    reconectabroker();
  }
  client.loop();

  delay(20);
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
    messageTemp.toLowerCase();
    if (messageTemp == "on")
    {
      Serial.println("Led Ligado");
      digitalWrite(LED_BUILTIN, HIGH);
    }
    else if (messageTemp == "off")
    {
      Serial.println(" Led Desligado");
      digitalWrite(LED_BUILTIN, LOW);
    }
    else
    {
    }
  }
}