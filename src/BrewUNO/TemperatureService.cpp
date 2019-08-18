#include <BrewUNO/TemperatureService.h>

TemperatureService::TemperatureService(AsyncWebServer *server, FS *fs, DallasTemperature dallasTemperature, BrewSettingsService *brewSettingsService) : _server(server),
                                                                                                                                                        _fs(fs),
                                                                                                                                                        _dallasTemperature(dallasTemperature),
                                                                                                                                                        _brewSettingsService(brewSettingsService)
{
    _server->on(GET_SENSORS_SERVICE_PATH, HTTP_GET, std::bind(&TemperatureService::GetTemperatureAndAdress, this, std::placeholders::_1));
}

TemperatureService::~TemperatureService() {}

void TemperatureService::GetTemperatureAndAdress(AsyncWebServerRequest *request)
{
    request->send(200, APPLICATION_JSON_TYPE, GetSensorsJson());
}

String TemperatureService::GetFirstSensorAddress()
{
    DeviceAddress Thermometer;
    _dallasTemperature.getAddress(Thermometer, 0);
    return GetAddressToString(Thermometer);
}

String json = "";

String TemperatureService::GetSensorsJson()
{
    if (json == "")
        GetTemperatures("", "");
    return json;
}

Temperatures TemperatureService::GetTemperatures(String main, String sparge)
{
    Temperatures temps;
    temps.Main = 0;
    temps.Sparge = 0;
    DeviceAddress Thermometer;
    _dallasTemperature.requestTemperatures();
    String _json = "{ \"sensors\": [ ";
    String addr = "";
    for (int i = 0; i < DeviceCount; i++)
    {
        _dallasTemperature.getAddress(Thermometer, i);
        float temp = _dallasTemperature.getTempC(Thermometer);
        _json += "{ \"address\": \"" + GetAddressToString(Thermometer) + "\",\"value\": \"" + String(_dallasTemperature.getTempC(Thermometer)) + "\"}";
        if (i < DeviceCount - 1)
            _json += ',';

        addr = GetAddressToString(Thermometer);
        if (main != "" && addr == main)
            temps.Main = temp + _brewSettingsService->MainSensorOffset;

        if (sparge != "" && addr == sparge)
            temps.Sparge = temp + _brewSettingsService->SpargeSensorOffset;
    }
    json = _json + "]}";
    temps.Json = json;
    return temps;
}

String TemperatureService::GetAddressToString(DeviceAddress deviceAddress)
{
    String str = "";
    for (uint8_t i = 0; i < 8; i++)
    {
        if (deviceAddress[i] < 16)
            str += String(0, HEX);
        str += String(deviceAddress[i], HEX);
    }
    return str;
}