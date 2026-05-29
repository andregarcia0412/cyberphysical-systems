#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <WiFi.h>
#include <esp_now.h>

#define DHTPIN 22
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

uint8_t broadcastAddress[] = {0x5C, 0x01, 0x3B, 0x73, 0x6C, 0x18};

struct DHTData
{
  char humidity[65];
  char temperature[65];
};
DHTData dhtData;
char buffer[65];

void onDataSend(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  Serial.println("Status do envio: ");
  if (status == ESP_NOW_SEND_SUCCESS)
  {
    Serial.println("OK");
  }
  else
  {
    Serial.println("Falha no envio");
  }
}

void setup()
{
  Serial.begin(115200);
  dht.begin();
  pinMode(LED_BUILTIN, OUTPUT);

  WiFi.mode(WIFI_STA);
  Serial.println("Endereço MAC: " + WiFi.macAddress());

  if (esp_now_init() != ESP_OK)
  {
    Serial.println("Erro ao inicializar ESP-NOW");
    return;
  }

  esp_now_register_send_cb(onDataSend);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);

  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    Serial.println("Erro ao adicionar peer");
    return;
  }
}

unsigned long lastTime = 0;
int counter = 0;

void loop()
{
  if (millis() - lastTime >= 1000)
  {
    lastTime = millis();
    float hum = dht.readHumidity();
    float temp = dht.readTemperature();

    Serial.print("Temperatura: ");
    Serial.println(temp);
    Serial.print("Humidade: ");
    Serial.println(hum);

    strcpy(dhtData.humidity, dtostrf(hum, 6, 2, buffer));
    strcpy(dhtData.temperature, dtostrf(temp, 6, 2, buffer));

    esp_err_t result = esp_now_send(
        broadcastAddress,
        (uint8_t *)&dhtData,
        sizeof(dhtData));

    counter++;
    if (result == ESP_OK)
    {
      Serial.println("Enviado com sucesso");

      if (counter == 5)
      {
        digitalWrite(LED_BUILTIN, HIGH);
        counter = 0;
      }
      else
      {
        digitalWrite(LED_BUILTIN, LOW);
      }
    }
    else
    {
      Serial.println("Erro ao enviar");
    }
  }
}