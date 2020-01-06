#include <Arduino.h>
#define ESPALEXA_ASYNC
#include <Espalexa.h>

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#elif defined(ESP_PLATFORM)
#include <WiFi.h>
#include <AsyncTCP.h>
#include <SPIFFS.h>
#endif

#include <FS.h>
#include <SecuritySettingsService.h>
#include <WiFiSettingsService.h>
#include <APSettingsService.h>
#include <NTPSettingsService.h>
#include <OTASettingsService.h>
#include <AuthenticationService.h>
#include <WiFiScanner.h>
#include <WiFiStatus.h>
#include <NTPStatus.h>
#include <APStatus.h>
#include <SystemStatus.h>

#include <PID_v1.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>
#include <pcf8574_esp.h>

#include <BrewUNO/ActiveStatus.h>
#include <BrewUNO/KeyPadService.h>
#include <BrewUNO/MashSettingsService.h>
#include <BrewUNO/BoilSettingsService.h>
#include <BrewUNO/BrewSettingsService.h>
#include <BrewUNO/BrewService.h>
#include <BrewUNO/MashService.h>
#include <BrewUNO/BoilService.h>
#include <BrewUNO/TemperatureService.h>
#include <BrewUNO/HeaterService.h>
#include <BrewUNO/MashKettleHeaterService.h>
#include <BrewUNO/SpargeKettleHeaterService.h>
#include <BrewUNO/ActiveStatus.h>
#include <BrewUNO/Buzzer.h>
#include <BrewUNO/Pump.h>
#include <BrewUNO/DisplayService.h>
#include <BrewUNO/InternationalizationService.h>
#include <BrewUNO/KeyPadService.h>
#include <BrewUNO/KeyButton.h>

#define SERIAL_BAUD_RATE 115200

//#define PCF8574_ADDRESS 0x38

OneWire oneWire(TEMPERATURE_BUS);
DallasTemperature DS18B20(&oneWire);
int deviceCount = 0;

LiquidCrystal_I2C lcd(0x0, 20, 4);

TwoWire pcfWire;
PCF857x pcf8574(PCF8574_ADDRESS, &pcfWire);

Espalexa espalexa;

AsyncWebServer server(80);

SecuritySettingsService securitySettingsService = SecuritySettingsService(&server, &SPIFFS);
WiFiSettingsService wifiSettingsService = WiFiSettingsService(&server, &SPIFFS, &securitySettingsService);
APSettingsService apSettingsService = APSettingsService(&server, &SPIFFS, &securitySettingsService);
OTASettingsService otaSettingsService = OTASettingsService(&server, &SPIFFS, &securitySettingsService);
AuthenticationService authenticationService = AuthenticationService(&server, &securitySettingsService);

WiFiScanner wifiScanner = WiFiScanner(&server, &securitySettingsService);
WiFiStatus wifiStatus = WiFiStatus(&server, &securitySettingsService);
NTPStatus ntpStatus = NTPStatus(&server, &securitySettingsService);
APStatus apStatus = APStatus(&server, &securitySettingsService);
SystemStatus systemStatus = SystemStatus(&server, &securitySettingsService);

//brewUNO
ActiveStatus activeStatus = ActiveStatus(&SPIFFS);
NTPSettingsService ntpSettingsService = NTPSettingsService(&server, &SPIFFS, &securitySettingsService, &activeStatus);

BrewSettingsService brewSettingsService = BrewSettingsService(&server, &SPIFFS, &activeStatus);
TemperatureService temperatureService = TemperatureService(&server, &SPIFFS, DS18B20, &brewSettingsService);
MashSettingsService mashSettings = MashSettingsService(&server, &SPIFFS);
BoilSettingsService boilSettingsService = BoilSettingsService(&server, &SPIFFS, &brewSettingsService);

Pump pump = Pump(&server, &activeStatus, &brewSettingsService);
DisplayService display = DisplayService(&activeStatus, &wifiStatus, &lcd);
MashKettleHeaterService mashKettleHeaterService = MashKettleHeaterService(&temperatureService, &activeStatus, &brewSettingsService);
SpargeKettleHeaterService spargeKettleHeaterService = SpargeKettleHeaterService(&temperatureService, &activeStatus, &brewSettingsService);
BoilKettleHeaterService boilKettleHeaterService = BoilKettleHeaterService(&temperatureService, &activeStatus, &brewSettingsService);
MashService mashService = MashService(&SPIFFS, &temperatureService, &pump);
BoilService boilService = BoilService(&SPIFFS, &temperatureService, &brewSettingsService);
BrewService brewService = BrewService(&server, &SPIFFS, &mashService, &boilService, &brewSettingsService, &mashKettleHeaterService, &spargeKettleHeaterService, &boilKettleHeaterService, &activeStatus, &temperatureService, &pump);
InternationalizationService internationalizationService = InternationalizationService(&server, &SPIFFS, &brewSettingsService);

