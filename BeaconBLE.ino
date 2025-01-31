#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

/* Definimos el UUID que se va a enviar en los datos de advertising */
char uuidToSend[] = "12345678-1234-5678-1234-56789abcdef0";

/* define el UUID que utilizará el servicio personalizado */
#define serviceID BLEUUID((uint16_t)0x1700)

bool deviceConnected = false;

void setup() {
  Serial.begin(115200);

  BLEDevice::init("MyESP32"); 
  BLEServer *MyServer = BLEDevice::createServer(); 

  BLEService *customService = MyServer->createService(serviceID);  

  //! Configuración de advertising
  BLEAdvertising *advertising = BLEDevice::getAdvertising();
  
  // Añadimos el UUID a los datos de advertising
  advertising->addServiceUUID(serviceID);  
  advertising->setScanResponse(false); 
  // Configuración recomendada para hacer no conectable
  advertising->setMinPreferred(0x06);
  advertising->setMinPreferred(0x12);   

  // UUID va como parte de los datos de advertising
  BLEAdvertisementData advertisementData;
  advertisementData.setCompleteServices(BLEUUID(uuidToSend));  // Colocamos el UUID en el advertising
  advertising->setAdvertisementData(advertisementData);

  // Iniciar el servicio y la publicidad
  customService->start();
  MyServer->getAdvertising()->start();  // Iniciar advertising

  Serial.println("Esperando que se detecte el dispositivo (sin conexión)...");
}

void loop() {
}
