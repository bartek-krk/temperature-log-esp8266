#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = ""; //your wlan ssid
const char* password = ""; //your wlan password
String apiKey = ""; //your api key
unsigned long delayMilliseconds(60000); //delay between requests

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println("Parsing JSON...");
  StaticJsonBuffer<200> jsonBuffer;
  char json[] = "{\"stationId\":3,\"temperature\":22.53}";
  JsonObject& root = jsonBuffer.parseObject(json);
  if(!root.success()) {
    Serial.println("parseObject() failed");
  } else {
    Serial.println("JSON parsing OK");
  }


  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print("Connecting to WLAN... ");
  }


  String measurementsURL = "http://temperature-log.herokuapp.com/api/measurements/";


  HTTPClient http;

  if (WiFi.status() == WL_CONNECTED) {
    http.begin(measurementsURL);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("api_key", apiKey);

    String data;
    root.printTo(data);
    int httpCode = http.PUT(data);

    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println(payload);
    }
    
    http.end();
    Serial.println(httpCode);
  }
  delay(delayMilliseconds);
}