const int btn1 = BUTTONUP_BUS;
const int btn2 = BUTTONDOWN_BUS;
const int btn3 = BUTTONSTART_BUS;
const int btn4 = BUTTONENTER_BUS;
KeyButton button1(btn1, pcf8574);
KeyButton button2(btn2, pcf8574);
KeyButton button3(btn3, pcf8574);
KeyButton button4(btn4, pcf8574);
KeyPadService keypad = KeyPadService(&activeStatus, &pcf8574, &brewService, &brewSettingsService, &pump, &button1, &button2, &button3, &button4);

int Dimmer = 0;
char Device1[] = "BrewUNO";    
char Device2[] = "Mostura";
char Device3[] = "Fervura";
char Device4[] = "Avanço"; 
char Device5[] = "Circulação";
char Device6[] = "Bomba";
char Device7[] = "Pausa";
char Device8[] = "Resumo";
char Device9[] = "P W M";

     
EspalexaDevice* espalexaPointer;
//Espalexa espalexa;

void AlexaCommands(EspalexaDevice* espalexaPointer) { 
   if(espalexaPointer == nullptr) return;
   int AlexaPercent = espalexaPointer->getPercent();

    if (espalexaPointer->getName() == "BrewUNO") { ////Alexa, desligue o brewuno.
      if (activeStatus.BrewStarted) {
        brewService.stopBrew(); }
    }

    if (espalexaPointer->getName() == "Mostura") { //Alexa, ativar/ligar mostura.
      if (!activeStatus.BrewStarted) {
        brewService.startBrew(); 
      }
    }

    if (espalexaPointer->getName() == "Fervura") { //Alexa, ativar/ligar fervura.
      if (!activeStatus.BrewStarted) {
        brewService.startBoil();
      }
    }

    if (espalexaPointer->getName() == "Avanço") { ////Alexa, ativar/ligar avanço.
      if (activeStatus.BrewStarted) {
        brewService.nextStep();
      } 
    }

    if (espalexaPointer->getName() == "Circulação") { //Alexa, ativar/ligar circulação.
      if (!activeStatus.PumpOn) {
       pump.TurnPumpOn();
      }  
    }

    if (espalexaPointer->getName() == "Bomba") { //Alexa, desativar/desligar bomba. 
      if (activeStatus.PumpOn) {
       pump.TurnPumpOff();
      }
    }

    if (espalexaPointer->getName() == "Pausa") { //Alexa, ativar pausa.
      if (activeStatus.BrewStarted) {
        brewService.pauseBrew();
      }
    }

    if (espalexaPointer->getName() == "Resumo") { //Alexa, ativar resumo.
        if (activeStatus.ActiveStep > 0 && activeStatus.ActiveStep != 3){
        brewService.resumeBrew();
      }
    }

    if (espalexaPointer->getName() == "P W M") { //Alexa, ajustar o PWM em 55 por cento!
      if (activeStatus.BrewStarted) {
         if (activeStatus.ActiveStep == mash) {
          brewSettingsService.MashHeaterPercentage=(AlexaPercent);
          } else if (activeStatus.ActiveStep == boil) {
            brewSettingsService.BoilPowerPercentage=(AlexaPercent); 
            }
      }
    }

}


