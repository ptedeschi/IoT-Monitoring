#ifndef IOT_CONFIGS_H
#define IOT_CONFIGS_H

// Physical device information for board and sensor
#define DEVICE_ID "baby-room"
#define DHT_TYPE DHT11

// Pin layout configuration
#define DHT_DATA_PIN 4

// Interval time (microsecond) for sending message
#define INTERVAL 300000000 // (5 * 60 * 1000000) = 5 minutes

/**
   WiFi setup
*/
#define IOT_CONFIG_WIFI_SSID            ""
#define IOT_CONFIG_WIFI_PASSWORD        ""

/**
   ThingSpeak setup
*/
#define THINGSPEAK_API_KEY      ""
#define THINGSPEAK_SERVER     ""

#endif /* IOT_CONFIGS_H */
