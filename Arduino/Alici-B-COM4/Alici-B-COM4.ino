////////////////////////////////KOL SAATİ BU OLACAK YANİ VERİLERİ BU GÖNDERECEK /////////////////////

///////Kütüphaneler////////
#include <SPI.h>         // Lora için kütüphane
#include <LoRa.h>        // Lora için kütüphane
#include "SSD1306Wire.h" // OLED ekran için kütüphane

///////OLED TANIMLAMA////////
#define I2C_SDA 21                           // OLED PIN
#define I2C_SCL 22                           // OLED PIN
SSD1306Wire display(0x3c, I2C_SDA, I2C_SCL); // Ekran Tanımlama

///////LORA TANIMLAMA////////
#define SCK 5
#define MISO 19
#define MOSI 27
#define RADIO_CS_PIN 18
#define RADIO_DI0_PIN 26
#define RADIO_RST_PIN 14
#define BAND 866E6 // GEÇERLİ FREKANS ARALIĞI

void setup()
{
    Serial.begin(115200);
    initOLED();
    initLora();
}

void loop()
{
    receiveDataLora();
}

/////////////////Lora - OLED - GPS ÇALIŞTIIRMAK İÇ GEREKLİ İŞLMELER (SETUP)/////////////
void initLora()
{ // LORA TANIMLANDI BAŞLAMASI İÇİN

    SPI.begin(SCK, MISO, MOSI, RADIO_CS_PIN);
    LoRa.setPins(RADIO_CS_PIN, RADIO_RST_PIN, RADIO_DI0_PIN); // PIN'LER TANIMLANDI

    if (!LoRa.begin(BAND))
    { // FREKANS ARALIĞINDA ÇALIŞMADIĞI DURUMDA HATA VERİYOR
        Serial.println("Starting LoRa failed!");
        while (1)
            ;
    }
    Serial.println("LoRa Initializing OK!");
}

void initOLED()
{ // OLED EKRAN TANIMLANDI BAŞLAMASI İÇİN
    display.init();
    display.flipScreenVertically();
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
}

/////// Veri göndermek için Lora kullanıp veri gönderme //////////
void receiveDataLora()
{
    setOLED();
    // try to parse packet
    int packetSize = LoRa.parsePacket();
    if (packetSize)
    {
        // received a packet
        Serial.print("Received packet ");

        // read packet
        while (LoRa.available())
        {
            double lat = LoRa.read();
            Serial.print(lat, 6);
            double lng = LoRa.read();
            Serial.print(lng, 6);
        }

        //display.drawString(0, 0, String(lat));
        //display.drawString(0, 18, String(lng));

        display.display();
    }
    else{
        Serial.print("Received packet ");

    }
}

/// OLED için gerekli bileşenleri barındırır////////////
void setOLED()
{
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_16);
}
