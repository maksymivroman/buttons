#ifndef EVENT_BUTTON_INDEX_HTML_PAGE_HPP
#define EVENT_BUTTON_INDEX_HTML_PAGE_HPP

#include <Arduino.h>

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta charset="UTF-8">
    <title>action button | Setup</title>
    <style>
        :root {
            --main: #333c45;
            --background: #eeeaea;
            --accent: #6e79d6;
        }

        html {
            font-size: 16px;
        }

        body {
            font-family: Arial, Helvetica, sans-serif;
            margin: 0;
            background: var(--background);
            display: flex;
            flex-direction: column;
            align-items: center;
            height: 100vh;
            overflow: auto;
        }

        a {
            color: #5151cb;
            cursor: pointer;
        }

        .header {
            max-height: 46px;
            background-color: var(--main);
            width: 100%;
            padding: 0 8px 0 8px;
            display: flex;
            align-items: center;
            flex: 1;
            justify-content: space-between;
            position: fixed;
            z-index: 100;
        }

        .header-text {
            color: white; font-weight: 200;
        }

        .label-m {
            font-size: .9rem !important;
        }

        .flex-center {
            display: flex;
            align-items: center;
        }

        .flex-col {
            display: flex;
            flex-flow: column;
        }

        .btn {
            color: #fff;
            display: inline-block;
            font-weight: 400;
            min-width: 110px;
            text-align: center;
            white-space: nowrap;
            vertical-align: middle;
            user-select: none;
            padding: .275rem .65rem;
            font-size: 0.9rem;
            line-height: 1.5;
            border-radius: 1rem;
            transition: color .15s ease-in-out, background-color .15s ease-in-out, border-color .15s ease-in-out, box-shadow .15s ease-in-out;
        }

        .btn:disabled {
            color: lightgray;
            background-color: gray;
            border: 1px solid gray;
        }

        .btn-light {
            background-color: #fff;
            border: 1px solid #fff;
            color: #000;
        }

        .btn-dark {
            background-color: #4d5156;
            border: 1px solid #4d5156;
            color: #fff;
        }

        .btn-accent {
            background-color: #6e79d6;
            border: 1px solid #6e79d6;
            color: #fff;
        }

        .btn-light:hover:not(:disabled) {
            background-color: #d0d0d0;
            cursor: pointer;
        }

        .btn-dark:hover:not(:disabled) {
            background-color: #494a4b;
            cursor: pointer;
        }

        .main {
            display: flex;
            flex: 1;
            width: 100%;
            padding: 1rem;
            flex-flow: column;
            align-items: flex-start;
        }

        .general-settings {
            display: flex;
            gap: 2rem;
            flex-wrap: wrap;
            flex-flow: row;
        }

        .settings-container {
            display: flex;
            flex-flow: column;
            gap: 8px;
            width: 100%;
            min-width: 10rem;
        }

        .info-status-container {
            display: flex;
            align-items: center;
            background: #fff;
            padding: 5px;
            min-width: 400px;
            border-radius: 16px;
            border: 1px solid #add0ad;
            gap: 6px;
            font-size: 14px;
        }

        .system-info-container {
            justify-content: center;
            width: 100%;
            gap: 16px;
            font-size: 12px;
        }

        .system-info {
            margin-right: 5px;
            color: #aba5a5;
        }

        .system-info-title {
            margin-right: 5px;
            color: white;
        }

        .feature-info {
            display: flex;
            margin-left: 12px;
            font-size: 14px;
        }

        .checkbox-form-field {
            display: flex;
            align-items: center;
        }

        .checkbox-form-field .modern-checkbox {
            appearance: none;
            background-color: #8f939b;
            border-radius: 72px;
            border-style: none;
            flex-shrink: 0;
            height: 20px;
            margin: 0;
            position: relative;
            width: 30px;
        }

        .checkbox-form-field .modern-checkbox::before {
            bottom: -6px;
            content: "";
            left: -6px;
            position: absolute;
            right: -6px;
            top: -6px;
        }

        .checkbox-form-field .modern-checkbox,
        .checkbox-form-field .modern-checkbox::after {
            transition: all 100ms ease-out;
        }

        .checkbox-form-field .modern-checkbox::after {
            background-color: #fff;
            border-radius: 50%;
            content: "";
            height: 14px;
            left: 3px;
            position: absolute;
            top: 3px;
            width: 14px;
        }

        .checkbox-form-field input[type=checkbox] {
            cursor: default;
        }

        .checkbox-form-field .modern-checkbox:hover {
            background-color: #818285;
            transition-duration: 0s;
        }

        .checkbox-form-field .modern-checkbox:checked {
            background-color: #6e79d6;
        }

        .checkbox-form-field .modern-checkbox:checked::after {
            background-color: #fff;
            left: 13px;
        }

        .checkbox-form-field :focus:not(.focus-visible) {
            outline: 0;
        }

        .checkbox-form-field .modern-checkbox:checked:hover:not(:disabled) {
            background-color: #535db3;
        }

        .checkbox-form-field .modern-checkbox:disabled {
            background-color: #adadaf;
        }

        .checkbox-form-field > label {
            align-self: flex-start;
            margin: 4px;
            font-size: 0.7rem;
        }

        .section-container {
            display: flex;
            flex-flow: column;
            margin-top: 2rem;
            align-items: flex-start;
            border-bottom: 4px solid var(--main);
            width: 100%;
        }

        .section-header {
            background-color: var(--main);
            color: white;
            padding: 4px 12px 2px 10px;
        }

        .content {
            display: flex;
            flex-flow: column;
            width: 100%;
            padding: 16px;
            overflow: auto;
        }

        .event-data {
            display: block;
            padding: 12px;
            background: inherit;
            width: 100%;
        }

        .modal {
            display: flex;
            justify-content: center;
            align-items: center;
            padding: 18px;
            background: rgba(4, 6, 14, 0.98);
            width: 100%;
            height: 100%;
            position: fixed;
            top: 0;
            left: 0;
            visibility: hidden;
            z-index: 111;
            gap: 42px;
        }

        table {
            background: inherit;
            width: 100%;
            height: fit-content;
            border-collapse: collapse;
        }

        th {
            border-bottom: 2px solid gray;
            padding: 8px 10px;
            text-align: left;
            font-size: 0.9rem;
            color: #333;
        }

        td {
            border-bottom: 1px solid lightgray;
            padding: 8px 10px;
            vertical-align: middle;
            font-size: 0.9rem;
        }

        .badge {
            display: inline-block;
            padding: 2px 6px;
            font-size: 0.75rem;
            font-weight: 600;
            border-radius: 3px;
            white-space: nowrap;
            margin: 2px 2px;
        }

        .badge-trigger {
            background-color: #e3f2fd;
            color: #0d47a1;
            border: 1px solid #bbdefb;
        }

        .badge-action {
            background-color: #e8f5e9;
            color: #1b5e20;
            border: 1px solid #c8e6c9;
        }

        .mt-m { margin-top: 16px; }
        .mt-l { margin-top: 36px; }

        input[type=checkbox]:disabled + label {
            color: darkgray;
        }

        *, ::after, ::before {
            box-sizing: border-box;
        }

        button, select {
            text-transform: none;
        }

        .btn-min-w {min-width: 100px;}

        .led-grid-layout {
            display: grid;
            grid-template-columns: repeat(auto-fill, minmax(260px, 1fr));
            width: 80%;
        }

        .color-picker-button {
            display: flex;
            align-items: center;
            gap: 8px;
        }

        .rgb-info-square {
            position: relative;
            outline: 1px solid #696565;
            border-radius: 2px;
            display: flex;
            align-items: center;
            height: 18px;
            width: 18px;
            justify-content: center;
            overflow: hidden;
            user-select: none;
            background: gray;
        }

        .marked-flag.rgb-info-square {
            opacity: .2;
        }

        .marked-flag.rgb-info-square::before {
            content: '';
            top: 0;
            left: 0;
            position: absolute;
            width: 26px;
            height: 1px;
            background-color: #a6132f;
            transform: rotate(45deg);
            transform-origin: top left;
        }

        .form-field {
            background: #fff;
            display: flex;
            flex-flow: column;
            border-radius: 4px;
            padding: 0;
        }

        .form-field > label {
            align-self: flex-start;
            margin: 2px;
            color: #365781;
            font-size: 0.7rem;
        }

        .form-field > input,
        .form-field > select,
        .form-field > textarea{
            background: transparent;
            border: none;
            outline: none;
        }

        .form-field > input:focus,
        .form-field > select:focus,
        .form-field > textarea:focus{
            box-shadow: inset 0 -2px 0 #4a90e2;
        }

        @media screen and (max-width: 800px) {
            .--hidden-sm {
                display: none;
            }
        }
