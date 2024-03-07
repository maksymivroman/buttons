#ifndef EVENT_BUTTON_HTML_PAGE_HPP
#define EVENT_BUTTON_HTML_PAGE_HPP

#include <Arduino.h>

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta charset="UTF-8">
    <title>Sanitizer | Setup</title>
    <style>
        html {
            font-size: 16px;
        }

        body {
            font-family: Arial, Helvetica, sans-serif;
            margin: 0;
            background: #eeeaea;
            display: flex;
            flex-direction: column;
            align-items: center;
            height: 100vh;
        }

        .header {
            max-height: 50px;
            background-color: #316b32;
            width: 100vw;
            padding: 0 12px 0 12px;
            display: flex;
            align-items: center;
            flex: 1;
            justify-content: space-between;
        }

        .section-header {
            font-weight: 200;
            margin-top: 36px;
        }

        .content {
            display: flex;
            flex-flow: column;
            width: 100vw;
            padding: 16px;
            overflow: auto;
        }

        .event-data {
            display: flex;
            padding: 5px;
            background: inherit;
            min-height: 500px;
            max-height: 50vh;
            overflow: auto;
        }

        .border {
            border: 1px solid lightgray;
            border-radius: 5px;
            box-shadow: 0 2px 7px #13537a24;
        }

        .wifi-credentials {
            display: flex;
            align-items: flex-start;
            align-content: space-between;
        }

        .main-container {
            padding: 0 8px;
        }

        .modal {
            display: flex;
            flex-direction: column;
            justify-content: center;
            padding: 18px;
            background: rgba(0, 0, 0, 0.9);
            width: 100vw;
            height: 100vh;
            position: absolute;
            top: 0;
            left: 0;
            visibility: hidden;
        }

        .extras-container {
            display: grid;
            grid-template-columns: repeat(auto-fill, 320px);
            margin-bottom: 16px;
            padding: 24px;
        }

        .connectivity-container {
            display: flex;
            flex: 1;
            flex-flow: column;
            align-content: flex-start;
            flex-wrap: wrap;
            padding: 24px;
        }

        .item {
            display: flex;
            align-items: center;
            padding: 8px;
            margin-right: 20px;
        }

        .--vertical {
            flex-flow: column;
            align-items: flex-start;
        }

        .--fill {
            width: -webkit-fill-available;
        }

        .--hidden {
            visibility: hidden;
        }

        .--display-none {
            display: none !important;
        }

        .--not-valid-event {
            border: 1px solid red;
        }

        table {
            background: inherit;
            width: 100vw;
            height: fit-content;
        }

        th {
            border-bottom: 1px solid gray;
        }

        td {
            border-bottom: 1px solid lightgray;
        }

        .hashed {
            color: #858181;
            background: repeating-linear-gradient(
                    45deg,
                    transparent,
                    transparent 10px,
                    rgba(0, 0, 0, 0.04) 10px,
                    rgba(0, 0, 0, 0.04) 20px
            );
        }

        .mt-m {
            margin-top: 16px;
        }

        .control {
            display: block;
            min-width: 220px;
            padding: .175rem .75rem;
            font-size: 1rem;
            line-height: 1.5;
            color: #495057;
            background-color: #fff;
            background-clip: padding-box;
            border: 1px solid #ced4da;
            border-radius: .25rem;
            transition: border-color .15s ease-in-out, box-shadow .15s ease-in-out;
        }

        .btn {
            color: #fff;
            display: inline-block;
            font-weight: 400;
            text-align: center;
            white-space: nowrap;
            vertical-align: middle;
            -webkit-user-select: none;
            -moz-user-select: none;
            -ms-user-select: none;
            user-select: none;
            padding: .375rem .75rem;
            font-size: 1rem;
            line-height: 1.5;
            border-radius: .25rem;
            transition: color .15s ease-in-out, background-color .15s ease-in-out, border-color .15s ease-in-out, box-shadow .15s ease-in-out;
        }

        .btn:disabled {
            color: lightgray;
            background-color: gray;
            border: 1px solid gray;
        }

        .btn-red {
            background-color: #4a58b1;
            border: 1px solid #3c4bab;
        }

        .btn-blue {
            background-color: #336934;
            border: 1px solid #316b32;
        }

        .small-btn {
            max-height: 24px;
            padding: 0 10px;
        }

        .btn-red:hover:not(:disabled) {
            background-color: #4a58b1;
        }

        .btn-blue:hover:not(:disabled) {
            background-color: #316b32;
        }

        .label {
            font-size: 1rem;
            display: inline-block;
            margin-top: .5rem;
            align-self: flex-start;
            font-weight: 600;
        }

        input[type=checkbox]:disabled + label {
            color: darkgray;
        }

        #loggerEnabled:not(:checked) ~ #loggerLevel {
            color: darkgray;
            pointer-events: none;
        }

        input:disabled {
            color: dimgray;
            background-color: lightgray;
            user-select: none;
        }

        input[type=checkbox]:not(:checked) + label + input {
            color: darkgray;
            display: none;
        }

        option {
            font-size: 1rem;
        }

        button, input, optgroup, select, textarea {
            margin: 0;
            font-family: inherit;
            font-size: inherit;
            line-height: inherit;
        }

        *, ::after, ::before {
            box-sizing: border-box;
        }

        button, select {
            text-transform: none;
        }

        select.control:not([size]):not([multiple]) {
            height: calc(2.25rem + 2px);
        }

        .flexbox-wrap {
            display: flex;
            flex-wrap: wrap;
        }

        .max-h {
            height: 100%;
            align-content: space-between;
        }

        .max-w {
            width: 100%;
            justify-content: space-between;
        }

        .flexbox-column-centered {
            display: flex;
            flex-flow: column;
            align-items: center
        }

        .flexbox-column-start {
            display: flex;
            flex-flow: column;
        }

        .flexbox-raw-centered {
            display: flex;
            justify-content: center;
            align-items: center
        }

        @media screen and (max-width: 800px) {
            .wifi-credentials {
                flex-direction: column;
            }

            .setup-logo {
                display: none;
            }
        }
    </style>
