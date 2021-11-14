#include <Arduino.h>
#include <WiFi.h>
#include <FlashHandler.h>
#include <WiFiHandler.h>
#include <TimeHandler.h>
#include <WebServer.h>
#include "time.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <string>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <CommonFunctions.h>

FlashHandler flashHandler(FlashIndex::FLASH_INDEX_MAX);
WiFiHandler wifiHandler("ASUS", "12345678", &flashHandler);

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 0;

Adafruit_BME280 bme;
TimeHandler<> timeHandler("pool.ntp.org", 1);

void initBMP280()
{
  Serial.println("Init BMP280");
  if (!bme.begin(BME280_ADDRESS_ALTERNATE))
  {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    delay(10000);
    ESP.restart();
  }
  /* Default settings from datasheet. */
  // bme.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
  //                 Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
  //                 Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
  //                 Adafruit_BMP280::FILTER_X16,      /* Filtering. */
  //                 Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

WiFiClient espClient;
PubSubClient client(espClient);

const char *mqtt_server = "192.168.31.30";
const int mqtt_port = 1883;
const char *mqtt_user = "guest";
const char *mqtt_password = "guest";

bool connectMqtt()
{
  Serial.print("Attempting MQTT connection...");
  String clientId = "ESP8266Client";
  if (client.connect(clientId.c_str(), mqtt_user, mqtt_password))
  {
    Serial.println("connected");
    return true;
  }
  else
  {
    Serial.print("failed, rc=");
    Serial.print(client.state());
    Serial.println(" try again in 5 seconds");
    delay(5000);
    return false;
  }
}

/*
{
  "sensorType": "temperature",
  "sensorChip": "bmp280",
  "dateTime": "2021-11-07T12:19:09",
  "unit": "Celcius",
  "symbol": "\u2103",
  "data": [
    48.75608
  ]
}
*/

void sendSensorData()
{
  String temperature = "/sensor/temperature";
  String pressure = "/sensor/pressure";
  String humidity = "/sensor/humidity";

  StaticJsonDocument<192> json1;
  json1["sensorType"] = "temperature";
  json1["sensorChip"] = "bme280";
  json1["datetime"] = timeHandler.getIsoTime().getDateTime();
  json1["unit"] = "Celcius";
  json1["symbol"] = "\u2103";
  JsonArray data1 = json1.createNestedArray("data");
  data1.add(bme.readTemperature());

  String json1Str;
  serializeJson(json1, json1Str);
  Serial.println(json1Str);
  Serial.println("");

  client.publish(temperature.c_str(), json1Str.c_str());

  StaticJsonDocument<192> json2;
  json2["sensorType"] = "pressure";
  json2["sensorChip"] = "bme280";
  json2["datetime"] = timeHandler.getIsoTime().getDateTime();
  json2["unit"] = "Pascal";
  json2["symbol"] = "Pa";
  JsonArray data2 = json2.createNestedArray("data");
  data2.add(bme.readPressure());

  String json2Str;
  serializeJson(json2, json2Str);
  Serial.println(json2Str);
  Serial.println("");

  client.publish(pressure.c_str(), json2Str.c_str());

  StaticJsonDocument<192> json3;
  json3["sensorType"] = "humidity";
  json3["sensorChip"] = "bme280";
  json3["datetime"] = timeHandler.getIsoTime().getDateTime();
  json3["unit"] = "Percent";
  json3["symbol"] = "%";
  JsonArray data3 = json3.createNestedArray("data");
  data3 = json3.createNestedArray("data");
  data3.add(bme.readHumidity());

  String json3Str;
  serializeJson(json3, json3Str);
  Serial.println(json3Str);
  Serial.println("");

  client.publish(pressure.c_str(), json3Str.c_str());

  //"Format rozszerzony YYYY-MM-DDThh:mm:ssZ"
}

void setup()
{
  // put your setup code here, to run once:

  Serial.begin(115200);
  Serial.println("Cpu speed:");
  Serial.println(ESP.getCpuFreqMHz());

  Serial.println("FlashHandler init");
  flashHandler.registerStorage(40);
  flashHandler.registerStorage(40);

  Serial.println("Wifi handler init");
  Serial.println("");
  if (wifiHandler.connectDefault())
  {
    Serial.print("Connected to ");
    Serial.println(WiFi.SSID());
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.println("Failed to connect to wifi");
    ESP.deepSleep(5 * 60 * 1000000);
    ESP.restart();
  }

  timeHandler.connect();

  initBMP280();

  client.setServer(mqtt_server, mqtt_port);

  connectMqtt();

  sendSensorData();

  client.disconnect();
  ESP.deepSleep(5 * 60 * 1000000);
  ESP.restart();
}

void loop()
{
  // put your main code here, to run repeatedly:
}