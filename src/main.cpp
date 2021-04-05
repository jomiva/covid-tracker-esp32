#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <TFT_eSPI.h>

#define ssid "Palixmajo"
#define password "26540mj522"

#define TFT_AQUA 0x04FF
#define TFT_GRAY 0x8410

WiFiClientSecure client;
TFT_eSPI tft = TFT_eSPI(135, 240);
DynamicJsonDocument doc(4096);

String fetchCovidData()
{
  String body = "";
  body.reserve(700);

  bool findHeaders = false;
  uint32_t now;
  client.setInsecure();

  if (client.connect("covid19.patria.org.ve", 443))
  {
    Serial.println(".... connected to server");
    client.println("GET /api/v1/summary HTTP/1.1");
    client.println("Host: covid19.patria.org.ve");
    client.println(F("User-Agent: arduino/1.0.0"));
    client.println("Accepts: application/json");
    client.println();
    now = millis();
    while (millis() - now < 3000)
    {
      while (client.available())
      {
        char c = client.read();
        // Serial.write(c);
        if (!findHeaders)
        {
          if (c == '{')
          {
            findHeaders = true;
            body = c;
          }
        }
        else
        {
          if (c == '\r')
          {
            break;
          }
          else
          {
            body += c;
          }
        }
      }
    }
  }
  client.stop();
  return body;
}

void printValuesToScreen()
{
  const int confirmed = doc["Confirmed"]["Count"];
  const int men = doc["Confirmed"]["ByGender"]["male"];
  const int women = doc["Confirmed"]["ByGender"]["female"];
  const int lara = doc["Confirmed"]["ByState"]["Lara"];
  const int recovered = doc["Recovered"]["Count"];
  const int deaths = doc["Deaths"]["Count"];
  const int active = doc["Active"]["Count"];

  Serial.println(confirmed);
  Serial.println(men);
  Serial.println(women);

  tft.setTextColor(TFT_WHITE);
  tft.drawString("Confirmados: ", 0, 38, 2);
  tft.drawString(String(confirmed).c_str(), 90, 38, 2);

  tft.setTextColor(TFT_GREEN);
  tft.drawString("Recuperados: ", 100, 54, 2);
  tft.drawString(String(recovered).c_str(), 180, 54, 2);

  tft.setTextColor(TFT_BLUE);
  tft.drawString("Hombres: ", 0, 57, 2);
  tft.drawString(String(men).c_str(), 55, 57, 2);

  tft.setTextColor(TFT_RED);
  tft.drawString("Muertos: ", 100, 80, 2);
  tft.drawString(String(deaths).c_str(), 160, 80, 2);

  tft.setTextColor(TFT_PURPLE);
  tft.drawString("Mujeres: ", 0, 78, 2);
  tft.drawString(String(women).c_str(), 55, 78, 2);

  tft.setTextColor(TFT_OLIVE);
  tft.drawString("Activos: ", 100, 100, 2);
  tft.drawString(String(active).c_str(), 160, 100, 2);
}

void getCovidData()
{
  const String response = fetchCovidData();

  DeserializationError error = deserializeJson(doc, response);
  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    getCovidData();
    return;
  }
  else
  {
    printValuesToScreen();
    return;
  }
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println(F("COVID Tracker"));

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_AQUA);
  tft.drawString("COVID TRACKER", 0, 0, 4);

  WiFi.begin(ssid, password);

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