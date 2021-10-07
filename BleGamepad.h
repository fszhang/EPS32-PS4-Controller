#ifndef ESP32_BLE_GAMEPAD_H
#define ESP32_BLE_GAMEPAD_H
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "BleConnectionStatus.h"
#include "BLEHIDDevice.h"
#include "BLECharacteristic.h"

#define GAMEPAD_ID      0x01
class BleGamepad {
private:
    BleConnectionStatus* connectionStatus;
    BLEHIDDevice* hid;
    BLECharacteristic* inputGamepad;
    static void taskServer(void* pvParameter);
public:
    uint8_t HID_Type=0;
    BleGamepad(std::string deviceName = "ZGamepad", std::string deviceManufacturer = "Ztech", uint8_t batteryLevel = 100);
    void begin(void);
    void end(void);
    void setAutoReport(bool autoReport = true);
    void sendReport(uint8_t ID, uint8_t* reportData, uint8_t DataLen);
    bool isConnected(void);
    void setBatteryLevel(uint8_t level);
    uint8_t batteryLevel;
    std::string deviceManufacturer = "ZGamepad";
    std::string deviceName = "Ztech";
protected:
  virtual void onStarted(BLEServer *pServer) { };
};

extern BleGamepad bleGamepad;

#endif // CONFIG_BT_ENABLED
#endif // ESP32_BLE_GAMEPAD_H
