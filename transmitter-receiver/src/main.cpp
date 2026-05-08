#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

uint8_t broadcastAddress[] = {0xEC, 0x64, 0xC9, 0x86, 0x1D, 0xE0};

typedef struct struct_message
{
  char a[10];
} struct_message;

struct_message myData;

#define BUTTON 5

bool inverte_led = false;
int valor;

void onDataSend(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  Serial.print("Status do envio: ");

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
  WiFi.mode(WIFI_STA);
  Serial.println("Endereço MAC: " + WiFi.macAddress());

  pinMode(BUTTON, INPUT);

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

void loop()
{
  valor = digitalRead(BUTTON);

  if (valor == HIGH)
  {
    while (digitalRead(BUTTON) == HIGH)
    {
      delay(50);
    }

    Serial.println("Botão Pressionado");

    inverte_led = !inverte_led;

    if (inverte_led)
    {
      strcpy(myData.a, "L_Led");
    }
    else
    {
      strcpy(myData.a, "D_Led");
    }

    esp_err_t result = esp_now_send(
        broadcastAddress,
        (uint8_t *)&myData,
        sizeof(myData));

    if (result == ESP_OK)
    {
      Serial.println("Enviado com sucesso");
    }
    else
    {
      Serial.println("Erro ao enviar");
    }
  }
}
