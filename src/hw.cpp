/*
 *  Project 41-mqtt_00 - Austral - EAM
 *
 *  hw.cpp
 *      Hardware dependent code
 */

#include <Arduino.h>

#include "hw.h"


/*
 *  Public functions
 */

/*
 *  get_board_num
 *      Return board number read from IB1,IB0
 */


int
get_board_num(void)
{
    return digitalRead(IB0) | (digitalRead(IB1) << 1); 
}

/*
 *  init_hw
 *      Called at power up
 */

void
init_hw(void)
{
    pinMode(MOTOR, OUTPUT);
    // pinMode(LED, OUTPUT);
    pinMode(SWITCH, INPUT_PULLUP);
    pinMode(IB0,INPUT_PULLUP);
    pinMode(IB1,INPUT_PULLUP);
}

/*
 *  verify_hw
 *      Verifies all input hardware
 */

int
verify_hw(void)
{
    return verify_half_turn();
}

/*
 *  Private functions
 */

/*
 *  verify_half_turn
 *      Verifies half turn switch
 */

static int verify_half_turn(void)
{
    int i;

    for (i = 0; i < 10; i++)
    {
        if (digitalRead(SWITCH) == 0)
        {
            return HALF_TURN;
        }
        delay(100);
    }
    return NO_NEWS;
}
