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
    
    // Client loop for AWS IoT
    clientLoop();


    // read Ultra Sonic
    float distance = readUltrasonicCM(1.0, 0);
    if (distance != -1) {
        Serial.print("Distance: ");
        Serial.print(distance);
        Serial.println(" cm");
    }

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    // Read sensor data and publish to AWS IoT
    publishMessage(readSensorData_DHT_H(), readSensorData_DHT_T(), readSensorData_SoilMoisture());
    delay(1000);
}
