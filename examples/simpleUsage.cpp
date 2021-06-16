#include <Arduino.h>
#include <WiFi.h>
#include <esp_log.h>

#include "streamer.hpp"

const char * SSID = "###YOUR SSID###";
const char * PASSWORD = "###YOUR PASSWORD###";

/// Streamer instance
Streamer stream;

void setup(){
    Serial.begin(115200);
    delay(1000);
    log_i("Begin");

    WiFi.begin(SSID, PASSWORD);
    while ( WiFi.status() != WL_CONNECTED ){
        delay(500);
    }

    /// starts scheduler
    vTaskStartScheduler();

    /// start streaming with 2 fps
    stream.begin(2);

}

void loop(){
    /// allows other tasks to run
    vTaskDelay(1000);
}