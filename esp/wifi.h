/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "config.h"

/* Private defines -----------------------------------------------------------*/
#define WIFI_CONNECTION_TIMEOUT_SECONDS 30

/* Globals -------------------------------------------------------------------*/
WiFiClientSecure espWifiClient;

const char *wifiSSID    = NET_SSID;
const char *wifiPasswd  = NET_PASSWD;

/* Functions -----------------------------------------------------------------*/

void wifi_reconnect(uint retries)
{
  Serial.print("Connecting to ");
  Serial.println(wifiSSID);
  WiFi.begin(wifiSSID, wifiPasswd);

  uint8_t r = 0;
  while (WiFi.status() != WL_CONNECTED && r < retries)
  {
    r++;
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");

  if (WiFi.isConnected())
  {
    Serial.println("Connected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP().toString());
  }
  else
  {
    Serial.println("Cannot connect to the WiFi network");
  }
}

void wifi_connect()
{
  delay(10);
  WiFi.mode(WIFI_STA);
  espWifiClient.setInsecure();   
  wifi_reconnect(WIFI_CONNECTION_TIMEOUT_SECONDS);
}

void wifi_loop()
{
  if (!WiFi.isConnected())
    wifi_reconnect(WIFI_CONNECTION_TIMEOUT_SECONDS);
}

#endif
/*** End of file **************************************************************/