</head>
<body>

<div style="display: flex; flex-flow: column; overflow: auto;">

    <div class="header">
        <div style="display: flex; align-items: center;">
                <svg xmlns="http://www.w3.org/2000/svg" height="36" viewBox="0 -960 960 960" width="36" fill="#FFFFFF">
                    <path d="M700-640q-39 0-69.5-30.5T600-740q0-35 23-77.5T700-920q54 60 77 102.5t23 77.5q0 39-30.5 69.5T700-640ZM240-80q-66 0-113-47T80-240v-216q0-31 14.5-57.5T135-557l330-214 20 20q20 20 23.5 47.5T497-652l-45 72h308q17 0 28.5 11.5T800-540q0 17-11.5 28.5T760-500H308l77-124-207 134q-9 5-13.5 14t-4.5 20v216q0 33 23.5 56.5T240-160h480q17 0 28.5 11.5T760-120q0 17-11.5 28.5T720-80H240Zm240-280v-80h360q17 0 28.5 11.5T880-400q0 17-11.5 28.5T840-360H480Zm0 140v-80h320q17 0 28.5 11.5T840-260q0 17-11.5 28.5T800-220H480ZM320-360Z"/>
                </svg>
            <div style="display: flex; align-items: baseline; margin-left: 12px;">
                <h2 style="color: white; font-weight: 200;">sanitizer</h2>
                <h2 style="color: white; font-weight: 200;" class="setup-logo">&nbsp|&nbsp</h2>
                <h4 style="color: lightgray; font-weight: 200;" class="setup-logo">Setup</h4>
            </div>
        </div>
        <div style="display: flex; align-items: center; gap: 10px;">
            <button style="align-self: flex-end;" type="button" class="btn btn-red" onclick="find()">Find me</button>
            <button disabled id="updateBtn" style="align-self: flex-end;" type="button" class="btn btn-red"
                    onclick="location.href='/update'">FW Update
            </button>
        </div>
    </div>

    <div class="content">
        <h2 class="section-header">Connectivity</h2>
        <div class="connectivity-container border">
            <div class="wifi-credentials">
                %WIFILIST%
                <label class="label"></label>
                %NETWORKINFO%
            </div>

            <div class="item mt-m" style="min-height: 55px; flex-wrap: wrap; gap: 20px;">
                <input type="checkbox" id="useHotspotSsid">
                <label for="useHotspotSsid">Use custom hotspot SSID</label>
                <input maxlength="32" type="text" id="hotspotSsid" class="control">
            </div>

        </div>

        <div style="display: flex; flex: 1; align-items: center; justify-content: space-between">
            <h2 class="section-header">Assigned events</h2>
            <button style="display: flex; padding: 0 10px; align-self: flex-end; margin-bottom: 16px; align-items: center; gap: 4px;"
                    type="button" class="btn btn-red" onclick="openEditor()"><h1
                    style="margin: 0; line-height: normal; font-weight: 200;">+</h1> Add new event
            </button>

        </div>
        <div class="event-data border">

            <table id="dataTable">
                <thead>
                <tr>
                    <th>host</th>
                    <th style="width: 70%;">event data</th>
                    <th>actions</th>
                </tr>
                </thead>
                <tbody>
                <!-- Table data -->
                </tbody>
            </table>

        </div>

        <h2 class="section-header">General settings</h2>

        <div class="modal" id="successModel">
            <div style="display: flex; align-items: center; flex-flow: column">
                <svg xmlns="http://www.w3.org/2000/svg" height="200" viewBox="0 -960 960 960" width="200" fill="#e30000">
                    <path id="upload-box_1" d="M700-640q-39 0-69.5-30.5T600-740q0-35 23-77.5T700-920q54 60 77 102.5t23 77.5q0 39-30.5 69.5T700-640ZM240-80q-66 0-113-47T80-240v-216q0-31 14.5-57.5T135-557l330-214 20 20q20 20 23.5 47.5T497-652l-45 72h308q17 0 28.5 11.5T800-540q0 17-11.5 28.5T760-500H308l77-124-207 134q-9 5-13.5 14t-4.5 20v216q0 33 23.5 56.5T240-160h480q17 0 28.5 11.5T760-120q0 17-11.5 28.5T720-80H240Zm240-280v-80h360q17 0 28.5 11.5T880-400q0 17-11.5 28.5T840-360H480Zm0 140v-80h320q17 0 28.5 11.5T840-260q0 17-11.5 28.5T800-220H480ZM320-360Z"/>
                </svg>

                <h1 style="color: #b9b9b9;" id="dialogMessageTitle">Configuration saved!</h1>
                <h4 style="color: #b9b9b9; font-weight: 200">Sanitizer controller now will reboot to apply new settings. It will take
                    a while</h4>
                <h4 style="color: #b9b9b9; font-weight: 200">If if was connected to Wi-Fi during setup, you can refresh the page to continue</h4>
                <button disabled id="refreshBtn" type="button" class="btn btn-blue" onclick="location.reload()">Refresh page</button>
            </div>
        </div>

        <div class="extras-container border">

            <div class="item">
                <input type="checkbox" id="loggerEnabled">
                <label style="margin-left: 8px; margin-right: 8px;" for="loggerEnabled">Logger</label>
                <select class="control" style="height: auto; min-width: auto;" id="loggerLevel">
                    <option value="0">Serial & Local</option>
                    <option value="1">Serial (115200 8-N-1)</option>
                    <option value="2">Local log (/logs)</option>
                </select>
            </div>

            <div class="item">
                <input type="checkbox" id="useSound" name="dnsName">
                <label style="margin-left: 8px;" for="useSound">Use sound notification</label>
            </div>

            %CLIENTMODEOPTIONS%

        </div>

        <h2 class="section-header">Remote triggering</h2>

        <div class="border" style="padding: 24px">
            <div class="item">
                <input type="checkbox" id="remoteTriggering">
                <label for="remoteTriggering" style="margin: 0 8px;">Remote triggering</label>
            </div>

            <div style="display: flex; flex-flow: row; align-items: center; margin-left: 16px;">
                <h5 class="item" style="margin: 0">Trigger using POST request:</h5>
            </div>
            <div style="display: flex; flex-flow: row; align-items: center; margin: 0 32px;">
                <h5 class="item" style="margin: 0">Content-Type: </h5>
                <h5 class="item" style="font-weight: normal; margin: 0">application/form-data</h5>
            </div>
            <div style="display: flex; flex-flow: row; align-items: center; margin: 0 32px;">
                <h5 class="item" style="margin: 0">Key/Value: </h5>
                <h5 class="item" style="font-weight: normal; margin: 0">TRIGGER_BUTTON: AUTO</h5>
            </div>
        </div>

        <div style="display: flex; flex: 1; justify-content: center; margin: 80px 0;">
            <button id="saveButton" type="button" class="btn btn-red" onclick="saveSettings()">Save And Reboot</button>
        </div>

    </div>

    <div style="display: flex; justify-content: flex-start; align-items: center; width: 100vw; background: lightgray;">
        <div class="flexbox-wrap max-h">
            <h5 style="color: #cb1d38; font-weight: 200; margin: 5px">Origin FW version:</h5>
            <h5 style="margin: 5px">%FWVERSION% </h5>
        </div>
        <div class="flexbox-wrap max-h">
            <h5 style="color: #cb1d38; font-weight: 200; margin: 5px">MAC address:</h5>
            <h5 style="margin: 5px">%MAC% </h5>
        </div>
        <div class="flexbox-wrap max-h">
            <h5 style="color: #cb1d38; font-weight: 200; margin: 5px">IP address:</h5>
            <h5 style="margin: 5px">%IP% </h5>
        </div>
        <div class="flexbox-wrap max-h">
            <h5 style="color: #cb1d38; font-weight: 200; margin: 5px">Free HEAP:</h5>
            <h5 style="margin: 5px">%HEAP% </h5>
        </div>
    </div>

