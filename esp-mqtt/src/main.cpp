#include <WiFi.h>
#include <PubSubClient.h>

const char *ssid = "Andre";                   // string com nome da rede onde o esp vai se conectar
const char *password = "andre123";            // string com a senha da rede. Deixar em branco caso a rede seja aberta
const char *mqttServer = "broker.hivemq.com"; // string com a url do Broker que vai receber e transmitir as mensagens
const int mqttPort = 1883;                    // inteiro com o número da porta utilizada na conexão
const char *mqttUser = "";                    // string com nome de usuário no Broker, quando for necessário
const char *mqttPassword = "";                // string com senha do Broker, quando necessário
const char *topico_led = "cps/m6ab/led";      // tópico a ser assinado
// const char* topico_temp = "cps/m3ef/temp";         //tópico para publicação

WiFiClient espClient;           // criação do objeto espClient do tipo WiFiClient
PubSubClient client(espClient); //

char mensagem[30];    // vetor para mensagem
const int ledPin = 2; // define a váriavel led com o pino
const int btnOn = 25;
const int btnOff = 12;

void reconectabroker();
void callback(char *, byte *, unsigned int);

void setup()
{
  pinMode(ledPin, OUTPUT); // define o pino do led como saída
  pinMode(btnOn, INPUT_PULLUP);
  pinMode(btnOff, INPUT_PULLUP);
  Serial.begin(115200); // define o baudrate da comunicação serial como 115200 kbps

  WiFi.begin(ssid, password, 6); // inicia a conexão WiFi com a rede

  while (WiFi.status() != WL_CONNECTED) // verifica se a conexão ainda não estabilizou
  {
    delay(500);                                            // espera 500 ms
    Serial.println("Iniciando conexao com a rede WiFi.."); //"printa" na tela a frase entre aspas
  }

  Serial.println("Conectado na rede WiFi!"); // quando sai do laço, print a frase entre aspas
  client.setCallback(callback);              // configura o Callback do cliente a partir do callback passado como parâmetro
}

void loop() // laço principal
{
  // Conecta com o broker, caso não esteja conectado.
  if (!client.connected())
  {
    reconectabroker(); // chama função de conexão ao Broker
  }
  // Para checar a rotina de callback
  client.loop();

  // Envia a mensagem ao broker
  if (!digitalRead(btnOn))
  {

    client.publish(topico_led, "on");
    Serial.println("Ligando led");
    delay(10);
  }
  if (!digitalRead(btnOff))
  {
    client.publish(topico_led, "off");
    Serial.println("Desligando led");
    delay(10);
  }

  delay(20);
}

void reconectabroker()
{
  // Conexao ao broker MQTT
  client.setServer(mqttServer, mqttPort);
  while (!client.connected())
  {
    Serial.println("Conectando ao broker MQTT...");
    if (client.connect("", mqttUser, mqttPassword))
    {
      Serial.println("Conectado ao broker!");
      client.subscribe(topico_led); // assina o esp32 no tópico topico_teste definido no inicio do programa
    }
    else
    {
      Serial.print("Falha na conexao ao broker - Estado: ");
      Serial.print(client.state());
      delay(4000);
    }
  }
} // definição da funçao de callback.
void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.println("Chegou mensagem no topico: ");
  Serial.print(topic);
  Serial.print(". Mensagem: ");
  String messageTemp;

  for (int i = 0; i < length; i++)
  { // escreve mensagem caractere por caractere
    Serial.print((char)payload[i]);
    messageTemp += (char)payload[i];
  }
  Serial.println();

  if (String(topic) == topico_led)
  {
    messageTemp.toLowerCase();
    if (messageTemp == "on")
    {
      Serial.println("Led Ligado");
      digitalWrite(ledPin, HIGH);
    }
    else if (messageTemp == "off")
    {
      Serial.println(" Led Desligado");
      digitalWrite(ledPin, LOW);
    }
    else
    {
    }
  }
}
