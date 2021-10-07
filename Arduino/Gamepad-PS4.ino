#include "BleGamepad.h"
#include "GamepadCtrl.h"
#include "GamepadHW.h"
#include <string.h>

GamepadCtrl gamepadCtrl;
GamepadHW gamepadHW;

uint8_t PowerUp = 0;
uint16_t PowerUpIndex;
void setup() 
{ 
    Serial.begin(115200);
    Serial.println("Starting BLE work!");
    bleGamepad.begin();
    gamepadHW.GPIO_Init();
}

void loop() 
{
    gamepadHW.readPinData();   
    if(bleGamepad.isConnected()) 
    {
        if(PowerUp==0)  //Power up first time
        {           
            PowerUpIndex++;
            delay(200);
            if(PowerUpIndex>15) //Power up for a while 
                PowerUp = 1;
        }//PowerUp==0
        else    //Power up for a while, than can send data
        { 
            gamepadCtrl.setCtrlData(
                gamepadHW.Buttons,      //Buttons, 16bit
                gamepadHW.AN_Ctrl[0],   //X
                gamepadHW.AN_Ctrl[1],   //Y
                gamepadHW.AN_Ctrl[2],   //Z
                gamepadHW.AN_Ctrl[4],   //Rx
                gamepadHW.AN_Ctrl[5],   //Ry
                gamepadHW.AN_Ctrl[3]);  //Rz
            gamepadCtrl.sendReport();
            delay(20);                                                                                  
        }//PowerUp for a while can send data
    }//Connected  
    else
    {
    }
}
