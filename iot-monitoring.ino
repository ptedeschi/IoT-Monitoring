#include <ESP8266WiFi.h>
#include <time.h>
#include <sys/time.h>

//Include da lib do sensor DHT11 e DHT22
#include "DHT.h"

#include "config.h"

DHT dht(DHT_DATA_PIN, DHT_TYPE);
WiFiClient client;
ADC_MODE(ADC_VCC);

void setup() {
  initSerial();
  initWifi();
  initTime();
  initSensor();
}

void initSerial() {
  // Start serial and initialize stdout
  Serial.begin(9600);
  Serial.setDebugOutput(true);
}

void initWifi() {

  // check for the presence of the shield :
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(IOT_CONFIG_WIFI_SSID);

  // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
  WiFi.begin(IOT_CONFIG_WIFI_SSID, IOT_CONFIG_WIFI_PASSWORD);

  Serial.print("Waiting for Wifi connection.");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("Connected to wifi");
}

void initTime() {
  time_t epochTime;

  configTime(0, 0, "pool.ntp.org", "time.nist.gov");

  while (true) {
    epochTime = time(NULL);

    if (epochTime == 0) {
      Serial.println("Fetching NTP epoch time failed! Waiting 2 seconds to retry.");
      delay(2000);
    } else {
      Serial.print("Fetched NTP epoch time is: ");
      Serial.println(epochTime);
      break;
    }
  }
}

void initSensor() {
  dht.begin();
}

void loop() {
  //Leitura de umidade
  float humidity = dht.readHumidity();

  //Leitura de temperatura
  float temperature = dht.readTemperature();

  //Se não for um numero retorna erro de leitura
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Erro ao ler o sensor!");
    return;
  }

  if (client.connect(THINGSPEAK_SERVER, 80))
  {
    float vdd = ESP.getVcc() / 1024.0;
    
    String postStr = THINGSPEAK_API_KEY;
    postStr += "&field1=";
    postStr += String(DEVICE_ID);
    postStr += "&field2=";
    postStr += String(temperature);
    postStr += "&field3=";
    postStr += String(humidity);
    postStr += "&field4=";
    postStr += String(vdd);
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + String(THINGSPEAK_API_KEY) + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);

    Serial.print("\nDevice: ");
    Serial.print(DEVICE_ID);
    Serial.print("\nTemperature: ");
    Serial.print(temperature);
    Serial.print("\nHumidity: ");
    Serial.println(humidity);
    Serial.print("\nVDD: ");
    Serial.println(vdd);

    client.stop();
  }

  Serial.println("Going into deep sleep for " + String(INTERVAL) + " microseconds");
  ESP.deepSleep(INTERVAL);
}
