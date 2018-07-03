#include <Arduino.h>
#include <Wire.h>
#include "Secret.h"
#include <U8g2lib.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include "ThingSpeak.h"
#include <math.h>
#include <WiFiUdp.h>

U8G2_SSD1306_64X48_ER_F_HW_I2C u8g2(U8G2_R0); // hardware
Adafruit_SHT31 sht31 = Adafruit_SHT31();
ESP8266WiFiMulti WiFiMulti;
WiFiClient  client;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "time.nist.gov", 36000);

float humi, pressure, temp;
const int UPDATE_INTERVAL_MINUTES = 5*60;

void setup(){
  Serial.begin(115200);
  delay(10);

  u8g2.begin();
  timeClient.begin();
  ThingSpeak.begin(client);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connect WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println(WiFi.localIP());

  if (! sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }

  delay(1000);
}

//*********************************************************************
void loop()
{
  temp = sht31.readTemperature();
  humi = sht31.readHumidity();

  timeClient.update();
  Serial.println(timeClient.getFormattedTime());

  if (! isnan(temp)) {  // check if 'is not a number'
    Serial.print("Temp *C\t\t\t");
    Serial.println(temp);
  } else {
    Serial.println("Failed to read temperature");
  }

  if (! isnan(humi)) {  // check if 'is not a number'
    Serial.print("Hum. %\t\t\t");
    Serial.println(humi);
  } else {
    Serial.println("Failed to read humidity");
  }

   Serial.print(F("Up time mins:\t\t"));
   Serial.println(float((millis()/1000)/60));
   Serial.println();

   u8g2.firstPage();
   do {
     u8g2.setFont(u8g2_font_helvR12_tf);
     u8g2.setContrast(10);
     u8g2.setCursor(0, 15);

     u8g2.print(temp,1);
     u8g2.print("\xb0");
     u8g2.print("C");

     u8g2.setCursor(0, 30);
     u8g2.print(humi,1);
     u8g2.print("%");

     u8g2.setCursor(0, 45);
     u8g2.print(timeClient.getFormattedTime());
   } while ( u8g2.nextPage() );

   ThingSpeak.setField(3,temp);
   ThingSpeak.setField(4,humi);
   ThingSpeak.writeFields(THINKSPEAK_CHANNEL, THINGSPEAK_API_KEY);

   delay(1000 * UPDATE_INTERVAL_MINUTES);
}
