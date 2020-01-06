#ifndef DisplayService_h
#define DisplayService_h

#include <ArduinoJson.h>
#include <TimeLib.h>
#include <BrewUNO/enum.h>
#include <BrewUNO/ActiveStatus.h>
#include <LiquidCrystal_I2C.h>
#include <WiFiStatus.h>
#include <APStatus.h>

#define PCF8574_ADDRESS 0x38

#define apmode_icon 1
#define stmode_icon 2
#define gpump_icon 3
#define pheater_icon 4
#define sheater_icon 5
#define gcelsius_icon 6
#define gwm_icon 7
#define gpw_icon 8

class DisplayService
{
public:
    DisplayService(ActiveStatus *activeStatus, WiFiStatus *wifiStatus, LiquidCrystal_I2C *lcd);
    ~DisplayService();
    void loop();
    void begin();

private:
    LiquidCrystal_I2C *_lcd;
    WiFiStatus *_wifiStatus;
    ActiveStatus *_activeStatus;

    void autoScan();
    void printHead();
    void printFooter();
    String GetCount(bool down);
    void RemoveLastChars(int length);
    String getTemperature(double targetTemperature, bool target);
    void printBody(int line, byte heatIcon, byte pwmIcon, double temperature, double targetTemperature, double pwm, bool pump, bool brewStarted, bool showPump, bool sparge, bool EnableSparge);
};
#endif