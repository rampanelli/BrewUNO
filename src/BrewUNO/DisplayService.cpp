#include <BrewUNO/DisplayService.h>

time_t lastUpdate = now();
String blankline = "                    ";
byte apmode[] = {B01010, B00100, B01010, B00100, B00100, B00100, B01110, B11111}; 
//{B10101, B10101, B10101, B01110, B00100, B00100, B00100, B00100};
byte stmode[] = {B01110, B11011, B10101, B11011, B10101, B01010, B01110, B00100};
byte gpump[] = {B00100, B00100, B01110, B01110, B11111, B11101, B11011, B01110};
//byte pheater[] = {B11100, B10100, B11100, B10101, B10101, B00111, B00101, B00101};
//byte pheater[] = {B01000, B11000, B01000, B01101, B01101, B00111, B00101, B00101};
byte pheater[] = {  B10100, B11100, B10100, B00010, B00110, B00010, B00010, B00111};
//byte sheater[] = {B11100, B10000, B11100, B00101, B11101, B00111, B00101, B00101};
//byte sheater[] = {B11100, B00100, B11000, B10101, B11101, B00111, B00101, B00101};
byte sheater[] = {  B10100, B11100, B10100, B00111, B00001, B00111, B00100, B00111};
byte gcelsius[] = {B01000, B10100, B01000, B00110, B01001, B01000, B01001, B00110};
byte gwm[] = {B11111, B01000, B00100, B01000, B11111, B00000, B11111, B00110};
byte gpw[] = {B00110, B11111, B00000, B11100, B10100, B10100, B11111, B00000};

DisplayService::DisplayService(ActiveStatus *activeStatus, WiFiStatus *wifiStatus, LiquidCrystal_I2C *lcd) : _activeStatus(activeStatus),
                                                                                                            _wifiStatus(wifiStatus),
                                                                                                            _lcd(lcd)
{
}

DisplayService::~DisplayService() {}

void DisplayService::begin()
{
    _lcd->init();
    _lcd->backlight();
    _lcd->createChar(apmode_icon, apmode);
    _lcd->createChar(stmode_icon, stmode);
    _lcd->createChar(gpump_icon, gpump);
    _lcd->createChar(pheater_icon, pheater);
    _lcd->createChar(sheater_icon, sheater);
    _lcd->createChar(gcelsius_icon, gcelsius);
    _lcd->createChar(gwm_icon, gwm);
    _lcd->createChar(gpw_icon, gpw);
}

void DisplayService::loop()
{
    if (now() - lastUpdate > 0.8)
    {
        lastUpdate = now();
        printHead();
        printBody(1, pheater_icon, gwm_icon, _activeStatus->Temperature, _activeStatus->TargetTemperature, _activeStatus->PWMPercentage,
                _activeStatus->PumpOn, _activeStatus->BrewStarted, true, false, _activeStatus->EnableSparge);
        printBody(2, sheater_icon, gpw_icon, _activeStatus->SpargeTemperature, _activeStatus->SpargeTargetTemperature, _activeStatus->SpargePWMPercentage,
                _activeStatus->PumpOn, _activeStatus->BrewStarted, false, true, _activeStatus->EnableSparge);
        printFooter();
    }
}

/* void DisplayService::printHead()
{
    _lcd->setCursor(0, 0);
    wl_status_t status = WiFi.status();
    WiFiMode_t currentWiFiMode = WiFi.getMode();
    if (status == WL_CONNECTED)
        _lcd->write(2);
    else if (currentWiFiMode == WIFI_AP || currentWiFiMode == WIFI_AP_STA)
        _lcd->write(1);
    _lcd->print(" BrewUNO  ");
    if (_activeStatus->BrewStarted && !_activeStatus->StepLocked)
        _lcd->print(" " + GetCount(true));
    else if (_activeStatus->StepLocked)
        _lcd->print(GetCount(false) + 'L');
    else
        _lcd->print("         ");
} */

