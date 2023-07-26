># event Button 2.0

### VERSION HISTORY
#### **Jan 2022** - First version was created for NoviSign digital signage as prototype of button for sending events to remote signage players.

#### **Feb 2023** - Work on second version was started.

### Main possibilities:
1. #### Send events via POST request
2. #### Forward incoming messages to Telegram messenger
3. #### LED and sound notification (RGB LED and piezoelectric sound generator)

>### 1. Send events via POST request:
#### Sending event is triggered by physical button (four button is connected in parallel)
#### HTTP method: POST. Content type: "application/json"
#### _HTTPS_ request not supported

>### 2. Forward incoming messages to Telegram messenger:
#### Incoming messages is handled by HTTP GET request `http://${host}/integration?data=${message data}`
#### Message can be combined with additional text
#### Telegram token, chanel ID and additional text can be set up from Setup page


>### 3. Trigger BUTTON remotely:
#### Handled by HTTP POST request with key `TRIGGER_BUTTON`
#### Boby: `form-data`
#### Key-value: `TRIGGER_BUTTON:` `AUTO` | `ON` | `OFF`


>### 4. LED and sound notification
#### RGB support
#### RGB PWM support _(currently not implemented)_
#### using of sound notification can be configured from Setup page

## EEPROM and data structure description [here](https://github.com/maksymivroman/buttons/blob/master/src/README.md)