////////////////////////////////KOL SAATİ BU OLACAK YANİ VERİLERİ BU GÖNDERECEK /////////////////////

///////Kütüphaneler////////
#include <SPI.h>         // Lora için kütüphane
#include <LoRa.h>        // Lora için kütüphane
#include "SSD1306Wire.h" // OLED ekran için kütüphane

///////OLED TANIMLAMA////////
#define I2C_SDA 21                           // OLED PIN
#define I2C_SCL 22                           // OLED PIN
SSD1306Wire display(0x3c, I2C_SDA, I2C_SCL, GEOMETRY_128_64, I2C_ONE, -1); // Ekran Tanımlama

///////LORA TANIMLAMA////////
#define SCK 5
#define MISO 19
#define MOSI 27
#define RADIO_CS_PIN 18
#define RADIO_DI0_PIN 26
#define RADIO_RST_PIN 14
#define BAND 866E6 // GEÇERLİ FREKANS ARALIĞI

// LORA İLE DATA GÖNDERME OBJESi
struct Data
{
    double lat;
    double lng;
    char z[10];
} data;

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

/////////////////Lora - OLED - GPS ÇALIŞTIIRMAK İÇ GEREKLİ İŞLMELER/////////////
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

    int packetSize = LoRa.parsePacket();
    if (packetSize)
    {
        // received a packet
        Serial.print("\nReceived packet size ");
        Serial.print(packetSize);
        Serial.print(" data ");
        // read packet
        while (LoRa.available())
            for (int i = 0; i < packetSize; i++)
            {
                ((byte *)&data)[i] = LoRa.read();
                Serial.print(' ');
                Serial.print(((byte *)&data)[i]);
            }
        // print RSSI of packet
        Serial.print("' with RSSI ");
        Serial.println(LoRa.packetRssi());
        Serial.print("lat = ");
        Serial.print(data.lat, 6);
        Serial.print(" lng = ");
        Serial.print(data.lng, 6);
        Serial.print(" z = ");
        Serial.print(data.z);

        display.drawString(0, 0, String(data.lat));
        display.drawString(0, 18, String(data.lng));

        display.display();
    }
 
}

/// OLED için gerekli bileşenleri barındırır////////////
void setOLED()
{
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_16);
}