# Event button 2.0

> ### Pin state for button is `0` by default (pulled down)

### EEPROM byte configuration (for future version)

|  _**byte**_   |          _**description**_           |
|:-------------:|:------------------------------------:|
|     **0**     |            Serial enabled            | 
|     **1**     | enable WEB Server on **Client** mode | 
|     **2**     |          enable OTA update           | 
|     **3**     |         enable DNS host name         | 
|     **4**     |        `RESERVED / NOT USED`         | 
|     **5**     |        `RESERVED / NOT USED`         | 
|     **6**     |        `RESERVED / NOT USED`         | 
|     **7**     |        `RESERVED / NOT USED`         | 
|     **8**     |        `RESERVED / NOT USED`         | 
|     **9**     |        `RESERVED / NOT USED`         | 
| **10 - 265**  |                 SSID                 | 
| **265 - 513** |               PASSWORD               | 
|     **-**     |     `RESERVED for HOTSPOT SSID`      | 
|     **-**     |     `RESERVED for HOTSPOT PASS`      | 