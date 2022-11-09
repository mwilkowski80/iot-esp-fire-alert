
#include "PrivateSettings.h"
#include "TaskManager.h"
#include "PeriodicRestart.h"
#include "WifiBlinkLed.h"
#include "CollectPublishTask.h"
#include <Arduino.h>
#include <Log64.h>
#include <WiFiUdp.h>

#include <memory>

TaskManager task_manager;
std::unique_ptr<WifiBlinkLed> wifi_blink_led;
std::unique_ptr<WiFiClient> wifi_client;
logger_t logger = LOGGER("iot-esp-fire-alert");
WiFiUDP udp;

void setup_collect_publish_task();

void setup() {
    LOG64_INIT();

    auto *blinking_led = new BlinkingLed(D4);
    wifi_blink_led = std::make_unique<WifiBlinkLed>(blinking_led);

    auto *wifi_network_task = new WifiNetworkTask(RP_WIFI_SSID, RP_WIFI_PASSWORD, 30000L);
    wifi_network_task->addListener(wifi_blink_led.get());
    task_manager.push_back(blinking_led);
    task_manager.push_back(wifi_network_task);

    wifi_client = std::make_unique<WiFiClient>();

    setup_collect_publish_task();

    task_manager.push_back(new PeriodicRestart(1000 * 3600 * 6));
    task_manager.setup();
}

template<typename T>
bool publish_via_udp(T value) {
    if (udp.beginPacket(UDP_SERVER_HOSTNAME, UDP_SERVER_PORT)) {
        size_t bytes_written = udp.write(String(value).c_str());
        bool end_packet_success = udp.endPacket();
        logger("Published value to the UDP server: " + String(value) +
               ". Bytes written: " + String(bytes_written) + ". End packet success: "
               + String(end_packet_success));
        return end_packet_success && bytes_written > 0;
    } else {
        logger("Failed to begin UDP packet");
        return false;
    }
}

void setup_collect_publish_task() {
    const uint8_t PIR_PIN = D7;
    const uint8_t PIR_BLINKING_LED_PIN = D6;

    BlinkingLed *pir_blinking_led = new BlinkingLed(PIR_BLINKING_LED_PIN, HIGH, LOW);

    pinMode(PIR_PIN, INPUT);

    auto collect_func = []() -> CollectResult<bool> {
        return {CollectResult<bool>::SUCCESS, digitalRead(PIR_PIN) == HIGH};
    };

    auto publish_func = [pir_blinking_led](bool state) -> bool {
        if (state) {
            pir_blinking_led->setConfig({LedStatus::OFF, millis() + 1000, 1000, 0});
        }
        return publish_via_udp(state);
    };

    task_manager.push_back(new CollectPublishTask<bool>(
            collect_func, publish_func, []() -> long { return (long) millis(); },
            300L, 60 * 1000L, 300L));
    task_manager.push_back(pir_blinking_led);
}

void loop() {
    task_manager.run();
    delay(100L);
}
