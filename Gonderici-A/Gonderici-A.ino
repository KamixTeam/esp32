////////////////////////////////KOL SAATİ BU OLACAK YANİ VERİLERİ BU GÖNDERECEK /////////////////////

///////Kütüphaneler////////
#define BUTTON_PIN                  38/// Buton Kütüphanesi
#define BUTTON_PIN_MASK             GPIO_SEL_38//
#include <SPI.h>            // Lora için kütüphane
#include <LoRa.h>           // Lora için kütüphane
#include "SSD1306Wire.h"    // OLED ekran için kütüphane
#include <TinyGPS++.h>      //GPS için kütüphane
#include <HardwareSerial.h> //GPS için kütüphane

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

///////GPS TANIMLAMA////////
#define RXD2 34
#define TXD2 12
#define GPS_SERIAL_NUM 1
TinyGPSPlus gps;
HardwareSerial GPSSerial(GPS_SERIAL_NUM);

void setup()
{
    Serial.begin(115200);
    initOLED();
    initLora();
    initGPS();
}

void loop()
{
    gpsdataSet();
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

void initGPS() // GPS TANIMLANMASI İÇİN
{
    GPSSerial.begin(9600, SERIAL_8N1, RXD2, TXD2);
}

///////////////////////////GPS verilerini düzenleyinp Lora ile gönderme
void gpsdataSet()
{
    setOLED(); // Oled ekranın değişmesi için gerekli olanlar
    while (GPSSerial.available() > 0)
        if (gps.encode(GPSSerial.read()))
        {
            Serial.print(F("Lokasyon: "));
            if (true)//gps.location.isValid()
            {
                double lat = 22.123456;     // lat=enlem verileri gönderebilmek için değişkene atadım
                Serial.print(gps.location.lat(), 6); // enlem
                Serial.print(F(","));
                double lng =10.123456;     // lng=boylam verileri gönderebilmek için değişkene atadım
                Serial.print(gps.location.lng(), 6); // boylam
                sendDataLora(lat, lng);              // veri göndermek için sendDataLora kullandım

                //send Display
                display.drawString(0, 0, String(lat));
                display.drawString(0, 18, String(lng));
                display.display(); // Ekranın çalışması için gerekli olanlar
            }

        }
}

/////// Veri göndermek için Lora kullanıp veri gönderme //////////
void sendDataLora(double lat, double lng)
{
    Serial.print("Sending packet: ");
    Serial.println(lat);
    Serial.println(lng);

    // send packet
    LoRa.beginPacket();
    LoRa.write(lat);
    LoRa.write(lng);
    LoRa.endPacket();
    delay(5000);
}

/// OLED için gerekli bileşenleri barındırır////////////
void setOLED()
{
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_16);
}