</style>
</head>
<body>

    <div class="header">
        <div class="flex-center">
            ^DEVICE_LOGO^
            <div style="display: flex; align-items: baseline; margin-left: 12px;">
                <h2 class="header-text" >action button</h2>
                <h2 class="header-text --hidden-sm">&nbsp|&nbsp</h2>
                <h4 class="header-text --hidden-sm">Setup</h4>
            </div>
        </div>
        <div class="flex-center" style="gap: 10px;">
            <button id="updateBtn" type="button" class="btn btn-light" onclick="location.href='/update'">FW Update</button>
            <button type="button" class="btn btn-light" onclick="find()">Find me</button>
            <button type="button" class="btn btn-light" onclick="location.href='/logs'">Logs</button>
            <button type="button" class="btn btn-accent" onclick="saveSettings()">Save & Reboot</button>
        </div>
    </div>

    <div class="main" style="margin-top: 46px;">
        <div class="general-settings">
            <div class="settings-container">
                <div class="form-field">
                    <label for="networks">Available networks</label>
                    <select id="networks"></select>
                </div>

                <div class="form-field">
                    <label for="wifiSsid">SSID</label>
                    <input id="wifiSsid">
                </div>

                <div class="form-field">
                    <label for="wifiPass">PASSWORD</label>
                    <input id="wifiPass" type="password">
                </div>
            </div>

            <div class="settings-container">
                <div class="form-field">
                    <label for="wiFiMode">WiFi mode</label>
                    <select id="wiFiMode"></select>
                </div>
                <div class="form-field">
                    <label for="timezone">Time zone</label>
                    <select id="timezone"></select>
                </div>
                <div class="form-field">
                    <label for="loggerLevel">Debug logs</label>
                    <select id="loggerLevel"></select>
                </div>
            </div>

            <div class="settings-container">
                <div class="form-field">
                    <label for="hotspotSsid">Device ID</label>
                    <input maxlength="32" type="text" id="hotspotSsid">
                </div>

                <div class="checkbox-form-field">
                    <input type="checkbox" id="useHotspotSsid" class="modern-checkbox">
                    <label for="useHotspotSsid">Device ID for SSID</label>
                </div>
            </div>

            <div class="settings-container">
                <div class="checkbox-form-field">
                    <input type="checkbox" id="useDnsName" class="modern-checkbox" disabled>
                    <label for="useDnsName">DNS host name (btn.iot)</label>
                </div>

                <div class="checkbox-form-field">
                    <input type="checkbox" id="useSound" class="modern-checkbox">
                    <label for="useSound">Sound notification</label>
                </div>

                ^CLIENT_MODE_OPTIONS^
            </div>
        </div>
    </div>

    <div class="section-container">
        <span class="section-header">Actions</span>
    </div>

    <div class="flex-col" style="width: 100%;">
        <button type="button" style="align-self: flex-end; margin: 1rem;" class="btn btn-dark" onclick="location.href='/editor'">Edit</button>
            <div class="event-data">
                <table id="dataTable">
                    <thead>
                    <tr>
                        <th style="width: 20%;">Name</th>
                        <th style="width: 25%;">Host</th>
                        <th style="width: 25%;">Payload</th>
                        <th style="width: 15%;">Triggers</th>
                        <th style="width: 15%;">Actions</th>
                    </tr>
                    </thead>
                    <tbody id="eventsListBody">
                    <!-- Read-only events list -->
                    </tbody>
                </table>
            </div>
    </div>

    <div class="section-container">
        <span class="section-header">Serial port actions</span>
    </div>

    <div class="main">
        <div class="checkbox-form-field">
            <input type="checkbox" id="serialEvents" class="modern-checkbox">
            <label for="serialEvents" class="label-m">Enable data via Serial(115200 8-N-1)</label>
        </div>
        <span class="mt-m">Warning! Serial logs will be disabled</span>
    </div>


    <div class="section-container">
        <span class="section-header">Switch mode</span>
    </div>
    <div class="main" style="gap: .75rem;">
        <div class="checkbox-form-field">
            <input type="checkbox" id="saveLastState" class="modern-checkbox">
            <label for="saveLastState" class="label-m">Enable Switch mode</label>
        </div>
        <div class="checkbox-form-field">
            <input type="checkbox" id="restoreLastStateOnLoad" class="modern-checkbox">
            <label for="restoreLastStateOnLoad" class="label-m">Restore last state on load</label>
        </div>
        <div class="feature-info">
            <span>Switch mode allows you to perform action depending on the Button state - ON/OFF</span>
        </div>
        <div class="info-status-container">
            <svg xmlns="http://www.w3.org/2000/svg" color="#add0ad" width="20px" height="20px" viewBox="0 0 24 24"><title>info</title><g fill="none" stroke="currentColor" stroke-width="2"><circle cx="12" cy="12" r="10"/><path stroke-linecap="round" d="M12 7h.01"/><path stroke-linecap="round" stroke-linejoin="round" d="M10 11h2v5m-2 0h4"/></g></svg>
            <span>Switch state:</span>
            <span style="font-weight: 600;" id="toggleState"></span>
        </div>
    </div>

    <div class="section-container">
        <span class="section-header">Keystore</span>
    </div>
    <div class="main" style="gap: .75rem;">
        <div class="checkbox-form-field">
            <input type="checkbox" id="keystoreEnabled" class="modern-checkbox">
            <label for="keystoreEnabled" class="label-m">Enable keystore</label>
        </div>
        <div class="checkbox-form-field">
            <input type="checkbox" id="sendEventOnKeystoreUpdate" class="modern-checkbox">
            <label for="sendEventOnKeystoreUpdate" class="label-m">Perform action on keystore update</label>
        </div>
        <div class="checkbox-form-field">
            <input type="checkbox" id="delaySendEvents" class="modern-checkbox">
            <label for="delaySendEvents" class="label-m">Handle delay before perform action ('delay' param)</label>
        </div>
        <div class="feature-info flex-col">
            <div class="flex-center">
                <span>Update keystore URL: </span>
                <strong style="margin-left: 12px;">http://<span class="device-ip">192.168.4.1</span>/keystore?delay=DELAY&key1=DATA&key2=DATA&key3...</strong>
            </div>
            <div class="flex-col">
                <span>Use <strong>$key$</strong>  expression in action payload data, and it will be replaced by corresponding value</span>
                <span><strong>delay</strong> - if provided, action will be initiated after delay, ms</span>
            </div>
        </div>
        <div class="info-status-container">
            <svg xmlns="http://www.w3.org/2000/svg" color="#add0ad" width="20px" height="20px" viewBox="0 0 24 24"><title>info</title><g fill="none" stroke="currentColor" stroke-width="2"><circle cx="12" cy="12" r="10"/><path stroke-linecap="round" d="M12 7h.01"/><path stroke-linecap="round" stroke-linejoin="round" d="M10 11h2v5m-2 0h4"/></g></svg>
            <span>Stored keys:</span>
            <span style="font-weight: 600;" id="ksKeys"></span>
        </div>
    </div>

    <div class="section-container">
        <span class="section-header">Remote triggering</span>
    </div>
    <div class="main" style="gap: .75rem;">
        <div class="checkbox-form-field">
            <input type="checkbox" id="remoteTriggering" class="modern-checkbox">
            <label for="remoteTriggering" class="label-m">Trigger Button remotely</label>
        </div>

        <div class="feature-info flex-col">
            <span style="margin-bottom: 12px;">Trigger button with POST request:</span>
            <div class="feature-info">
                <strong>Content-Type: </strong>
                <span>application/form-data</span>
            </div>
            <div class="feature-info">
                <strong>Key/Value: </strong>
                <span>TRIGGER_BUTTON: AUTO</span>
            </div>
        </div>
    </div>

    <div class="section-container">
        <span class="section-header">Income LED and Sound notifications</span>
    </div>
    <div class="main" style="gap: .75rem;">
        <div class="checkbox-form-field">
            <input type="checkbox" id="remoteStateChange" class="modern-checkbox">
            <label for="remoteStateChange" class="label-m">Switch LED/Sound notifications remotely</label>
        </div>

        <div class="feature-info flex-col">
            <span style="margin-bottom: 12px;">Toggle button LED/Sound using GET request:</span>
            <div class="feature-info">
                <span>URL:</span>
                <strong>http://<span class="device-ip">192.168.4.1</span>/external?led=${on/off}&beep={on/off}</strong>
            </div>
        </div>
    </div>

    <div class="section-container">
        <span class="section-header">Dev Reports</span>
    </div>
    <div class="main" style="gap: .75rem;">
        <div class="checkbox-form-field">
            <input type="checkbox" id="statisticEnabled" class="modern-checkbox">
            <label for="statisticEnabled" class="label-m">Enable Dev Reports</label>
        </div>
        <div class="flex-center" style="gap: .75rem;">
            <div class="form-field">
                <label for="statisticApi">API url</label>
                <input style="min-width: 300px;" maxlength="255" type="text" id="statisticApi">
            </div>
            <div class="form-field" style="min-width: 200px;">
                <label for="statisticLevel">Report Type</label>
                <select id="statisticLevel"></select>
            </div>
        </div>
    </div>

    <div class="section-container">
        <span class="section-header">LED configuration</span>
    </div>
    <div class="main">
        <div class="checkbox-form-field" style="margin-bottom: 12px;">
            <input type="checkbox" id="overrideLedConfig" class="modern-checkbox">
            <label for="overrideLedConfig" class="label-m">Custom LED Actions</label>
        </div>
        <div class="led-grid-layout" id="led-container">
            <!--led controls-->
            no actions defined
        </div>
    </div>

    <div class="section-container">
        <span class="section-header">Onboard Server</span>
    </div>
    <div class="main" style="gap: 8px;">
        <div class="checkbox-form-field">
            <input type="checkbox" id="customServer" class="modern-checkbox">
            <label for="customServer" class="label-m">Enable Onboard server</label>
        </div>
        <div class="flex-center">
            <span>Server Path: &nbsp</span>
            <a id="serverPathLink" target="_blank" href="#"></a>
            <svg xmlns="http://www.w3.org/2000/svg" color="#5151cb" width="1em" height="1em" viewBox="0 0 24 24"><title>open</title><path fill="none" stroke="currentColor" stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M10 4H6a2 2 0 0 0-2 2v12a2 2 0 0 0 2 2h12a2 2 0 0 0 2-2v-4m-8-2l8-8m0 0v5m0-5h-5"/></svg>
            <button style="margin-left: 16px;" type="button" onclick="location.href='/server/editor'" class="btn btn-dark">Open Editor</button>
        </div>
    </div>

    <div class="content">
        <div class="modal" id="successModel">
            <svg xmlns="http://www.w3.org/2000/svg" xml:space="preserve" width="200px" height="200px"
                 style="fill-rule:evenodd; clip-rule:evenodd" viewBox="0 0 200 200">
                    <g id="Layer_x0020_1">
                        <polygon fill="#5B5A50"
                                 points="193.05,122.02 198.24,124.05 99.92,161.86 2.62,124.46 7.84,122.52 5.7,121.72 0.16,123.79 0.16,142.69 100.09,180.95 200,142.76 200,123.79 194.3,121.66 "/>
                        <polygon fill="#5B5B5B"
                                 points="100.7,54.44 197.22,85.03 101.01,120.73 2.98,84.29 100.7,54.44 100.7,52.91 0,83.98 0,119.58 99.92,157.05 199.85,119.58 199.85,84.91 100.7,52.91 "/>
                        <polygon id="upload-box_1" fill="#E53324"
                                 points="99.92,180.91 0.16,142.69 0.16,161.73 100.09,200 200,161.81 200,142.77 "/>
                        <polygon fill="#898989" points="2.98,84.29 101.01,120.73 197.22,85.03 100.7,54.44 "/>
                    </g>
                </svg>

            <div class="flex-col">
                <h2 style="color: #b9b9b9;" id="dialogMessageTitle">Please wait...</h2>
                <span style="color: #b9b9b9; font-weight: 200" id="dialogMessage">Settings saved. Rebooting...</span>
                <button disabled id="refreshBtn" type="button" class="btn btn-accent mt-l"
                        onclick="location.reload()">OK
                </button>
            </div>
        </div>

    </div>

    <div class="main" style="background: #475d72;">
        <div class="flex-center system-info-container">
            <div class="flex-center">
                <span class="system-info-title">HW version:</span>
                <span class="system-info" id="hwVersion"></span>
            </div>
            <div class="flex-center">
                <span class="system-info-title">FW version:</span>
                <span class="system-info" id="fwVersion"></span>
            </div>
            <div class="flex-center">
                <span class="system-info-title">Free HEAP:</span>
                <span class="system-info" id="heap"></span>
            </div>
            <div class="flex-center">
                <span class="system-info-title">MAC:</span>
                <span class="system-info" id="mac"></span>
            </div>
            <div class="flex-center" style="align-items: center; gap: 4px">
                <div id="rFlag" class="rgb-info-square"><h5 style="color: #e50c0c;">R</h5></div>
                <div id="gFlag" class="rgb-info-square"><h5 style="color: #077307;">G</h5></div>
                <div id="bFlag" class="rgb-info-square"><h5 style="color: #4949ff;">B</h5></div>
            </div>
        </div>
    </div>

