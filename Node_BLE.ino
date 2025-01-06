#include <WiFi.h>
#include <WiFiClient.h>
#include <BLEDevice.h>
#include <BLEBeacon.h>

#define SERVICE_UUID "12345678-1234-5678-1234-56789abcdef0"
#define CHARACTERISTIC_UUID "abcdef01-1234-5678-1234-56789abcdef0"

const char *ssid = "Velocity_DarkWeb";
const char *password = "Electrokage1342";
const char *serverUrl = "http://localhost:3000/api/notifications/place/";

String placeUUID = "12345678-1234-5678-1234-56789abcdef0";  // UUID del lugar

void setup() {
  Serial.begin(9600);

  // Conectar al WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  Serial.println("Conectado a WiFi");

  // Inicia el dispositivo BLE Beacon
  BLEDevice::init("");
  BLEBeacon beacon;
  beacon.setManufacturerId(0x4C00);
  beacon.setProximityUUID(SERVICE_UUID);

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinInterval(400);
  pAdvertising->setMaxInterval(500);
  pAdvertising->start();
}

void getCurrentPlaceInfo() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    if (client.connect(serverUrl, 80)) {
      String url = String("/api/notifications/place/") + placeUUID;
      client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + serverUrl + "\r\n" + "Connection: close\r\n\r\n");
      while (client.connected() || client.available()) {
        if (client.available()) {
          String response = client.readStringUntil('\n');
          Serial.println(response);
        }
      }
    }
    client.stop();
  }
}

void loop() {
  getCurrentPlaceInfo();
  delay(2000);
}
