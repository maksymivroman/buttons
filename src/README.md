> ### Pin state for button is `0` by default (pulled down)

### EEPROM struct configuration 
#### _reserved 1024 bytes_
#### _used 556 bytes_

```c++
struct EEPROMSETTINGS {
    bool serialEnabled;
    bool clientWebAccess;
    bool enableOtaUpdate;
    bool useDnsName ;
    bool useSound;
    bool useCustomHSsid;
    bool useTelegramIntegration;
    bool remoteTriggering;
    unsigned int fwVersion;
    char wifiSsid[256]{};
    char wifiPass[256]{};
    char hotspotSsid[32]{};
};
```
> ### Receiving configuration data via POST request
### Data structure (JSON)


```json
{
    "inputdata" : {
      "wifiname" : "SSID",
      "wifipass" : "PASS",
      
      "eventdata": {
        "${HOST1}": "${EVENT1}",
        "${HOST2}": "${EVENT2}",
        ...
        "${HOSTn}": "${EVENTn}"
      },
      
      "configuration" : {
        "serialEnabled": "1-true, 0-false",
        "clientWebAccess":"1-true, 0-false",
        "enableOtaUpdate": "1-true, 0-false",
        "useDnsName": "1-true, 0-false",
        "useSound": "1-true, 0-false",
        "useCustomHSsid": "1-true, 0-false",
        "useTelegramIntegration": "1-true, 0-false",
        "remoteTriggering": "1-true, 0-false",
        "fwVersion": "unsigned int",
        "wifiSsid": "char[256]",
        "wifiPass": "char[256]",
        "hotspotSsid": "char[32]"
      },
      "integration" : {
        "tToken" : "string",
        "tChanelID": number,
        "tPrefix": "string",
        "tSuffix": "string"
      }
    }
}
```