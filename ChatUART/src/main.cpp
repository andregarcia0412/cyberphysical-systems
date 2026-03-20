#include <Arduino.h>
#define RX 16
#define TX 17

String message = "";

void setup()
{
  pinMode(TX, OUTPUT);
  pinMode(RX, INPUT);
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RX, TX);
}

void loop()
{
  if (Serial.available())
  {
    message = Serial.readStringUntil('\n');
    Serial2.println(message);
    Serial.println("Vocẽ: " + message);
  }

  if (Serial2.available())
  {
    Serial.println("Outro ESP32: " + Serial2.readStringUntil('\n'));
  }
}
