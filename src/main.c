#include "stdbool.h"
#include "compiler.h"
#include "STC89xx.h"
#include "1602.h"

#define _P0 0x80
SBIT(DU, _P0, 6);



int main() {
    P2 = 0x00;
    DU = 1;
    DU = 0;

    init_1602();

    display_1602_str(1, 1, "Guessing Game!");
    display_1602_str(2, 1, "Press any button to start!");
    while (1);
}
