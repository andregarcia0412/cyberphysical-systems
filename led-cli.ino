#include <Arduino.h>

String input = "";
int delayVal = 200;

#define LED_BUILTIN 2

void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  if (Serial.available() > 0)
  {
    input = Serial.readStringUntil('\n');
  }

  if (input.equals("led+on"))
  {
    digitalWrite(LED_BUILTIN, HIGH);
  }

  if (input.equals("led+blink"))
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(delayVal);
    digitalWrite(LED_BUILTIN, LOW);
    delay(delayVal);
  }

  if (input.equals("led+off"))
  {
    Serial.println(input);
    digitalWrite(LED_BUILTIN, LOW);
  }

  if (input.startsWith("delay+"))
  {
    Serial.println(input);
    Serial.println(input.toInt());
    delayVal = input.toInt();
  }

  if (input.startsWith("blink+"))
  {
    for (int i = 0; i < input.toInt(); i++)
    {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(delayVal);
      digitalWrite(LED_BUILTIN, LOW);
      delay(delayVal);
    }
  }
}
