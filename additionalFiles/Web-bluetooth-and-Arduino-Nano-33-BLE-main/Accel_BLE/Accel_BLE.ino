#include <ArduinoBLE.h>

BLEService whiteboardService("722cf000-6c3d-48ac-8180-64551d967680");

BLECharacteristic toDoCharacteristic("722cf001-6c3d-48ac-8180-64551d967680", BLERead | BLEWrite | BLENotify, "0000010000");

BLEBoolCharacteristic ledCharacteristic("722cf002-6c3d-48ac-8180-64551d967680", BLERead | BLEWrite);

BLEDevice central;

void setup(){

  Serial.begin(9600);

  pinMode(13,OUTPUT);

  if(!BLE.begin()){
    Serial.println("Starting BLE failed!");
    while (1);
  }

  BLE.setLocalName("WhiteBoardConfig");
  BLE.setAdvertisedService(whiteboardService);

  whiteboardService.addCharacteristic(toDoCharacteristic);
  
  whiteboardService.addCharacteristic(ledCharacteristic);

  BLE.addService(whiteboardService);

  ledCharacteristic.setValue(0);

  BLE.setEventHandler(BLEConnected, connectHandler);
  BLE.setEventHandler(BLEDisconnected, disconnectHandler);

  BLE.advertise();

  Serial.println("Comms begin");
}

unsigned long prevTime = 0;

void loop(){

  BLE.poll();

  unsigned long t = millis();

  if(central.connected()){
    if(t - prevTime > 100){
      Serial.println((char*)toDoCharacteristic.value());
      prevTime = t;
    }
  } else {
    if(t - prevTime > 100){
      central = BLE.central();
      prevTime = t;
    }
  }

  if(toDoCharacteristic.written()) {
    Serial.println("test");
  }

  if(ledCharacteristic.written()){
    if(ledCharacteristic.value()){
      digitalWrite(13,HIGH);
    }
    else{
      digitalWrite(13,LOW);
    }
  }
}

void connectHandler(BLEDevice central){
  Serial.print("Connected to: ");
  Serial.println(central.address());
}

void disconnectHandler(BLEDevice central){
  Serial.print("Disconnected from: ");
  Serial.println(central.address());
}