void DisplayService::printHead()
{
    _lcd->setCursor(0, 0);
    wl_status_t status = WiFi.status();
    WiFiMode_t currentWiFiMode = WiFi.getMode();
    if (status == WL_CONNECTED)
        _lcd->write(2);
    else if (currentWiFiMode == WIFI_AP || currentWiFiMode == WIFI_AP_STA)
        _lcd->write(1);
    _lcd->print("BRewUNO 6 ");
    if (_activeStatus->BrewStarted && !_activeStatus->StepLocked) {
        _lcd->print(GetCount(true));
        _lcd->setCursor(19, 2);
        _lcd->print(" ");
    }
    else if (_activeStatus->StepLocked) {
        _lcd->print(GetCount(false));
        _lcd->setCursor(19, 2);
        _lcd->print("L");
    }
    else {
        _lcd->print("         ");
        _lcd->setCursor(19, 2);
        _lcd->print(" ");
    } 
}

void DisplayService::printBody(int line, byte heatIcon, byte pwmIcon, double temperature, double targetTemperature,
                               double pwm, bool pump, bool brewStarted, bool showPump, bool sparge, bool enableSparge)
{
    _lcd->setCursor(0, line);
    _lcd->write(heatIcon);

    if (sparge && !enableSparge)
        _lcd->print(" 00.00>00");
    else
        _lcd->print(" " + getTemperature(temperature, false) +
                    ">" + (brewStarted ? getTemperature(targetTemperature, true) : "00"));

    _lcd->setCursor(10, line);
    _lcd->write(gcelsius_icon);
    _lcd->setCursor(13, line);
    _lcd->write(pwmIcon);
    _lcd->setCursor(14, line);

    if (pwm <= 0)
        _lcd->print("  0%");
    else if (pwm < 10)
        _lcd->print("  " + String(pwm).substring(0, 1) + "%");
    else if (pwm <= 99)
        _lcd->print(" " + String(pwm).substring(0, 2) + "%");
    else
        _lcd->print("100%");

    _lcd->setCursor(19, 1);
    if (showPump && pump)
        _lcd->write(gpump_icon);
    else if (showPump)
        _lcd->print("P");
}

void DisplayService::printFooter()
{
    _lcd->setCursor(0, 3);
    if (!_activeStatus->BrewStarted)
    {
        String ip = "";
        wl_status_t status = WiFi.status();
        WiFiMode_t currentWiFiMode = WiFi.getMode();
        if (status == WL_CONNECTED)
            ip = "IP: " + WiFi.localIP().toString();
        else if (currentWiFiMode == WIFI_AP || currentWiFiMode == WIFI_AP_STA)
            ip = "IP: " + WiFi.softAPIP().toString();
        _lcd->print(ip);
        RemoveLastChars(ip.length());
    }
    else if (_activeStatus->ActiveStep == mash)
    {
        String step = _activeStatus->ActiveMashStepName.substring(0, 13) + " " + _activeStatus->ActiveMashStepSufixName.substring(0, 6);
        _lcd->print(step);
        RemoveLastChars(step.length());
    }
    else if (_activeStatus->ActiveStep == boil && _activeStatus->ActiveBoilStepName != "")
    {
        String boil = _activeStatus->ActiveBoilStepName.substring(0, 20);
        _lcd->print(boil);
        RemoveLastChars(boil.length());
    }
    else
        _lcd->print(blankline);
}

String DisplayService::getTemperature(double temperature, bool target)
{
    if (target)
        if (temperature <= 99)
            return String(temperature).substring(0, 2);
        else
            return "1H";
    else if (temperature <= 0)
        return "00.00";
    else if (temperature >= 100)
        return "100.0";
    else
        return String(temperature);
}

void DisplayService::RemoveLastChars(int length)
{
    for (int i = 0; i < 20 - length; i++)
        _lcd->print(" ");
}

String DisplayService::GetCount(bool down)
{
    int difference = down ? _activeStatus->EndTime - now() : now() - _activeStatus->EndTime;
    if (difference <= 0 && down)
        return "00:00:00";
    int seconds = floor(difference);
    int minutes = floor(seconds / 60);
    int hours = floor(minutes / 60);
    //hours %= 24;
    minutes %= 60;
    seconds %= 60;
    char buffer[16];
    sprintf(buffer, "%02u:%02u:%02u", hours, minutes, seconds);
    return buffer;
}