</div>

<dialog id="editor" style="background-color: #444444;">
    <div class="flexbox-column-centered">
        <h1 style="color: white; font-weight: 200; margin-bottom: 64px;">Add new event</h1>
        <div class="flexbox-column-centered" style="align-items: flex-start; gap: 8px;">
            <div class="flexbox-raw-centered" style="align-items: flex-start; gap: 24px; margin-bottom: 64px;">
                <div class="flexbox-raw-centered" style="gap: 8px;">
                    <input id="nsType" type="radio" name="eventType" value="ns-event">
                    <label for="nsType" style="color: white;">NoviSign event</label>
                </div>

                <div class="flexbox-raw-centered" style="gap: 8px;">
                    <input id="customType" type="radio" name="eventType" value="custom-event" checked>
                    <label for="customType" style="color: white;">Custom event</label>
                </div>
            </div>

            <div class="flexbox-wrap max-w">
                <div class="flexbox-column-start">
                    <label style="color: white; margin-bottom: 8px;" for="eventHost">Event host/integration type</label>
                    <input type="text" id="eventHost" class="control" style="min-width: 300px;">
                </div>
                <div id="nsType-eventType" class="flexbox-column-start --hidden">
                    <label style="color: white; margin-bottom: 8px;" for="eventType">Event type</label>
                    <input type="text" id="eventType" class="control" style="min-width: 300px;" value="DYNAMIC_EVENT">
                </div>
            </div>
            <label style="color: white; margin-top: 16px;" for="events">Event data</label>
            <textarea id="events" cols=75 rows=10></textarea>
            <span id="eventsFormatWarn" style="color: red; visibility: hidden; line-height: normal;">Events data not
                valid</span>
            <span id="eventAlreadyExist" style="color: red; visibility: hidden; line-height: normal;">Event already exists for this host</span>
            <div style="margin-top: 24px;">
                <input type="checkbox" id="eventJsonFormat" name="dnsName" value="1">
                <label style="margin-left: 8px; color: white" for="eventJsonFormat">Format as JSON</label>
            </div>
        </div>

        <div class="flexbox-wrap" style="gap: 32px; align-self: flex-end;">
            <button id="editThisEvent" type="button" class="btn btn-red --hidden"
                    style="min-width: 100px; margin-top: 32px;"
                    onclick="editEvent()">Edit
            </button>
            <button id="addNewEvent" type="button" class="btn btn-red" style="min-width: 100px; margin-top: 32px;"
                    disabled
                    onclick="addToEventList()">Add
            </button>
            <button type="button" class="btn btn-red" style="min-width: 100px; margin-top: 32px;"
                    onclick="closeEditor()">Cancel
            </button>
        </div>
    </div>
