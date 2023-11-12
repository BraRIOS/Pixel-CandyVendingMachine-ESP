/*
 *  Project 41-mqtt_00 - Austral - EAM
 *
 *  wifi_ruts.cpp
 *      Routines code for WiFi management
 */

#include <Arduino.h>

#include <WiFi.h>

#include "wifi_ruts.h"      //  Prototypes of routines whose code is in this module
#include "wifi_data.h"      //  Data configuration for WiFi connection

void
connect_wifi(void)
{
    printf("Trying to connect to %s\n", MY_SSID);
    WiFi.begin(MY_SSID, MY_PASS);

    while( WiFi.status() != WL_CONNECTED )
    {
        Serial.print(".");
        delay(500);
    }
    Serial.println();

    Serial.println("______________");
    Serial.print( "Conected to ");
    Serial.println( MY_SSID );
    Serial.print( "MAC = ");
    Serial.println( WiFi.macAddress() );
    Serial.print( "local IP = " );
    Serial.println( WiFi.localIP() );
    Serial.println("______________");
}

void
disconect_wifi(void)
{
    WiFi.disconnect();
}

int
is_connected_wifi(void)
{
    return WiFi.isConnected();
}


