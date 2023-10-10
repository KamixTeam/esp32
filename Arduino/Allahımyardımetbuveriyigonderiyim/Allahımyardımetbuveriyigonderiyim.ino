#include <SPI.h>
#include <LoRa.h>

struct Data {
  double x;
  double y;
  char z[10];
} data;


void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Receiver");
  LoRa.setPins(18, 14, 26); // for Lora 32u4
  if (!LoRa.begin(866E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("\nReceived packet size ");
    Serial.print(packetSize);
    Serial.print(" data ");
    // read packet
    while (LoRa.available())
      for (int i = 0; i < packetSize; i++) {
        ((byte *) &data)[i] = LoRa.read();
        Serial.print(' ');
        Serial.print(((byte *) &data)[i]);
      }
    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
    Serial.print("x = "); Serial.print(data.x, 6);
    Serial.print(" y = "); Serial.print(data.y, 6);
    Serial.print(" z = "); Serial.print(data.z);

  }
}