</dialog>

</div>
<div id="body"></div>

</body>
</html>

<script>
    function httpPOST(data, successMessage) {
        const srvURL = window.location.protocol + "//" + window.location.host + "/";
        const httpRequest = new XMLHttpRequest();
        httpRequest.open("POST", srvURL, true);
        httpRequest.setRequestHeader("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
        httpRequest.send(data);
        httpRequest.responseType = 'text';
        httpRequest.onreadystatechange = function () {
            if (httpRequest.readyState === httpRequest.DONE) {
                if (httpRequest.status === 200) {
                    document.getElementById('dialogMessageTitle').innerText = successMessage;
                    const successModel = document.getElementById('successModel');
                    successModel.style.visibility = 'visible';
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

    function saveSettings() {
        const name = document.getElementById('wifiname').value;
        const pass = document.getElementById('wifipass').value;
        const hSsid = document.getElementById('hotspotSsid').value;
        const clientWebAccess = document.getElementById('clientWebAccess')? Number(document.getElementById('clientWebAccess')?.checked) : config.clientWebAccess;
        const enableOtaUpdate = document.getElementById('enableOtaUpdate')? Number(document.getElementById('enableOtaUpdate')?.checked) : config.enableOtaUpdate;
        const loggerEnabled = Number(document.getElementById('loggerEnabled').checked);
        const loggerLevel = Number(document.getElementById('loggerLevel').selectedIndex);
        const useSound = Number(document.getElementById('useSound').checked);
        const customHSsid = Number(document.getElementById('useHotspotSsid').checked);
        const remoteTriggering = Number(document.getElementById('remoteTriggering').checked);

        const extrasConfig = JSON.stringify({
            clientWebAccess,
            loggerEnabled,
            loggerLevel,
            useSound,
            customHSsid,
            enableOtaUpdate,
            remoteTriggering,
            wifiSsid: name,
            wifiPass: pass,
            hotspotSsid: hSsid
        });

        let data = '{ "inputdata" :{"wifiname":"' + name + '","wifipass":"' + pass + '","configuration":' + extrasConfig + ',"eventdata":' + JSON.stringify(eventDataObj) + '}}';
        data.replace(/" /g, '');
        data.replace(/ "/g, '');
        const srvURL = window.location.protocol + "//" + window.location.host + "/";
        const httpRequest = new XMLHttpRequest();
        httpRequest.open("POST", srvURL, true);
        httpRequest.setRequestHeader("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
        httpRequest.send("SAVE=" + data);
        httpRequest.responseType = 'text';
        httpRequest.onreadystatechange = function () {
            if (httpRequest.readyState === httpRequest.DONE) {
                if (httpRequest.status === 200) {
                    const successModel = document.getElementById('successModel');
                    successModel.style.visibility = 'visible';
                    handleSuccessSave();
                }
            }
        }
    }

    function showSaved() {
        config = %CONFIGURATION% ;

        eventDataObj = %EVENTINFO% ;

        if (eventDataObj === null) {
            eventDataObj = {};
        }

        try {
            document.getElementById('clientWebAccess').checked = config.clientWebAccess;
            document.getElementById('enableOtaUpdate').checked = config.enableOtaUpdate;
        } catch (e) {
            console.warn('some options are restricted in client mode');
        }

        if (!config.enableOtaUpdate && !document.getElementById('enableOtaUpdate')) {
            document.getElementById('updateBtn').remove();
        }

        document.getElementById('loggerEnabled').checked = config.loggerEnabled;
        document.getElementById('loggerLevel').selectedIndex = config.loggerLevel;
        document.getElementById('useSound').checked = config.useSound;
        document.getElementById('useHotspotSsid').checked = config.customHSsid;
        document.getElementById('hotspotSsid').value = config.hotspotSsid;
        document.getElementById('remoteTriggering').checked = config.remoteTriggering;

        try {
            JSON.parse('"' + eventDataObj + '"');
        } catch (e) {
            eventDataObj = {"sample_event_host": "sample_event_data"};
            console.warn('no saved events found');
        }

    }

    function update() {
        const select = document.getElementById('networks');
        const option = select.options[select.selectedIndex];
        document.getElementById('wifiname').value = option.value;
    }
    let config, eventDataObj;
    let entryKeyToEdit;
    const dataTable = document.getElementById('dataTable');
    const tbody = dataTable.querySelector('tbody');

    function populateTable() {
        tbody.innerHTML = '';
        for (const key in eventDataObj) {
            const row = `
                        <tr ${key.startsWith("$",0) ? 'class="hashed"' : ''}>
                          <td>${key}</td>
                          <td >${eventDataObj[key]}</td>
                          <td style="display: flex; justify-content: flex-end; gap: 8px;">
                            <button class="btn btn-blue small-btn" onclick="openEditSingleEventModal('${key}')">Edit</button>
                            <button class="btn btn-red small-btn" onclick="removeEntry('${key}')">Remove</button>
                          </td>
                        </tr>
                      `;
            tbody.insertAdjacentHTML('beforeend', row);
        }
    }

    function addEventEntry(host, data) {
        const eventExist = host && eventDataObj.hasOwnProperty(host);
        if (!eventExist) {
            if (data) {
                eventDataObj[host] = data;
                populateTable();
            } else {
                throw new Error("event data is empty");
            }
        } else {
            throw new Error("event already exist");
        }
    }

    function removeEntry(key) {
        if (confirm(`Are you sure you want to remove the event "${key}"?`)) {
            delete eventDataObj[key];
            populateTable();
        }
    }

    const eventData = document.getElementById("events");
    const nsEventTypeContainer = document.getElementById("nsType-eventType");
    const hostInput = document.getElementById("eventHost");
    const addNewEventButton = document.getElementById("addNewEvent");
    const eventsFormatWarn = document.getElementById("eventsFormatWarn");
    const eventsAlreadyExistWarn = document.getElementById("eventAlreadyExist");

    document.getElementsByName("eventType").forEach(e => e.addEventListener("input", (el) => {
        switch (el.target.value) {
            case "ns-event":
                eventData.value = "[{\"name\":\"propName_1\",\"value\":\"val_forProp_1\"}]";
                hostInput.value = "";
                hostInput.disabled = false;
                nsEventTypeContainer.classList.remove("--hidden");
                break;
            case "custom-event":
                nsEventTypeContainer.classList.add("--hidden");
                hostInput.disabled = false;
                eventData.value = "";
                hostInput.value = "";
                break;
        }
        checkFormValidity();
    }))

    document.getElementById("eventJsonFormat").addEventListener("change", () => {
        checkFormValidity();
    })

    hostInput.addEventListener("input", () => {
        checkFormValidity();
    });

    eventData.addEventListener("input", () => {
        if (document.getElementById("eventJsonFormat").checked) {
            autoFormatEventData();
        }
        checkFormValidity();
    });

    function checkFormValidity() {
        let isValid;
        isValid = hostInput.value && eventData.value !== "";
        const eventAlreadyExist = hostInput.value && eventDataObj.hasOwnProperty(hostInput.value);
        eventsFormatWarn.style.visibility = 'hidden';
        eventsAlreadyExistWarn.style.visibility = eventAlreadyExist && !entryKeyToEdit ? 'visible' : 'hidden';
        eventData.classList.remove("--not-valid-event");

        if (document.getElementById("eventJsonFormat").checked) {
            try {
                JSON.parse(eventData.value);
            } catch (e) {
                eventsFormatWarn.style.visibility = 'visible';
                eventData.classList.add("--not-valid-event");
                isValid = false;
            }
        }
        addNewEventButton.disabled = !isValid || eventAlreadyExist;
    }

    function autoFormatEventData() {
        try {
            const obj = JSON.parse(eventData.value);
            eventData.value = JSON.stringify(obj, undefined, 4);
        } catch (e) {
        }
    }

    function addToEventList() {
        let data = eventData.value;
        if (document.getElementById("nsType").checked) {
            const type = document.getElementById("eventType").value;
            const obj = JSON.parse(eventData.value);
            data = `{'eventName':'${type}','eventData':${JSON.stringify(obj, undefined, null)}}`;
        }
        try {
            addEventEntry(hostInput.value, data);
            /**reset controls*/
            if (document.getElementById("customType").checked) {
                eventData.value = "";
                hostInput.value = "";
            } else {
                document.getElementById("customType").click();
            }

        } catch (e) {
            alert(e);
        }
        closeEditor();
    }

    function openEditor() {
        checkFormValidity();
        document.getElementById('editor').showModal();
    }

    function closeEditor() {
        resetControls();
        entryKeyToEdit = null;
        document.getElementById('editor').close();
    }

    function resetControls() {
        eventData.value = "";
        hostInput.value = "";
        document.getElementById("editThisEvent").classList.add('--hidden');
        addNewEventButton.classList.remove('--display-none');
        document.getElementById("customType").click();
        document.getElementById("eventJsonFormat").checked = false;
        checkFormValidity();
    }

    function openEditSingleEventModal(key) {
        document.getElementById('editor').showModal();
        if (key) {
            entryKeyToEdit = key;
            eventData.value = eventDataObj[key];
            hostInput.value = key;

            const editEventButton = document.getElementById("editThisEvent");
            editEventButton.classList.remove('--hidden');
            addNewEventButton.classList.add('--display-none');
        }
    }

    function editEvent() {
        if (hostInput.value !== null && eventData.value !== null) {
            delete eventDataObj[entryKeyToEdit];
            eventDataObj[hostInput.value] = eventData.value;
            populateTable();
        }
        closeEditor();
    }

    showSaved();
    populateTable();

    function handleSuccessSave() {
        if (window.location.host === '192.168.4.1') {
            changeButtonColor(()=>{
                document.querySelector('#refreshBtn').disabled = false;
            });
        } else {
            setTimeout(()=>{
                document.querySelector('#upload-box_1').style.fill = '#3c5dd7';
            }, 4000);
            setTimeout(startCheckRestart, 2000);
        }
    }

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

</script>)rawliteral";

const char logs_page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta charset="UTF-8">
    <title>Sanitizer | Logs</title>
    <style>
        html {
            font-size: 16px;
        }

        body {
            font-family: Arial, Helvetica, sans-serif;
            margin: 0;
            background: #eeeaea;
            display: flex;
            flex-direction: column;
            align-items: center;
            height: 100vh;
        }

        .header {
            max-height: 50px;
            background-color: #316b32;
            width: 100vw;
            padding: 0 12px 0 12px;
            display: flex;
            align-items: center;
            flex: 1;
            justify-content: space-between;
        }

        .content {
            display: flex;
            flex-flow: column;
            width: 100vw;
            padding: 16px;
            overflow: auto;
        }

        .btn {
            color: #fff;
            display: inline-block;
            font-weight: 400;
            text-align: center;
            white-space: nowrap;
            vertical-align: middle;
            -webkit-user-select: none;
            -moz-user-select: none;
            -ms-user-select: none;
            user-select: none;
            padding: .375rem .75rem;
            font-size: 1rem;
            line-height: 1.5;
            border-radius: .25rem;
            transition: color .15s ease-in-out, background-color .15s ease-in-out, border-color .15s ease-in-out, box-shadow .15s ease-in-out;
        }

        .btn-red {
            background-color: #4a58b1;
            border: 1px solid #3c4bab;
        }

        .btn-red:hover:not(:disabled) {
            background-color: #4a58b1;
        }

        button, input, optgroup, select, textarea {
            margin: 0;
            font-family: inherit;
            font-size: inherit;
            line-height: inherit;
        }

        *, ::after, ::before {
            box-sizing: border-box;
        }

        button, select {
            text-transform: none;
        }

        table {
            background: inherit;
            width: 100vw;
            height: fit-content;
        }

        th {
            border-bottom: 1px solid gray;
        }

        td {
            border-bottom: 1px solid lightgray;
        }

        @media screen and (max-width: 800px) {
            .setup-logo {
                display: none;
            }
        }
    </style>
</head>
<body>
<div style="display: flex; flex-flow: column; overflow: auto;">
    <div class="header">
        <div style="display: flex; align-items: center;">
            <svg xmlns="http://www.w3.org/2000/svg" height="36" viewBox="0 -960 960 960" width="36" fill="#FFFFFF">
                <path d="M700-640q-39 0-69.5-30.5T600-740q0-35 23-77.5T700-920q54 60 77 102.5t23 77.5q0 39-30.5 69.5T700-640ZM240-80q-66 0-113-47T80-240v-216q0-31 14.5-57.5T135-557l330-214 20 20q20 20 23.5 47.5T497-652l-45 72h308q17 0 28.5 11.5T800-540q0 17-11.5 28.5T760-500H308l77-124-207 134q-9 5-13.5 14t-4.5 20v216q0 33 23.5 56.5T240-160h480q17 0 28.5 11.5T760-120q0 17-11.5 28.5T720-80H240Zm240-280v-80h360q17 0 28.5 11.5T880-400q0 17-11.5 28.5T840-360H480Zm0 140v-80h320q17 0 28.5 11.5T840-260q0 17-11.5 28.5T800-220H480ZM320-360Z"/>
            </svg>
            <div style="display: flex; align-items: baseline; margin-left: 12px;">
                <h2 style="color: white; font-weight: 200;">Sanitizer</h2>
                <h2 style="color: white; font-weight: 200;" class="setup-logo">&nbsp|&nbsp</h2>
                <h4 style="color: lightgray; font-weight: 200;" class="setup-logo">Logs</h4>
            </div>
        </div>
        <div style="display: flex; align-items: center; gap: 10px;">
            <button style="align-self: flex-end;" type="button" class="btn btn-red"
                    onclick="location.href='/'">Back
            </button>
        </div>
    </div>
    <div class="content">
        <div style="overflow: auto">
            <table id="logsTable" style="width: auto; font-family: monospace; min-width: 400px;">
                <thead>
                <tr>
                    <th colspan="1">#</th>
                    <th colspan="1" style="display: flex; justify-content: flex-end; padding: 4px">
                        <button style="padding: 0 8px;" type="button" class="btn btn-red"
                                onclick="getLogs()">refresh
                        </button>
                    </th>
                </tr>
                </thead>
                <tbody>
                <tr></tr>
                </tbody>
            </table>
        </div>
    </div>
</div>
</body>
</html>
<script>
    const logsTbody = document.querySelector('#logsTable').querySelector('tbody');
    function getLogs() {
        const srvURL = window.location.protocol + "//" + window.location.host + "/logsData";
        const httpRequest = new XMLHttpRequest();
        httpRequest.open("GET", srvURL, true);
        httpRequest.setRequestHeader("Content-Type", "application/json; charset=UTF-8");
        httpRequest.send();
        httpRequest.responseType = 'json';
        httpRequest.onreadystatechange = function () {
            if (httpRequest.status === 200) {
                if (httpRequest.response) {
                    try {
                        while (logsTbody.rows.length > 0) {
                            logsTbody.deleteRow(0)
                        }
                        if (Object.entries(httpRequest.response).length === 0) {
                            const row = `<tr><td>-</td><td>No logs available</td></tr>`;
                            logsTbody.insertAdjacentHTML('beforeend', row);
                        }
                        for (const key in httpRequest.response) {
                            const row = `<tr><td>${key}-</td><td>${httpRequest.response[key]}</td></tr>`;
                            logsTbody.insertAdjacentHTML('beforeend', row);
                        }
                    } catch (e) {
                        showError(`Wrong logsData: ${e}`)
                    }
                }
            } else {
                showError(httpRequest.status);
            }
        };
    }
    getLogs();
    function showError(text) {
        console.error('Error:', text);
        while (logsTbody.rows.length > 0) {
            logsTbody.deleteRow(0)
        }
        const row = `<tr><td>Error:</td><td>failed to load logs!</td></tr>`;
        logsTbody.insertAdjacentHTML('beforeend', row);
    }
</script>
)rawliteral";

#endif //EVENT_BUTTON_HTML_PAGE_HPP
