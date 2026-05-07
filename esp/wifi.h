/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "config.h"

/* Private defines -----------------------------------------------------------*/
#define WIFI_CONNECTION_TIMEOUT_SECONDS 30 // Tiempo maximo de reintentos de conexion

/* Globals -------------------------------------------------------------------*/
WiFiClientSecure espWifiClient;            // Cliente WiFi seguro

// Identificadoes de la red wifi
const char *wifiSSID    = NET_SSID;        
const char *wifiPasswd  = NET_PASSWD;

/* Functions -----------------------------------------------------------------*/

void wifi_reconnect(uint retries)    // Funcion que intenta conectar al wifi
{
  Serial.print("Connecting to ");
  Serial.println(wifiSSID);
  WiFi.begin(wifiSSID, wifiPasswd);

  uint8_t r = 0;
  while (WiFi.status() != WL_CONNECTED && r < retries)  // Mientras que no este conectado y no se haya supera el nuemero de reintentos, lo sigue intentando
  {
    r++;
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");

  if (WiFi.isConnected())                // Si se ha conectado, saca por pantalla la IP
  {
    Serial.println("Connected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP().toString());
  }
  else                                  // Si no lo ha conseguido saca un mensaje de fallo
  {
    Serial.println("Cannot connect to the WiFi network");
  }
}

void wifi_connect()      // Funcion que inicia la conexion Wifi
{
  delay(10);
  WiFi.mode(WIFI_STA);      // Pone el modo en cliente Wifi
  espWifiClient.setInsecure();     // Se desactiva la verificacion TLS para aceptar cualquier certificado  
  wifi_reconnect(WIFI_CONNECTION_TIMEOUT_SECONDS);    // Llama a la funcion antererior para que se intente conectar
}

void wifi_loop() // Funcion para llamarla periodicamente, si la conexion se cae, vuelve a llamar a la funcion para conectarse
{
  if (!WiFi.isConnected())
    wifi_reconnect(WIFI_CONNECTION_TIMEOUT_SECONDS);
}

#endif
/*** End of file **************************************************************/
