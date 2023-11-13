/*
 *  Project 41-mqtt_00 - Austral - EAM
 *
 *      File main.cpp
 */

#include <Arduino.h>
#include <TelnetStream.h>

#include "wifi_ruts.h"
#include "mqtt.h"
#include "hw.h"

int board;

void
setup(void)
{
    Serial.begin(BAUD);

    connect_wifi();

    init_hw();
    board = get_board_num();
    printf("Board = %d\n", board);
    init_mqtt(board);
    TelnetStream.begin();
}

void
loop(void)
{
    if( verify_hw() == SWITCH )             //  Verify all input hardware
        do_publish("switch","pressed");
    test_mqtt();                            //  Test news from broker
}
