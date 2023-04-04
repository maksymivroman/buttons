#ifndef EVENT_BUTTON_HTML_PAGE_HPP
#define EVENT_BUTTON_HTML_PAGE_HPP

#include <Arduino.h>

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta charset="UTF-8">
    <title>Event button | Setup</title>
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
            background-color: #1e5e9d;
            width: 100vw;
            padding: 0 20px 0 20px;
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
        }

        .event-data {
            display: flex;
            border: 1px solid lightgray;
            border-radius: 5px;
            box-shadow: 0 2px 7px #13537a24;
            padding: 5px;
            background: inherit;
            min-height: 500px;
            max-height: 50vh;
            overflow: auto;
        }

        .wifi-credentials {
            display: flex;
            flex-direction: row;
            align-items: center;
            align-content: space-between;
            justify-content: space-evenly;
        }

        .editor {
            display: flex;
            flex-direction: column;
            justify-content: center;
            padding: 18px;
            background: rgba(0, 0, 0, 0.8);
            width: 100vw;
            height: 100vh;
            position: absolute;
            top: 0;
            left: 0;
            visibility: hidden;
        }

        .extras-container {
            display: flex;
            flex: 1;
            flex-direction: column;
            margin-bottom: 16px;
        }

        .item {
            display: flex;
            padding: 8px;
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

        .control {
            display: block;
            min-width: 220px;
            padding: .375rem .75rem;
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

        .btn-red {
            background-color: #cb1d38;
            border: 1px solid #cb1d38;
        }

        .btn-blue {
            background-color: #1e5e9d;
            border: 1px solid #165c9f;
        }

        .btn-red:hover {
            background-color: #a6132f;
        }

        .btn-blue:hover {
            background-color: #083867;
        }

        .label {
            font-size: 1rem;
            display: inline-block;
            margin-bottom: .5rem;
            align-self: flex-start;
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

        @media screen and (max-width: 800px) {
            .wifi-credentials {
                flex-direction: column;
            }
        }
    </style>

</head>
<body>
<div class="header">
    <div style="display: flex; align-items: baseline;">
        <h2 style="color: white; font-weight: 200;">event button |&nbsp</h2>
        <h4 style="color: lightgray; font-weight: 200;">Setup</h4>
    </div>
    <div style="display: flex; align-items: center; gap: 10px;">
        <button style="align-self: flex-end;" type="button" class="btn btn-red" onclick="find()">Find me</button>
        <button style="align-self: flex-end;" type="button" class="btn btn-red">FW Update</button>
    </div>
</div>

<div class="content">
    <h2 style="font-weight: 200">Connectivity</h2>
    <div class="wifi-credentials">
        %NETWORKINFO%
        %EVENTINFO%
        %WIFILIST%
    </div>

    <div style="display: flex; flex: 1; align-items: center; justify-content: space-between">
        <h2 style="font-weight: 200">Assigned events</h2>
        <button style="padding: 0 10px" type="button" class="btn btn-blue" onclick="openEditor(true)">edit</button>
    </div>
    <div class="event-data">
        <table>
            <thead>
            <tr>
                <th colspan="1">Host</th>
                <th style="width: 70%;" colspan="2">Data</th>
            </tr>
            </thead>
            <tbody>
            <tr id="event" style="">
                <td><h5 style="margin: 5px" id="eventHost"></h5></td>
                <td><h6 id="eventData" style="margin: 5px 20px"></h6></td>
            </tr>
            </tbody>
        </table>
    </div>

    <h2 style="font-weight: 200">General settings</h2>

    <div class="editor" id="editor">
        <h2 style="color: white; font-weight: 200; align-self: center;">Edit events</h2>
        <h5 style="color: #b9b9b9;">Events should be valid JSON and host cannot be duplicated!</h5>
        <textarea id="events" cols=120 rows=20 class="control"
                  style="margin-bottom: 0.5rem; background-color: #e0dddd"></textarea>
        <div style="display: flex; justify-content: space-between">
            <div style="align-self: flex-end">
                <input type="checkbox" id="editorAutoFormat" name="dnsName" value="1" checked>
                <label style="margin-left: 8px; color: white" for="editorAutoFormat">Automatically format</label>
            </div>
            <h5 id="eventsFormatWarn" style="color: red; visibility: hidden; line-height: normal">events format not
                valid</h5>
            <div style="align-self: flex-end">
                <button type="button" class="btn btn-red" onclick="saveEvensFromEditor()">Save</button>
                <button type="button" class="btn btn-blue" onclick="openEditor(false)">Cancel</button>
            </div>
        </div>
    </div>

    <div class="editor" id="successModel">
        <div style="display: flex; align-items: center; flex-flow: column">
            <h1 style="color: #b9b9b9;">Configuration saved!</h1>
            <h4 style="color: #b9b9b9; font-weight: 200">Button will reboot now. Please reconnect to continue or close
                this page.</h4>
        </div>
    </div>

    <div class="extras-container">
        <div class="item">
            <input type="checkbox" id="clientWebAccess" name="hotspotAccess" value="1" checked>
            <label style="margin-left: 8px;" for="clientWebAccess">web access on WIFI client mode</label>
        </div>

        <div class="item">
            <input type="checkbox" id="useDnsName" name="dnsName" value="1" checked>
            <label style="margin-left: 8px;" for="useDnsName">use DNS host name (btn.iot)</label>
        </div>
    </div>

    <div style="display: flex; flex: 1; justify-content: center">
        <button id="saveButton" type="button" class="btn btn-red" onclick="saveSettings()">Save And Reboot</button>
    </div>

</div>

<div style="display: flex; flex: 1; justify-content: flex-start; align-items: flex-end; width: 95vw;">
    <div style="display: flex;">
        <h5 style="color: #cb1d38; font-weight: 200; margin: 5px">Firmware version:</h5>
        <h5 style="margin: 5px">%FWVERSION% </h5>
    </div>
    <div style="display: flex;">
        <h5 style="color: #cb1d38; font-weight: 200; margin: 5px">MAC address:</h5>
        <h5 style="margin: 5px">%MAC% </h5>
    </div>
    <div style="display: flex;">
        <h5 style="color: #cb1d38; font-weight: 200; margin: 5px">IP address:</h5>
        <h5 style="margin: 5px">%IP% </h5>
    </div>
    <div style="display: flex;">
        <h5 style="color: #cb1d38; font-weight: 200; margin: 5px">Free HEAP:</h5>
        <h5 style="margin: 5px">%HEAP% </h5>
    </div>
</div>
</body>
</html>

<script>
    function find() {
        const srvURL = window.location.protocol + "//" + window.location.host + "/";
        const httpRequest = new XMLHttpRequest();
        httpRequest.open("POST", srvURL, true);
        httpRequest.setRequestHeader("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
        httpRequest.send("find");
        httpRequest.responseType = 'text';
        httpRequest.onreadystatechange = function () {
        };
    }

    function saveSettings() {
        const name = document.getElementById('wifiname').value;
        const pass = document.getElementById('wifipass').value;
        const clientWebAccess = Number(document.getElementById('clientWebAccess').checked);
        const useDnsName = Number(document.getElementById('useDnsName').checked);

        const extrasConfig = JSON.stringify({clientWebAccess, useDnsName});

        let data = '{ "inputdata" :{"wifiname":"' + name + '","wifipass":"' + pass + '","configuration":' + extrasConfig + ',"eventdata":' + eventSettings + '}}';
        data.replace(/" /g, '');
        data.replace(/ "/g, '');
        const srvURL = window.location.protocol + "//" + window.location.host + "/";
        const httpRequest = new XMLHttpRequest();
        httpRequest.open("POST", srvURL, true);
        httpRequest.setRequestHeader("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
        httpRequest.send(data);
        httpRequest.responseType = 'text';
        httpRequest.onreadystatechange = function () {
            if (httpRequest.readyState === httpRequest.DONE) {
                if (httpRequest.status === 200) {
                    console.log(httpRequest.response);
                    const editor = document.getElementById('successModel');
                    editor.style.visibility = 'visible';
                }
            }
        }
    }

    function showSaved() {
        let data, jsonEvents;
        try {
            jsonEvents = document.getElementById('savedJSON').innerHTML;
            data = JSON.parse(jsonEvents);
        }catch (e) {
            jsonEvents = '{"sample_event_host" : "sample_event_data"}';
        }
        document.getElementById('events').value = JSON.stringify(data, undefined, 4);
        eventSettings = jsonEvents;
        console.log(eventSettings);
        buildTable();
    }

    function buildTable() {
        const item = document.querySelector('#event');
        let i = 0;
        JSON.parse(eventSettings, (key, value) => {
            if (typeof value !== "object") {
                const clone = item.cloneNode(true);
                clone.id = `event-item${i}`;
                item.before(clone);
                const newItem = document.getElementById(`event-item${i}`);
                newItem.querySelector('h5').innerHTML = key;
                newItem.querySelector('h6').innerHTML = value;
            }
            i++;
        });
    }

    function update() {
        const select = document.getElementById('networks');
        const option = select.options[select.selectedIndex];
        document.getElementById('wifiname').value = option.value;
    }

    function openEditor(open) {
        const editor = document.getElementById('editor');
        if (open === true) {

            document.getElementById('events').value = JSON.stringify(JSON.parse(eventSettings), undefined, 4);
            editor.style.visibility = 'visible'
        } else {
            editor.style.visibility = 'hidden';
        }
    }

    function saveEvensFromEditor() {
        eventSettings = document.getElementById('events').value;
        const matches = document.querySelectorAll(`[id*='event-item']`);
        matches.forEach(item => item.remove());
        buildTable();
        openEditor(false);
    }

    const eventsTextarea = document.getElementById("events");
    const eventsFormatWarn = document.getElementById("eventsFormatWarn");
    const autoFormat = document.getElementById('editorAutoFormat');
    let eventSettings;

    eventsTextarea.addEventListener("input", () => {
        eventsFormatWarn.style.visibility = 'hidden';
        eventsTextarea.style.border = '1px solid white';
        try {
            const obj = JSON.parse(eventsTextarea.value);
            if (autoFormat.checked) {
                eventsTextarea.value = JSON.stringify(obj, undefined, 4);
            }
        } catch (e) {
            eventsFormatWarn.style.visibility = 'visible';
            eventsTextarea.style.border = '3px solid red';
        }
    });

    autoFormat.addEventListener("input", () => {
        if (autoFormat.checked) {
            const obj = JSON.parse(eventsTextarea.value);
            eventsTextarea.value = JSON.stringify(obj, undefined, 4);
        }
    })

    window.onload = showSaved;
</script>)rawliteral";

#endif //EVENT_BUTTON_HTML_PAGE_HPP
