// main.cpp
#include <Arduino.h>
#include <WiFiManager.h>
#include "aws_iot.h"
#include "sensor.h"
#include "secrets.h"
#include "config.h"

/**
 * The function `connectWifi` attempts to connect to WiFi using WiFiManager and restarts the ESP if
 * connection fails.
 * if connection fails. Connect to ESP32_XXXXX network and configure WiFi.
 */
void connectWifi() {
    WiFiManager wm;
    Serial.print("Connecting to Wifi ");
    if (!wm.autoConnect()) {
        Serial.println("Failed to connect, restarting...");
        delay(3000);
        ESP.restart();
    }
    Serial.println("Connected.");
}

void setup() {
    Serial.begin(115200);
    connectWifi();
    connectAWS();
    initSensor();
    NTPConnect();
}

void loop() {
    
    Serial.println("[LOOP] --- New loop iteration ---");
    Serial.print("[MEM] Free heap (start): ");
    Serial.println(ESP.getFreeHeap());

    clientLoop();

    Serial.println("[SENSOR] Reading sensors...");
    float distance = readUltrasonicCM(1.0, 0);
    float temp = readSensorData_DHT_T();
    float hum = readSensorData_DHT_H();
    int soil = readSensorData_SoilMoisture();
    Serial.print("[SENSOR] Distance: "); Serial.println(distance);
    Serial.print("[SENSOR] Temperature: "); Serial.println(temp);
    Serial.print("[SENSOR] Humidity: "); Serial.println(hum);
    Serial.print("[SENSOR] Soil Moisture: "); Serial.println(soil);

    Serial.println("[AWS] Publishing message to AWS IoT...");
    publishMessage(hum, temp, soil);
    Serial.println("[AWS] Message published.");
    delay(1000);

    Serial.print("[MEM] Free heap (end): ");
    Serial.println(ESP.getFreeHeap());
}