</body>
</html>

<script>
    function httpPOST(data, resultTitle, resultMessage, canClose, postprocess) {
        document.getElementById('successModel').style.visibility = 'visible';
        const dialogMessageTitle = document.getElementById('dialogMessageTitle');
        const dialogMessage = document.getElementById('dialogMessage');
        dialogMessageTitle.innerText = resultTitle;
        dialogMessage.innerText = "Saving...";
        const srvURL = window.location.protocol + "//" + window.location.host + "/";
        const httpRequest = new XMLHttpRequest();
        httpRequest.open("POST", srvURL, true);
        httpRequest.setRequestHeader("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
        httpRequest.send(data);
        httpRequest.responseType = 'text';
        httpRequest.onreadystatechange = function () {
            if (httpRequest.readyState === httpRequest.DONE) {
                if (httpRequest.status === 200) {
                    dialogMessage.innerText = resultMessage;
                    if (canClose) {
                        document.getElementById('refreshBtn').disabled = false;
                    }
                    postprocess();
                } else {
                    dialogMessageTitle.innerText = "Failed";
                    dialogMessage.innerText = "Ops! Something went wrong...";
                    document.getElementById('refreshBtn').innerText = " Close";
                    document.getElementById('refreshBtn').disabled = false;
                }
            }
        };
    }

    function clearEeprom() {
        httpPOST("CLEAR_EEPROM", "EEPROM cleared!");
    }

    function find() {
        const srvURL = window.location.protocol + "//" + window.location.host + "/";
        const httpRequest = new XMLHttpRequest();
        httpRequest.open("POST", srvURL, true);
        httpRequest.setRequestHeader("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
        httpRequest.send("FIND=find");
        httpRequest.responseType = 'text';
        httpRequest.onreadystatechange = function () {
        };
    }

    let ledConfig = {};

    function collectSettingsPayload() {
        const name = document.getElementById('wifiSsid')?.value || document.getElementById('wifiname')?.value || '';
        const pass = document.getElementById('wifiPass')?.value || document.getElementById('wifipass')?.value || '';
        const hSsid = document.getElementById('hotspotSsid')?.value || '';

        const clientWebAccess = document.getElementById('clientWebAccess') ? Boolean(document.getElementById('clientWebAccess')?.checked) : Boolean(config.clientWebAccess);
        const enableOtaUpdate = document.getElementById('enableOtaUpdate') ? Boolean(document.getElementById('enableOtaUpdate')?.checked) : Boolean(config.enableOtaUpdate);

        const loggerLevel = Number(document.getElementById('loggerLevel')?.selectedIndex ?? 0);
        const wiFiMode = Number(document.getElementById('wiFiMode')?.selectedIndex ?? 0);

        const statisticEnabled = Boolean(document.getElementById('statisticEnabled')?.checked);
        const statisticLevel = Number(document.getElementById('statisticLevel')?.selectedIndex ?? 0);

        const useSound = Boolean(document.getElementById('useSound')?.checked);
        const customHSsid = Boolean(document.getElementById('useHotspotSsid')?.checked);
        const remoteTriggering = Boolean(document.getElementById('remoteTriggering')?.checked);
        const serialEvents = Boolean(document.getElementById('serialEvents')?.checked);
        const saveLastState = Boolean(document.getElementById('saveLastState')?.checked);
        const restoreLastStateOnLoad = Boolean(document.getElementById('restoreLastStateOnLoad')?.checked);
        const remoteStateChange = Boolean(document.getElementById('remoteStateChange')?.checked);
        const keystoreEnabled = Boolean(document.getElementById('keystoreEnabled')?.checked);
        const sendEventOnKeystoreUpdate = Boolean(document.getElementById('sendEventOnKeystoreUpdate')?.checked);
        const delaySendEvents = Boolean(document.getElementById('delaySendEvents')?.checked);
        const overrideLedConfig = Boolean(document.getElementById('overrideLedConfig')?.checked);
        const customServer = Boolean(document.getElementById('customServer')?.checked);
        const timezone = Number(document.getElementById('timezone')?.selectedIndex ?? 0);

        const statApi = document.getElementById('statisticApi')?.value || '';

        return {
            wifiSsid: name,
            wifiPass: pass,
            hotspotSsid: hSsid,
            customHSsid,
            statisticApi: statApi,
            clientWebAccess,
            enableOtaUpdate,
            loggerLevel,
            wiFiMode,
            statisticEnabled,
            statisticLevel,
            useSound,
            remoteTriggering,
            remoteStateChange,
            saveLastState,
            restoreLastStateOnLoad,
            keystoreEnabled,
            sendEventOnKeystoreUpdate,
            delaySendEvents,
            overrideLedConfig,
            serialEvents,
            customServer,
            timezone,
            ledConfig
        };
    }

    async function saveSettings() {
        const dialog = document.getElementById('successModel');
        const dialogMessageTitle = document.getElementById('dialogMessageTitle');
        const dialogMessage = document.getElementById('dialogMessage');
        const refreshBtn = document.getElementById('refreshBtn');

        dialog.style.visibility = 'visible';
        dialogMessageTitle.innerText = "Save settings";
        dialogMessage.innerText = "Saving settings...";
        refreshBtn.disabled = true;

        const payload = collectSettingsPayload();

        try {
            const response = await fetch('/settings', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify(payload)
            });

            if (response.ok) {
                const data = await response.json().catch(() => ({}));
                dialogMessage.innerText = data.result || "Settings saved. Rebooting...";
                refreshBtn.disabled = false;
                refreshBtn.innerText = "OK";
            } else {
                const errData = await response.json().catch(() => ({}));
                const errMsg = errData.error || `Server returned error (${response.status})`;
                dialogMessageTitle.innerText = "Failed";
                dialogMessage.innerText = errMsg;
                refreshBtn.innerText = "Close";
                refreshBtn.disabled = false;
            }
        } catch (err) {
            console.error('Error saving settings:', err);
            dialogMessageTitle.innerText = "Failed";
            dialogMessage.innerText = "Network error: Could not save settings";
            refreshBtn.innerText = "Close";
            refreshBtn.disabled = false;
        }
    }

    async function loadData() {
        try {
            const statusRes = await fetch('/status');
            if (statusRes.ok) {
                const statusJson = await statusRes.json();
                if (statusJson && statusJson.result) {
                    renderStatus(statusJson.result);
                }
            }

            const settingsRes = await fetch('/settings');
            if (settingsRes.ok) {
                const settingsData = await settingsRes.json();
                if (settingsData && settingsData.result) {
                    config = settingsData.result;
                    renderSettings(config);
                }
            }

            const eventsRes = await fetch('/actions');
            if (eventsRes.ok) {
                const eventsJson = await eventsRes.json();
                if (eventsJson && eventsJson.result) {
                    populateTable(eventsJson.result);
                } else if (Array.isArray(eventsJson)) {
                    populateTable(eventsJson);
                }
            }

            const networksRes = await fetch('/networks');
            if (networksRes.ok) {
                const networksJson = await networksRes.json();
                if (networksJson && networksJson.result) {
                    const list = Array.isArray(networksJson.result) ? networksJson.result : (networksJson.result.networks || []);
                    if (list.length > 0) {
                        renderWifiList(list);
                    } else {
                        setTimeout(async () => {
                            try {
                                const retryRes = await fetch('/networks');
                                if (retryRes.ok) {
                                    const retryJson = await retryRes.json();
                                    const retryList = Array.isArray(retryJson.result) ? retryJson.result : (retryJson.result.networks || []);
                                    if (retryList.length > 0) renderWifiList(retryList);
                                }
                            } catch (_) {}
                        }, 2500);
                    }
                }
            }
        } catch (e) {
            console.error('Error loading settings, actions, networks, or status:', e);
        }
    }

    function renderStatus(status) {
        if (!status) return;

        if (document.getElementById('fwVersion')) document.getElementById('fwVersion').textContent = status.fwVersion || '';
        if (document.getElementById('hwVersion')) document.getElementById('hwVersion').textContent = status.hwVersion || '';
        if (document.getElementById('mac')) document.getElementById('mac').textContent = status.mac || '';
        if (document.getElementById('heap')) document.getElementById('heap').textContent = status.heap !== undefined ? status.heap : '';
        if (document.getElementById('ksKeys')) document.getElementById('ksKeys').textContent = status.ksKeys !== undefined ? status.ksKeys : '';
        if (document.getElementById('toggleState')) document.getElementById('toggleState').textContent = status.toggleState || '';

        document.querySelectorAll('.device-ip').forEach(el => {
            el.textContent = status.ip || window.location.host;
        });

        if (status.rgbFlags) {
            const rFlag = document.getElementById('rFlag');
            const gFlag = document.getElementById('gFlag');
            const bFlag = document.getElementById('bFlag');
            if (rFlag) rFlag.className = 'rgb-info-square ' + (status.rgbFlags.rDisabled ? 'marked-flag' : '');
            if (gFlag) gFlag.className = 'rgb-info-square ' + (status.rgbFlags.gDisabled ? 'marked-flag' : '');
            if (bFlag) bFlag.className = 'rgb-info-square ' + (status.rgbFlags.bDisabled ? 'marked-flag' : '');
        }
    }

    function renderWifiList(networks) {
        const select = document.getElementById('networks');
        if (!select || !networks) return;
        select.innerHTML = '<option value="" disabled selected>Select network...</option>';
        networks.forEach(net => {
            const ssid = typeof net === 'string' ? net : (net.ssid || '');
            if (ssid) {
                const opt = document.createElement('option');
                opt.value = ssid;
                const rssi = typeof net === 'object' && net.rssi !== undefined ? ` (${net.rssi} dBm)` : '';
                const lock = typeof net === 'object' && net.secure ? ' 🔒' : '';
                opt.textContent = `${ssid}${rssi}${lock}`;
                select.appendChild(opt);
            }
        });
    }

    function renderOptions(selectId, optionsList) {
        const select = document.getElementById(selectId);
        if (!select || !optionsList || !optionsList.length) return;
        select.innerHTML = '';
        optionsList.forEach(opt => {
            const optionEl = document.createElement('option');
            optionEl.value = opt.value;
            optionEl.textContent = opt.label;
            select.appendChild(optionEl);
        });
    }

    function renderSettings(cfg) {
        if (!cfg) return;

        if (cfg.options) {
            renderOptions('loggerLevel', cfg.options.loggerLevels);
            renderOptions('wiFiMode', cfg.options.wiFiModes);
            renderOptions('timezone', cfg.options.timezones);
            renderOptions('statisticLevel', cfg.options.statisticLevels);
        }

        const ssidEl = document.getElementById('wifiSsid') || document.getElementById('wifiname');
        if (ssidEl && cfg.wifiSsid !== undefined) {
            ssidEl.value = cfg.wifiSsid;
        }
        const passEl = document.getElementById('wifiPass') || document.getElementById('wifipass');
        if (passEl && cfg.wifiPass !== undefined) {
            passEl.value = cfg.wifiPass;
        }
        if (document.getElementById('statisticApi') && cfg.statisticApi !== undefined) {
            document.getElementById('statisticApi').value = cfg.statisticApi;
        }
        if (document.getElementById('hotspotSsid') && cfg.hotspotSsid !== undefined) {
            document.getElementById('hotspotSsid').value = cfg.hotspotSsid;
        }

        const checkboxSettings = [
            'useSound', 'statisticEnabled',
            'saveLastState', 'overrideLedConfig', 'serialEvents', 'customServer',
            'remoteTriggering', 'restoreLastStateOnLoad', 'keystoreEnabled',
            'remoteStateChange', 'sendEventOnKeystoreUpdate', 'delaySendEvents',
            'clientWebAccess', 'enableOtaUpdate'
        ];

        for (const key of checkboxSettings) {
            const el = document.getElementById(key);
            if (el && cfg[key] !== undefined) {
                el.checked = Boolean(cfg[key]);
            }
        }

        const hotspotCheckbox = document.getElementById('useHotspotSsid');
        if (hotspotCheckbox && cfg.customHSsid !== undefined) {
            hotspotCheckbox.checked = Boolean(cfg.customHSsid);
        }

        if (document.getElementById('loggerLevel') && cfg.loggerLevel !== undefined) {
            document.getElementById('loggerLevel').value = cfg.loggerLevel;
        }
        if (document.getElementById('wiFiMode') && cfg.wiFiMode !== undefined) {
            document.getElementById('wiFiMode').value = cfg.wiFiMode;
        }
        if (document.getElementById('timezone') && cfg.timezone !== undefined) {
            document.getElementById('timezone').value = cfg.timezone;
        }
        if (document.getElementById('statisticLevel') && cfg.statisticLevel !== undefined) {
            document.getElementById('statisticLevel').value = cfg.statisticLevel;
        }

        if (!cfg.enableOtaUpdate && !document.getElementById('enableOtaUpdate')) {
            const updateBtn = document.getElementById('updateBtn');
            if (updateBtn) updateBtn.remove();
        }

        if (cfg.ledConfig) {
            const ledContainer = document.getElementById('led-container');
            ledConfig = {...cfg.ledConfig};
            initLedControls(ledContainer, ledConfig);
        }

        if (cfg.serverPath) {
            const serverLink = document.getElementById('serverPathLink');
            if (serverLink) {
                const path = cfg.serverPath.startsWith('/') ? cfg.serverPath : '/' + cfg.serverPath;
                serverLink.href = path;
                serverLink.textContent = window.location.host + path;
            }
        }
    }

    let config;

    function populateTable(events) {
        const tbody = document.getElementById('eventsListBody') || document.querySelector('#dataTable tbody');
        if (!tbody) return;
        tbody.innerHTML = '';
        if (!events || (Array.isArray(events) && events.length === 0)) {
            tbody.innerHTML = '<tr><td colspan="5" style="text-align: center; color: #888; padding: 20px;">No events configured. Click "Edit" to configure events.</td></tr>';
            return;
        }

        if (Array.isArray(events)) {
            events.forEach(ev => {
                const name = escapeHtml(ev.name || 'Event');
                const host = escapeHtml(ev.host || '-');
                const payload = escapeHtml(ev.payload || '-');
                const triggers = Array.isArray(ev.triggers) && ev.triggers.length > 0
                    ? ev.triggers.map(t => `<span class="badge badge-trigger">${escapeHtml(t)}</span>`).join(' ')
                    : '-';
                const actions = Array.isArray(ev.actions) && ev.actions.length > 0
                    ? ev.actions.map(a => `<span class="badge badge-action">${escapeHtml(a)}</span>`).join(' ')
                    : '-';
                const disabledStyle = ev.enabled === false ? 'opacity: 0.55;' : '';
                const row = `
                    <tr style="${disabledStyle}">
                        <td><strong>${name}</strong></td>
                        <td style="word-break: break-all;">${host}</td>
                        <td style="font-family: monospace; font-size: 0.85rem; word-break: break-all;">${payload}</td>
                        <td>${triggers}</td>
                        <td>${actions}</td>
                    </tr>
                `;
                tbody.insertAdjacentHTML('beforeend', row);
            });
        }
    }

    function escapeHtml(text) {
        if (!text) return '';
        return String(text)
            .replace(/&/g, "&amp;")
            .replace(/</g, "&lt;")
            .replace(/>/g, "&gt;")
            .replace(/"/g, "&quot;")
            .replace(/'/g, "&#039;");
    }

    const networkSelectCtrl = document.getElementById('networks');
    if (networkSelectCtrl) {
        networkSelectCtrl.addEventListener('change', () => {
            const ssidInput = document.getElementById('wifiSsid') || document.getElementById('wifiname');
            if (ssidInput) ssidInput.value = networkSelectCtrl.value;
        });
    }

    loadData();

    function changeButtonColor(fn) {
        setTimeout(()=>{
            document.querySelector('#upload-box_1').style.fill = '#3c5dd7';
        }, 4000);
        setTimeout(()=>{
            document.querySelector('#upload-box_1').style.fill = '#66B65F';
            fn();
        }, 8000);
    }

    function checkButtonRestart() {
        return new Promise((resolve, reject) => {
            const srvURL = window.location.protocol + "//" + window.location.host + "/";
            const httpRequest = new XMLHttpRequest();
            httpRequest.onreadystatechange = function () {
                if (httpRequest.readyState === 4) {
                    if (httpRequest.status === 200) {
                        resolve(httpRequest.responseText);
                    } else {
                        reject('err');}
                }
            };
            httpRequest.timeout = 1000;
            httpRequest.responseType = 'text';
            httpRequest.open("POST", srvURL, true);
            httpRequest.setRequestHeader("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
            httpRequest.send("ID=id");
        });
    }

    function startCheckRestart() {
        checkButtonRestart().then(
            _ => window.location.reload()
        ).catch(_=> setTimeout(startCheckRestart,2000));
    }

    function initLedControls(container, ledRBGData) {
        const ledColorMap = {
            ledIdleDefault: 'Idle default',
            ledIdlePressed: 'Idle on Switch ON',
            ledLoading: 'Loading progress',
            ledWarn: 'Warning',
            ledDone: 'Action done',
            ledKeystoreUpdate: 'Keystore action',
            ledSendEvents: 'Actions progress',
            ledExternalInterface: 'External LED/Sound'
        }
        if (Object.keys(ledRBGData).length) {
            container.innerText = '';
            for (const key in ledRBGData) {
                const ctr = `<div class="flex-center">
                    <input type="color" style="visibility: hidden;" id="${key}"value="${ledRBGData[key]}">
                    <label class="color-picker-button" for="${key}">
                    <svg for="ledIdleDefault" xmlns="http://www.w3.org/2000/svg" xml:space="preserve" width="36px" height="36px" viewBox="0 3 24 24">
                        <g> <polygon style="fill:#475d72" points="12.08,6.53 23.67,10.2 12.12,14.49 0.36,10.11 12.08,6.53 12.08,6.35 0,10.08 0,14.35 11.99,18.85 23.98,14.35 23.98,10.19 12.08,6.35 "></polygon>
                            <polygon style="fill:${ledRBGData[key]}" points="11.99,21.71 0.02,17.12 0.02,19.41 12.01,24 24,19.42 24,17.13 "></polygon>
                            <polygon style="fill:#475d72" points="0.36,10.11 12.12,14.49 23.67,10.2 12.08,6.53 "></polygon>
                        </g>
                    </svg>${ledColorMap[key] || key}</label></div>`;
                container.insertAdjacentHTML('beforeend', ctr);
                const ctrl = document.getElementById(key);
                ctrl.addEventListener("change", () => ledConfig[key] = ctrl.value);
            }
        }
    }

</script>
)rawliteral";

const char logs_page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta charset="UTF-8">
    <title>action button | Logs</title>
    <style>
        :root {
            --main: #333c45;
            --background: #eeeaea;
            --accent: #6e79d6;
        }

        *, ::after, ::before {
            box-sizing: border-box;
        }

        html {
            font-size: 16px;
        }

        body {
            font-family: Arial, Helvetica, sans-serif;
            margin: 0;
            background: var(--background);
            display: flex;
            flex-direction: column;
            align-items: center;
            height: 100vh;
            overflow: auto;
        }

        .header {
            max-height: 46px;
            background-color: var(--main);
            width: 100%;
            padding: 0 8px 0 8px;
            display: flex;
            align-items: center;
            flex: 1;
            justify-content: space-between;
            position: fixed;
            z-index: 100;
        }

        .header-text {
            color: white;
            font-weight: 200;
        }

        .flex-center {
            display: flex;
            align-items: center;
        }

        .flex-col {
            display: flex;
            flex-flow: column;
        }

        .btn {
            color: #fff;
            display: inline-flex;
            align-items: center;
            justify-content: center;
            gap: 6px;
            font-weight: 400;
            min-width: 110px;
            text-align: center;
            white-space: nowrap;
            vertical-align: middle;
            user-select: none;
            padding: .275rem .65rem;
            font-size: 0.9rem;
            line-height: 1.5;
            border-radius: 1rem;
            cursor: pointer;
            transition: color .15s ease-in-out, background-color .15s ease-in-out, border-color .15s ease-in-out, box-shadow .15s ease-in-out;
        }

        .btn-sm {
            min-width: auto;
            padding: .25rem .6rem;
            font-size: 0.85rem;
            border-radius: 0.75rem;
        }

        .btn:disabled {
            color: lightgray;
            background-color: gray;
            border: 1px solid gray;
            cursor: not-allowed;
        }

        .btn-light {
            background-color: #fff;
            border: 1px solid #fff;
            color: #000;
        }

        .btn-light:hover:not(:disabled) {
            background-color: #d0d0d0;
            border-color: #d0d0d0;
            cursor: pointer;
        }

        .btn-dark {
            background-color: #4d5156;
            border: 1px solid #4d5156;
            color: #fff;
        }

        .btn-dark:hover:not(:disabled) {
            background-color: #3b3e42;
            border-color: #3b3e42;
            cursor: pointer;
        }

        .btn-accent {
            background-color: var(--accent);
            border: 1px solid var(--accent);
            color: #fff;
        }

        .btn-accent:hover:not(:disabled) {
            background-color: #5863c4;
            border-color: #5863c4;
            cursor: pointer;
        }

        .main {
            display: flex;
            flex: 1;
            width: 100%;
            padding: 1rem;
            flex-flow: column;
            align-items: flex-start;
            margin-top: 46px;
        }

        .section-container {
            display: flex;
            flex-flow: column;
            margin-top: 1rem;
            align-items: flex-start;
            border-bottom: 4px solid var(--main);
            width: 100%;
        }

        .section-header {
            background-color: var(--main);
            color: white;
            padding: 4px 12px 2px 10px;
            font-size: 1rem;
            font-weight: 500;
        }

        .toolbar {
            display: flex;
            flex-wrap: wrap;
            align-items: center;
            justify-content: space-between;
            gap: 12px;
            width: 100%;
            padding: 12px 0;
        }

        .table-responsive {
            width: 100%;
            flex: 1;
            background: #fff;
            border-radius: 5px;
            border: 1px solid lightgray;
            box-shadow: 0 2px 7px #13537a24;
            overflow: auto;
            max-height: calc(100vh - 180px);
        }

        table {
            background: inherit;
            width: 100%;
            border-collapse: collapse;
            text-align: left;
            font-family: monospace;
            font-size: 0.85rem;
        }

        th {
            border-bottom: 2px solid gray;
            padding: 10px 12px;
            text-align: left;
            font-size: 0.85rem;
            color: #333;
            background-color: #fafafa;
            font-weight: 600;
            white-space: nowrap;
            position: sticky;
            top: 0;
            z-index: 10;
        }

        td {
            border-bottom: 1px solid #e9ecef;
            padding: 8px 12px;
            vertical-align: middle;
            background: #fff;
        }

        tr:hover td {
            background-color: #f8faff;
        }

        .form-field {
            background: #fff;
            display: flex;
            align-items: center;
            border-radius: 4px;
            padding: 4px 8px;
            border: 1px solid #ced4da;
        }

        .form-field > input {
            background: transparent;
            border: none;
            outline: none;
            font-size: 0.85rem;
            font-family: inherit;
            width: 100%;
        }

        @media screen and (max-width: 800px) {
            .--hidden-sm {
                display: none;
            }
        }
    </style>
</head>
<body>

<div class="header">
    <div class="flex-center">
        ^DEVICE_LOGO^
        <div style="display: flex; align-items: baseline; margin-left: 12px;">
            <h2 class="header-text">action button</h2>
            <h2 class="header-text --hidden-sm">&nbsp|&nbsp</h2>
            <h4 class="header-text --hidden-sm">Logs</h4>
        </div>
    </div>
    <div class="flex-center" style="gap: 10px;">
        <button type="button" class="btn btn-light" onclick="location.href='/'">Back</button>
    </div>
</div>

<div class="main">
    <div class="section-container">
        <span class="section-header">System Logs</span>
    </div>

    <div class="toolbar">
        <div class="flex-center" style="gap: 8px; flex-wrap: wrap;">
            <button type="button" class="btn btn-dark btn-sm" onclick="getLogs()">
                <svg xmlns="http://www.w3.org/2000/svg" width="1.2em" height="1.2em" viewBox="0 0 24 24"><title>refresh</title><path fill="currentColor" d="M12 20q-3.35 0-5.675-2.325T4 12t2.325-5.675T12 4q1.725 0 3.3.712T18 6.75V4h2v7h-7V9h3.2q-.8-1.4-2.187-2.2T12 6Q9.5 6 7.75 7.75T6 12t1.75 4.25T12 18q1.925 0 3.475-1.1T17.65 14h2.1q-.65 2.85-2.887 4.425T12 20"/></svg>
                Refresh
            </button>
            <span id="logCountBadge" style="font-size: 0.85rem; color: #555; font-weight: 600; margin-left: 8px;">0 entries</span>
        </div>
        <div class="form-field" style="width: 220px;">
            <input type="text" id="logFilter" placeholder="Filter logs..." oninput="filterLogs()">
        </div>
    </div>

    <div class="table-responsive">
        <table id="logsTable">
            <thead>
                <tr>
                    <th style="width: 80px;">#</th>
                    <th>Message</th>
                </tr>
            </thead>
            <tbody id="logsTbody">
                <!-- Log rows -->
            </tbody>
        </table>
        <div id="emptyLogs" style="display: none; text-align: center; padding: 40px 20px; color: #888;">
            No logs available
        </div>
    </div>
</div>

<script>
    let rawLogs = {};

    function renderLogs(logsObj) {
        const tbody = document.getElementById('logsTbody');
        const emptyEl = document.getElementById('emptyLogs');
        const badge = document.getElementById('logCountBadge');
        const filter = (document.getElementById('logFilter').value || '').toLowerCase().trim();

        tbody.innerHTML = '';
        const keys = Object.keys(logsObj || {});
        let count = 0;

        if (keys.length === 0) {
            emptyEl.style.display = 'block';
            emptyEl.textContent = 'No logs available';
            badge.textContent = '0 entries';
            return;
        }

        emptyEl.style.display = 'none';

        keys.forEach(key => {
            const msg = String(logsObj[key] || '');
            if (!filter || key.toLowerCase().includes(filter) || msg.toLowerCase().includes(filter)) {
                count++;
                const row = `
                    <tr>
                        <td style="color: #666; font-weight: bold;">${escapeHtml(key)}</td>
                        <td style="word-break: break-all;">${escapeHtml(msg)}</td>
                    </tr>
                `;
                tbody.insertAdjacentHTML('beforeend', row);
            }
        });

        badge.textContent = `${count} ${count === 1 ? 'entry' : 'entries'}`;
        if (count === 0 && keys.length > 0) {
            emptyEl.style.display = 'block';
            emptyEl.textContent = 'No logs match filter';
        }
    }

    async function getLogs() {
        const srvURL = window.location.protocol + "//" + window.location.host + "/logsData";
        try {
            const res = await fetch(srvURL);
            if (res.ok) {
                const data = await res.json();
                rawLogs = data || {};
                renderLogs(rawLogs);
            } else {
                showError('Server returned status ' + res.status);
            }
        } catch (e) {
            showError(e);
        }
    }

    function filterLogs() {
        renderLogs(rawLogs);
    }

    function showError(err) {
        console.error('Error loading logs:', err);
        const tbody = document.getElementById('logsTbody');
        const emptyEl = document.getElementById('emptyLogs');
        tbody.innerHTML = `<tr><td style="color: #c62828;">Error</td><td style="color: #c62828;">Failed to load logs: ${escapeHtml(String(err))}</td></tr>`;
        emptyEl.style.display = 'none';
    }

    function escapeHtml(text) {
        if (!text) return '';
        return String(text)
            .replace(/&/g, "&amp;")
            .replace(/</g, "&lt;")
            .replace(/>/g, "&gt;")
            .replace(/"/g, "&quot;")
            .replace(/'/g, "&#039;");
    }

    getLogs();
</script>
</body>
</html>
)rawliteral";

namespace Components {
    const char CLIENT_MODE_OPTIONS[] PROGMEM = R"rawliteral(
        <div class="checkbox-form-field">
            <input type="checkbox" id="enableOtaUpdate" name="otaUpdate" class="modern-checkbox">
            <label for="enableOtaUpdate">FW Update on client mode</label>
        </div>
        <div class="checkbox-form-field">
            <input type="checkbox" id="clientWebAccess" name="hotspotAccess" class="modern-checkbox">
            <label for="clientWebAccess">Web access on client mode</label>
        </div>
    )rawliteral";

    const char DEVICE_LOGO[] PROGMEM = R"rawliteral(<svg xmlns="http://www.w3.org/2000/svg" xml:space="preserve" width="36px" height="36px" viewBox="0 3 24 24">
    <g> <polygon style="fill:#FFFFFF" points="12.08,6.53 23.67,10.2 12.12,14.49 0.36,10.11 12.08,6.53 12.08,6.35 0,10.08 0,14.35 11.99,18.85 23.98,14.35 23.98,10.19 12.08,6.35 "/>
        <polygon style="fill:#FFFFFF" points="11.99,21.71 0.02,17.12 0.02,19.41 12.01,24 24,19.42 24,17.13 "/>
        <polygon style="fill:#FFFFFF " points="0.36,10.11 12.12,14.49 23.67,10.2 12.08,6.53 "/>
    </g>
</svg>)rawliteral";

    const char NONE[] = "";
}

#endif //EVENT_BUTTON_INDEX_HTML_PAGE_HPP
