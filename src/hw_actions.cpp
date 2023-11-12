/*
 *  Project 41-mqtt_00 - Austral - EAM
 *
 *  hw_actions.cpp
 *      Low level actions in output hardware
 */

#include <Arduino.h>

#include "hw_actions.h"

void
toggle_led_red(void)
{
    digitalWrite(LED_RED, !digitalRead( LED_RED ));
}

void
toggle_led_yel(void)
{
    digitalWrite(LED_YEL, !digitalRead( LED_YEL ));
}

void
toggle_led_grn(void)
{
    digitalWrite(LED_GRN, !digitalRead( LED_GRN ));
}

void
toggle_led_int(void)
{
    digitalWrite(LED_INT, !digitalRead( LED_INT ));
}

void
set_led_red(int level)
{
    digitalWrite(LED_RED, level != 0);
}

void
set_led_yel(int level)
{
    digitalWrite(LED_YEL, level != 0);
}

void
set_led_grn(int level)
{
    digitalWrite(LED_GRN, level != 0);
}

void
set_led_int(int level)
{
    digitalWrite(LED_INT, level != 0);
}

void
clear_all_leds(void)
{
    set_led_red(0);
    set_led_yel(0);
    set_led_grn(0);
    set_led_int(0);
}


