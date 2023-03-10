//#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "display16x2.cpp"

int main() {
    stdio_init_all();
    sleep_ms(2000);
    printf("Start:\n");
    display16x2::init();
    while(true) {
        printf("%i\n", (int)'D');
        sleep_ms(1000);
    }
    return 0;
}