#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include "fileWrite.hpp"
#include "fileRead.hpp"
#include "SPIFFS.h"

uint8_t broadcastAddress[] = {0xA0, 0xB7, 0x65, 0x28, 0x62, 0xD0};

struct TestData
{
  char id[33];
  char message[100];
};

TestData data;

#define BUTTON 5
#define TEST_AMOUNT 1000
#define TEST_DELAY_MS 50

int okQtt = 0, errQtt = 0;
int id = 1;
char buffer[33];
bool justExecutedTests = false;

void resetTestData()
{
  id = 1;
  okQtt = 0;
  errQtt = 0;
  justExecutedTests = false;
}

void onDataSend(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  Serial.println("Status do envio: ");
  if (status == ESP_NOW_SEND_SUCCESS)
  {
    Serial.println("OK");
    okQtt++;
  }
  else
  {
    Serial.println("Falha no envio");
    errQtt++;
  }
}

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  Serial.println("Endereço MAC: " + WiFi.macAddress());

  pinMode(BUTTON, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

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

  if (!SPIFFS.begin(true))
  {
    Serial.println("Erro ao montar SPIFFS");
    return;
  }

  if (Serial)
  {
    Serial.print("Handshake de testes offline: ");
    fileRead();
  }
}

void loop()
{
  while (digitalRead(BUTTON) == HIGH)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(50);
  }

  digitalWrite(LED_BUILTIN, LOW);

  for (int i = 0; i < TEST_AMOUNT; i++)
  {
    strcpy(data.id, itoa(id, buffer, 10));
    strcpy(data.message, "mensagem bem legal");

    esp_err_t result = esp_now_send(
        broadcastAddress,
        (uint8_t *)&data,
        sizeof(data));

    if (result == ESP_OK)
    {
      Serial.println("Enviado com sucesso");
    }
    else
    {
      Serial.println("Erro ao enviar");
    }

    id++;
    justExecutedTests = true;

    delay(TEST_DELAY_MS);
  }

  if (justExecutedTests)
  {
    Serial.println("Testes finalizados");
    Serial.println("Envios concluídos: " + String(okQtt));
    Serial.println("Falhas de envio: " + String(errQtt));

    float seconds = (TEST_AMOUNT * TEST_DELAY_MS) / 1000.0;
    float commRateSeconds = (okQtt + errQtt) / seconds;

    Serial.println("Taxa de envios por segundo: " + String(commRateSeconds));
    Serial.println("Tempo total: " + String(seconds) + "s");

    fileWrite(String(okQtt) + "," + String(errQtt) + "," + String(commRateSeconds) + "," + String(seconds));

    resetTestData();
  }
}
