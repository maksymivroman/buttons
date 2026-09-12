#ifndef EVENT_BUTTON_FLAGS_HTML_PAGE_HPP
#define EVENT_BUTTON_FLAGS_HTML_PAGE_HPP

#include <Arduino.h>

const char flags_page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta charset="UTF-8">
    <title>action button | Flags</title>
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

        .label-m {
            font-size: .9rem !important;
            margin-left: 8px;
            cursor: pointer;
            user-select: none;
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
            cursor: pointer;
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
            transition: all 100ms ease-out;
        }

        .checkbox-form-field .modern-checkbox:hover {
            background-color: #818285;
        }

        .checkbox-form-field .modern-checkbox:checked {
            background-color: var(--accent);
        }

        .checkbox-form-field .modern-checkbox:checked::after {
            background-color: #fff;
            left: 13px;
        }

        .checkbox-form-field :focus:not(.focus-visible) {
            outline: 0;
        }

        .status-msg {
            margin-top: 16px;
            font-weight: 500;
            font-size: 0.9rem;
            padding: 8px 12px;
            border-radius: 6px;
            display: none;
        }

        .status-msg.success {
            background-color: #e8f5e9;
            color: #1b5e20;
            border: 1px solid #c8e6c9;
            display: block;
        }

        .status-msg.error {
            background-color: #ffebee;
            color: #c62828;
            border: 1px solid #ffcdd2;
            display: block;
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
            <h4 class="header-text --hidden-sm">Flags</h4>
        </div>
    </div>
    <div class="flex-center" style="gap: 10px;">
        <button type="button" class="btn btn-light" onclick="location.href='/'">Back</button>
        <button id="saveFlagsBtn" type="button" class="btn btn-accent" onclick="saveFlags()">Save Flags</button>
    </div>
</div>

<div class="main">
    <div class="section-container">
        <span class="section-header">Hardware Flags</span>
    </div>

    <div class="flex-col" style="gap: 1rem; margin-top: 1.5rem; width: 100%; max-width: 600px;">
        <div class="checkbox-form-field">
            <input type="checkbox" id="ledRDisabled" class="modern-checkbox">
            <label for="ledRDisabled" class="label-m">Disable Red LED</label>
        </div>
        <div class="checkbox-form-field">
            <input type="checkbox" id="ledGDisabled" class="modern-checkbox">
            <label for="ledGDisabled" class="label-m">Disable Green LED</label>
        </div>
        <div class="checkbox-form-field">
            <input type="checkbox" id="ledBDisabled" class="modern-checkbox">
            <label for="ledBDisabled" class="label-m">Disable Blue LED</label>
        </div>

        <div id="statusMsg" class="status-msg"></div>
    </div>
</div>

<script>
    async function loadFlags() {
        try {
            const res = await fetch('/flagsData');
            if (res.ok) {
                const data = await res.json();
                const flags = data.result || data;
                if (flags) {
                    if (flags.ledRDisabled !== undefined) document.getElementById('ledRDisabled').checked = Boolean(flags.ledRDisabled);
                    if (flags.ledGDisabled !== undefined) document.getElementById('ledGDisabled').checked = Boolean(flags.ledGDisabled);
                    if (flags.ledBDisabled !== undefined) document.getElementById('ledBDisabled').checked = Boolean(flags.ledBDisabled);
                }
            }
        } catch (e) {
            console.error('Failed to load flags:', e);
        }
    }

    async function saveFlags() {
        const btn = document.getElementById('saveFlagsBtn');
        const statusEl = document.getElementById('statusMsg');
        btn.disabled = true;
        statusEl.className = 'status-msg';
        statusEl.textContent = '';

        const ledRDisabled = document.getElementById('ledRDisabled').checked ? '1' : '0';
        const ledGDisabled = document.getElementById('ledGDisabled').checked ? '1' : '0';
        const ledBDisabled = document.getElementById('ledBDisabled').checked ? '1' : '0';

        const body = `ledRDisabled=${ledRDisabled}&ledGDisabled=${ledGDisabled}&ledBDisabled=${ledBDisabled}`;

        try {
            const res = await fetch('/flags', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/x-www-form-urlencoded; charset=UTF-8'
                },
                body: body
            });

            if (res.ok) {
                statusEl.className = 'status-msg success';
                statusEl.textContent = 'Flags updated successfully! Device is restarting...';
                setTimeout(() => {
                    location.href = '/';
                }, 4000);
            } else {
                statusEl.className = 'status-msg error';
                statusEl.textContent = 'Failed to update flags. Status: ' + res.status;
                btn.disabled = false;
            }
        } catch (e) {
            statusEl.className = 'status-msg error';
            statusEl.textContent = 'Error updating flags: ' + e;
            btn.disabled = false;
        }
    }

    loadFlags();
</script>
</body>
</html>
)rawliteral";

#endif //EVENT_BUTTON_FLAGS_HTML_PAGE_HPP