void setup()
{
  // Disable wifi config persistance and auto reconnect
  WiFi.persistent(false);
  WiFi.setAutoReconnect(false);

#if defined(ESP_PLATFORM)
  // Init the wifi driver on ESP32
  WiFi.mode(WIFI_MODE_MAX);
  WiFi.mode(WIFI_MODE_NULL);
#endif

  Serial.begin(SERIAL_BAUD_RATE);
  SPIFFS.begin();
  //SPIFFS.format();

  // Start security settings service first
  securitySettingsService.begin();

  // Start services
  ntpSettingsService.begin();
  otaSettingsService.begin();
  apSettingsService.begin();
  wifiSettingsService.begin();

  // Serving static resources from /www/
  server.serveStatic("/js/", SPIFFS, "/www/js/");
  server.serveStatic("/css/", SPIFFS, "/www/css/");
  server.serveStatic("/fonts/", SPIFFS, "/www/fonts/");
  server.serveStatic("/app/", SPIFFS, "/www/app/");
  server.serveStatic("/favicon.ico", SPIFFS, "/www/favicon.ico");

  // Serving all other get requests with "/www/index.htm"
  // OPTIONS get a straight up 200 response
 /*  server.onNotFound([](AsyncWebServerRequest *request) {
    if (request->method() == HTTP_GET)
      request->send(SPIFFS, "/www/index.html");
    else if (request->method() == HTTP_OPTIONS)
      request->send(200);
    else
      request->send(404);
  }); */
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      if (request->method() == HTTP_GET)
      request->send(SPIFFS, "/www/index.html");
    else if (request->method() == HTTP_OPTIONS)
      request->send(200);
    else
      request->send(404);
    });
  server.onNotFound([](AsyncWebServerRequest *request){
      if (!espalexa.handleAlexaApiCall(request))
      {
        request->send(404, "text/plain", "Not found");
      }
    });

// Disable CORS if required
#if defined(ENABLE_CORS)
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", CORS_ORIGIN);
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Accept, Content-Type, Authorization");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Credentials", "true");
#endif

  //server.begin();

  //BrewUNO
  pinMode(PUMP_BUS, OUTPUT);
  pinMode(BUZZER_BUS, OUTPUT);
  digitalWrite(BUZZER_BUS, LOW);
  pinMode(HEATER_BUS, OUTPUT);
  pinMode(SPARGE_HEATER_BUS, OUTPUT);
  pinMode(BOIL_HEATER_BUS, OUTPUT);

  pump.TurnPumpOff();
  DS18B20.begin();
  // locate devices on the bus
  Serial.println("");
  Serial.println("Hello! I'm BrewUNO =) with Alexa (═══)!");
  Serial.println("Locating DS18B20 devices...");
  Serial.print("Found ");
  deviceCount = DS18B20.getDeviceCount();
  Serial.print(deviceCount, DEC);
  Serial.println(" devices.");
  Serial.println("");
  temperatureService.DeviceCount = deviceCount;
  brewSettingsService.begin();
  brewService.begin();
  display.begin();

  pcfWire.begin(D2, D1);
  //Specsheets say PCF8574 is officially rated only for 100KHz I2C-bus
  //PCF8575 is rated for 400KHz
  pcfWire.setClock(100000L);
  pcf8574.begin();

  espalexaPointer = new EspalexaDevice(Device1, AlexaCommands, EspalexaDeviceType::dimmable, Dimmer * 2.55); 
  espalexa.addDevice(espalexaPointer);
  espalexaPointer = new EspalexaDevice(Device2, AlexaCommands, EspalexaDeviceType::dimmable, Dimmer * 2.55); 
  espalexa.addDevice(espalexaPointer);
  espalexaPointer = new EspalexaDevice(Device3, AlexaCommands, EspalexaDeviceType::dimmable, Dimmer * 2.55); 
  espalexa.addDevice(espalexaPointer);
  espalexaPointer = new EspalexaDevice(Device4, AlexaCommands, EspalexaDeviceType::dimmable, Dimmer * 2.55); 
  espalexa.addDevice(espalexaPointer);
  espalexaPointer = new EspalexaDevice(Device5, AlexaCommands, EspalexaDeviceType::dimmable, Dimmer * 2.55); 
  espalexa.addDevice(espalexaPointer);
  espalexaPointer = new EspalexaDevice(Device6, AlexaCommands, EspalexaDeviceType::dimmable, Dimmer * 2.55); 
  espalexa.addDevice(espalexaPointer);
  espalexaPointer = new EspalexaDevice(Device7, AlexaCommands, EspalexaDeviceType::dimmable, Dimmer * 2.55); 
  espalexa.addDevice(espalexaPointer);
  espalexaPointer = new EspalexaDevice(Device8, AlexaCommands, EspalexaDeviceType::dimmable, Dimmer * 2.55); 
  espalexa.addDevice(espalexaPointer);
  espalexaPointer = new EspalexaDevice(Device9, AlexaCommands, EspalexaDeviceType::dimmable, Dimmer * 2.55); 
  espalexa.addDevice(espalexaPointer);  
   //espalexa.begin();
  espalexa.begin(&server);
  
}

void loop()
{
  wifiSettingsService.loop();
  apSettingsService.loop();
  ntpSettingsService.loop();
  otaSettingsService.loop();
  brewService.loop();
  display.loop();
  keypad.loop();
  espalexa.loop();
}

