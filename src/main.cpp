#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>

ESP8266WiFiMulti WiFiMulti;

const String URL = "http://rose-garden-api.herokuapp.com";
bool loading = false;

void setup()
{

  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  for (uint8_t t = 4; t > 0; t--)
  {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("Araujo_Leal", "5qvAP98xczftal12");
}

void request(String path)
{
  if (loading)
    return;
  loading = true;
  Serial.println("Sending request...");
  Serial.println(path);
  Serial.println(path.length());
  WiFiClient client;
  HTTPClient http;
  if (http.begin(client, URL + path))
  {

    int httpCode = http.GET();
    if (httpCode > 0)
    {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
      {
        String payload = http.getString();
        Serial.println("[RESPONSE] " + payload);
      }
    }
    else
    {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }
  loading = false;
}

void loop()
{
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED))
  {

    if (Serial.available() > 0)
    {
      String line = Serial.readStringUntil('\n');
      if (line.startsWith("[REQUEST] "))
      {
        line.replace("[REQUEST] ", "");
        request(line.substring(0, line.length() - 1));
      }
    }
  }
}
