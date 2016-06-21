/*
*   utils.c
*/

#include "utils.h"
#include "screen.h"
#include "i2c.h"
#include "buttons.h"

u32 waitInput(void)
{
    u32 pressedKey = 0,
        key;

    //Wait for no keys to be pressed
    while(HID_PAD);

    do
    {
        //Wait for a key to be pressed
        while(!HID_PAD);

        key = HID_PAD;

        //Make sure it's pressed
        for(u32 i = 0x13000; i; i--)
        {
            if(key != HID_PAD) break;
            if(i == 1) pressedKey = 1;
        }
    }
    while(!pressedKey);

    return key;
}

void mcuShutDown(void)
{
    i2cWriteRegister(I2C_DEV_MCU, 0x20, 1);
    while(1);
}

void mcuReboot(void)
{
    i2cWriteRegister(I2C_DEV_MCU, 0x20, 1 << 2);
    while(1);
}

void error(const char *message)
{
    drawString("An error has occurred:", 10, 10, COLOR_RED);
    int posY = drawString(message, 10, 30, COLOR_WHITE);
    drawString("Press any button to continue booting from NAND", 10, posY + 2 * SPACING_Y, COLOR_WHITE);

    waitInput();

    // mcuShutDown();
}