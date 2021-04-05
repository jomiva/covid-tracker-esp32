#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

#define ssid "Palixmajo"
#define password "26540mj522"

void getCovidData()
{
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println(F("COVID Tracker"));

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  // put your main code here, to run repeatedly:
  getCovidData();
  delay(260000);
}