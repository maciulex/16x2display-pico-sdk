#ifndef DISPLAY16X2
#define DISPLAY16X2

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <stdio.h>
#include <string>
#ifndef I2C_CONFIG
    #define I2C_CONFIG
    #define I2C_INTERFACE i2c0
    #define I2C_SDA_PIN 4
    #define I2C_SCL_PIN 5
#endif

namespace display16x2 {
    const uint8_t DisplayAddress =(0x7c>>1);
    const uint8_t CommandRegister= 0b00000000; //0b10000000
    const uint8_t DataRegister   = 0b01000000; //0b01000000 
    
    void send(uint8_t *data, uint8_t length, bool isCommand);
    void initLed();
    void clearDisplay();
    void initI2C();
    void init(bool initI2CProtocol = true);
    void sendMessage(std::string input);
    void displaySetting(bool displayOn = true, bool cursorOn = false, bool cursorBlink = false);
    //line <1,2> pos <0,16>
    void positionSet(uint8_t line = 0, uint8_t pos = 0);

    void positionSet(uint8_t line, uint8_t pos) {
        uint8_t lineWidth = 16;
        uint8_t lineAddressStart[2] {0, 0x40};
        if (line > 2 || pos > 16) return;
        uint8_t command = 0b1000'0000;
        command += lineAddressStart[line - 1];
        command += pos;
        send(&command, 1, true);
    }

    void displaySetting(bool displayOn, bool cursorOn, bool cursorBlink) {
        uint8_t command[1] {0b0000'1000};
        if (displayOn) {
            command[0] |= 0b0000'0100;
        }
        if (cursorOn) {
            command[0] |= 0b0000'0010;
        }
        if (cursorBlink) {
            command[0] |= 0b0000'0001;
        }
        send(&command[0], 1, true);
        sleep_ms(2);
    }

    void sendMessage(std::string input) {
        for (int i = 0; i < input.length(); i++) {
            uint8_t Char = (uint8_t(input[i]));
            send(&Char, 1, false);
        }
    }

    void send(uint8_t *data, uint8_t length, bool isCommand) {
        uint8_t Command[2];
        Command[1] = data[0];

        if (isCommand) Command[0] = CommandRegister;
        else Command[0] = DataRegister;

        i2c_write_blocking(I2C_INTERFACE, DisplayAddress, &Command[0], 2, false);
        sleep_ms(1);
    }
    


    void initLed() {
        uint8_t command[1] {0b0011'0000};
        send(&command[0], 1, true);
        sleep_ms(5);
    }

    void returnCursor() {
        uint8_t command[1] {0b0000'0010};
        send(&command[0], 1, true);
        sleep_ms(2);
    }

    void clearDisplay() {
        uint8_t command[1] {0b0000'0001};
        send(&command[0], 1, true);
        sleep_ms(2);
    }

    void initI2C() {
        i2c_init(I2C_INTERFACE, 400'000);

        gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
        gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
        gpio_pull_up(I2C_SDA_PIN);
        gpio_pull_up(I2C_SCL_PIN);
    }

    void init(bool initI2CProtocol) {
        if (initI2CProtocol) initI2C();
        uint8_t command[1];
        
        //init process
        sleep_ms(45);

        command[0] = 0b0010'1000;
        send(&command[0], 1, true);
        sleep_ms(5);


        displaySetting(true, false, true); 


        clearDisplay();

        command[0] = 0b0000'0010;
        send(&command[0], 1, true);
        sleep_ms(5);

        //initprocess end
        clearDisplay();
        clearDisplay();
        
        sendMessage("Hello world, i dont like this world");
        returnCursor();
        positionSet(2, 0);
        sendMessage("buda");
    }

}


#endif