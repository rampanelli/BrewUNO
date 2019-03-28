#ifndef BrewService_h
#define BrewService_h

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#elif defined(ESP_PLATFORM)
#include <WiFi.h>
#include <AsyncTCP.h>
#endif

#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <IPAddress.h>
#include <AsyncJsonRequestWebHandler.h>
#include <AsyncJsonCallbackResponse.h>
#include <TimeLib.h>
#include <NtpClientLib.h>
#include <enum.h>
#include <MashService.h>
#include <BoilService.h>
#include <KettleHeaterService.h>
#include <BrewSettingsService.h>
#include <ActiveStatus.h>
#include <Pump.h>

#define START_BREW_SERVICE_PATH "/rest/startbrew"
#define STOP_BREW_SERVICE_PATH "/rest/stopbrew"
#define NEXT_STEP_SERVICE_PATH "/rest/nextstepbrew"
#define RESUME_BREW_SERVICE_PATH "/rest/resumebrew"
#define GET_ACTIVE_STATUS_SERVICE_PATH "/rest/getactivestatus"
#define CHANGE_BOIL_PERCENTAGE_SERVICE_PATH "/rest/changeboilpercentage"
#define START_BOIL_SERVICE_PATH "/rest/startboil"

#define APPLICATION_JSON_TYPE "application/json"
#define NPT_JSON_ERROR_MESSAGE "{ \"error\": true, \"message\":\"NTP server not reachable\"}"
#define INVALID_JSON_ERROR "Error validating json."

class BrewService
{
public:
  BrewService(AsyncWebServer *server,
              FS *fs,
              MashService *mashService,
              BoilService *boilService,
              BrewSettingsService *brewSettingsService,
              KettleHeaterService *kettleHeaterService,
              ActiveStatus *activeStatus,
              TemperatureService *temperatureService);

  ~BrewService();

  void loop();
  void begin();

private:
  FS *_fs;
  AsyncWebServer *_server;
  BoilService *_boilService;
  MashService *_mashService;
  BrewSettingsService *_brewSettingsService;
  KettleHeaterService *_kettleHeaterService;
  TemperatureService *_temperatureService;
  ActiveStatus *_activeStatus;

  void getActiveStatus(AsyncWebServerRequest *request);
  void startBrew(AsyncWebServerRequest *request);
  void stopBrew(AsyncWebServerRequest *request);
  void nextStep(AsyncWebServerRequest *request);
  void resumeBrew(AsyncWebServerRequest *request);
  void startBoil(AsyncWebServerRequest *request);
  void changeBoilPercentage(AsyncWebServerRequest *request, JsonVariant &json);
};
#endif