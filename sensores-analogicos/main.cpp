#define LED_GREEN 32
#define LED_YELLOW 33
#define LED_RED 25
#define LED_BLUE 26
#define LIGHT 34
#define BUTTON 4

int l = 0;
bool pressed = true;

void setup()
{
  Serial.begin(115200);
  analogReadResolution(12);

  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
}

void loop()
{
  l = analogRead(LIGHT);

  if (digitalRead(BUTTON) == false)
  {
    delay(70);
    pressed = !pressed;
  }

  if (pressed)
  {
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_BLUE, HIGH);
    digitalWrite(LED_YELLOW, HIGH);
    delay(100);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_BLUE, LOW);
    digitalWrite(LED_YELLOW, LOW);
    delay(100);
  }
  else
  {
    if (l > 0)
    {
      digitalWrite(LED_RED, HIGH);
    }
    if (l > 100)
    {
      digitalWrite(LED_YELLOW, HIGH);
    }
    if (l > 200)
    {
      digitalWrite(LED_GREEN, HIGH);
    }
    if (l > 300)
    {
      digitalWrite(LED_BLUE, HIGH);
    }
  }
}
