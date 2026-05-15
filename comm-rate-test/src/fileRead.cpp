#include "fileRead.hpp"

void fileRead()
{
    File file = SPIFFS.open("/testdata.csv", FILE_READ);

    if (!file)
    {
        Serial.println("Erro ao abrir arquivo");
        return;
    }

    Serial.println("Conteúdo do arquivo");

    Serial.println(file.readString());

    file.close();
}