/*
 *  Project Pixel Candy Machine - Austral - Electrónica Digital     - Grupo 13
 *
 *  hw.h
 *      Hardware dependent code
 */

enum
{
    NO_NEWS, HALF_TURN
};

void init_hw(void);         //  Called at power up
int get_board_num(void);    //  return board number
void dispense_candies(int buyed);    //  Dispense candies