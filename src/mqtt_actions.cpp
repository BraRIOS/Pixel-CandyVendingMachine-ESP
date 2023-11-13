/*
 *  Project 41-mqtt_00 - Austral - EAM
 *
 *  mqtt_actions.cpp
 *      Actions called as suscribed topics are received
 *      Low level actions called from here are in 'hw_actions.cpp'
 */

#include <Arduino.h>
#include <TelnetStream.h>
#include <cstring>

#include "mqtt_actions.h"       //  Prototypes of functions whose code are here
#include "websocket.h"

extern int board;
extern int candiesStock;
extern int price;

void
init_values( int origin, char *msg )
{
    TelnetStream.printf("%02d -> %02d |  %s: msg -> %s\n", origin, board, __FUNCTION__, msg );
    // Parsea el mensaje para obtener el stock y el precio
    char *token = strtok(msg, " ");

    // Obtiene el primer número (stock)
    if (token != nullptr) {
        candiesStock = std::atoi(token);
    }

    // Obtiene el segundo número (precio)
    token = strtok(nullptr, " ");
    if (token != nullptr) {
        price = std::atoi(token);
    }
    notifyCandies();
    notifyPrice();
}

void refill( int org, char *msg ){
    TelnetStream.printf("%02d -> %02d |  %s: msg -> %s\n", org, board, __FUNCTION__, msg );
    candiesStock += std::atoi(msg);
    notifyCandies();
}
void new_price( int org, char *msg ){
    TelnetStream.printf("%02d -> %02d |  %s: msg -> %s\n", org, board, __FUNCTION__, msg );
    price = std::atoi(msg);
    notifyPrice();
}