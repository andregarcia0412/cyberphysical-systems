#include "fileWrite.hpp"
#include "Arduino.h"

void fileWrite(String data)
{
    File file = SPIFFS.open("/testdata.csv", FILE_APPEND);

    if (!file)
    {
        Serial.println("Erro ao abrir arquivo");
        return;
    }

    file.print(data);
    file.close();

    Serial.println("Arquivo escrito com sucesso");
}