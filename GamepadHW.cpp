#include "GamepadHW.h"

uint16_t ANStick_Adj[4][4] =
{
    {700,1730,2130,3000},
    {500,1830,1925,3312}, 
    {700,1844,1998,3442},
    {500,1871,1936,3314}
};


void GamepadHW::GPIO_Init( void )
{
    pinMode(AN_PIN_LX,INPUT_PULLDOWN);
    pinMode(AN_PIN_LY,INPUT_PULLDOWN);
    pinMode(AN_PIN_RX,INPUT_PULLDOWN);
    pinMode(AN_PIN_RY,INPUT_PULLDOWN);
    pinMode(BTN_PIN_LST,INPUT_PULLDOWN);
    pinMode(BTN_PIN_RST,INPUT_PULLDOWN);
    pinMode(BTN_PIN_SEL,INPUT_PULLUP);
    pinMode(BTN_PIN_START,INPUT_PULLUP);
    pinMode(BTN_PIN_DPADR,INPUT_PULLUP);
    pinMode(BTN_PIN_DPADL,INPUT_PULLUP);
    pinMode(BTN_PIN_LSD,INPUT_PULLDOWN);
    pinMode(BTN_PIN_RSD,INPUT_PULLDOWN);
    pinMode(BTN_PIN_LTR,INPUT_PULLDOWN);
    pinMode(BTN_PIN_RTR,INPUT_PULLDOWN);    
    pinMode(BTN_PIN_N,INPUT_PULLUP);
    pinMode(BTN_PIN_W,INPUT_PULLUP);
    pinMode(BTN_PIN_E,INPUT_PULLUP);
    pinMode(BTN_PIN_S,INPUT_PULLUP);
    pinMode(BTN_PIN_DPADD,INPUT_PULLUP);  
    pinMode(BTN_PIN_DPADU,INPUT_PULLUP);
}

void GamepadHW::readPinData( void )
{
    uint8_t i;
    AnlogStickData[0] = analogRead(AN_PIN_LX);
    AnlogStickData[1] = analogRead(AN_PIN_LY);
    AnlogStickData[2] = analogRead(AN_PIN_RX);
    AnlogStickData[3] = analogRead(AN_PIN_RY); 
    AnlogStickData[4] = analogRead(BTN_PIN_LTR);
    AnlogStickData[5] = analogRead(BTN_PIN_RTR); 

    for(i=0;i<4;i++)
    {
        AN_Ctrl[i] = AN2Byte(AnlogStickData[i],ANStick_Adj[i][0],ANStick_Adj[i][1],ANStick_Adj[i][2],ANStick_Adj[i][3]);  
    }
    
    AN_Ctrl[4] = ANTR2Byte(AnlogStickData[4]); 
    AN_Ctrl[5] = ANTR2Byte(AnlogStickData[5]); 
 
//    Serial.printf("LX:%d  LY:%d  RX:%d  RY:%d  LTR:%d  RTR:%d\r\n",
//    AnlogStickData[0],
//    AnlogStickData[1],
//    AnlogStickData[2],
//    AnlogStickData[3],    
//    AnlogStickData[4],
//    AnlogStickData[5]);
    //Serial.printf("BT:%d  \r\n",ButtonData);

    buttonData = ((~digitalRead(BTN_PIN_DPADU))&0x01)<<BT_DPADU;
    buttonData |= ((~digitalRead(BTN_PIN_DPADR))&0x01)<<BT_DPADR;
    buttonData |= ((~digitalRead(BTN_PIN_DPADD))&0x01)<<BT_DPADD;
    buttonData |= ((~digitalRead(BTN_PIN_DPADL))&0x01)<<BT_DPADL;
    buttonData |= ((~digitalRead(BTN_PIN_W))&0x01)<<BT_BTW;
    buttonData |= ((~digitalRead(BTN_PIN_N))&0x01)<<BT_BTN;        
    buttonData |= ((~digitalRead(BTN_PIN_S))&0x01)<<BT_BTS;
    buttonData |= ((~digitalRead(BTN_PIN_E))&0x01)<<BT_BTE;
    buttonData |= digitalRead(BTN_PIN_LSD)<<BT_LSD;
    buttonData |= digitalRead(BTN_PIN_RSD)<<BT_RSD;
    buttonData |= (AN_Ctrl[4]>70?0x01:0x00)<<BT_LTR;
    buttonData |= (AN_Ctrl[5]>70?0x01:0x00)<<BT_RTR; 
    buttonData |= digitalRead(BTN_PIN_LST)<<BT_LST;  
    buttonData |= digitalRead(BTN_PIN_RST)<<BT_RST;         
    buttonData |= ((~digitalRead(BTN_PIN_SEL))&0x01)<<BT_SEL;    
    buttonData |= ((~digitalRead(BTN_PIN_START))&0x01)<<BT_START;  

    Buttons = buttonData;                
    //Serial.printf("LX:%d  LY:%d  RX:%d  RY:%d\r\n",AN_Ctrl[0],AN_Ctrl[1],AN_Ctrl[2],AN_Ctrl[3]);   
}


uint8_t GamepadHW::AN2Byte( int AN_Data , uint16_t ANMin, uint16_t ANMidL, uint16_t ANMidR, uint16_t ANMax )
{   
    if( AN_Data > ANMax )
        AN_Data = ANMax;
    else if(AN_Data < ANMin)
        AN_Data = ANMin;

  
    if(AN_Data>ANMidR)
    {           
        return ((AN_Data - ANMidR)*127)/(ANMax-ANMidR) + 127; 
    }
    else if( AN_Data<ANMidL )
    {
        return ((AN_Data - ANMin)*127)/(ANMidL-ANMin);
    }
    else
        return 127;
}


uint8_t GamepadHW::ANTR2Byte( int AN_Data )
{   
    if(AN_Data>1870)
    {
        AN_Data = 1870;
    }
    else if(AN_Data<500)
    {
        AN_Data = 500;
    }

    return ((1870 - AN_Data)*2)/11;
}
