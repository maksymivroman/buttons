#ifndef EVENT_BUTTON_ACTION_EDITOR_HPP
#define EVENT_BUTTON_ACTION_EDITOR_HPP

#include <Arduino.h>

const char action_editor_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta charset="UTF-8">
    <title>action button | Actions</title>
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

        .btn-icon {
            min-width: auto;
            width: 30px;
            height: 30px;
            padding: 0;
            border-radius: 50%;
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

        .btn-red {
            background-color: #cb1d38;
            border: 1px solid #cb1d38;
            color: #fff;
        }

        .btn-red:hover:not(:disabled) {
            background-color: #a6132f;
            border-color: #a6132f;
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

        .toolbar-left, .toolbar-right {
            display: flex;
            align-items: center;
            gap: 8px;
            flex-wrap: wrap;
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

        .table-responsive {
            width: 100%;
            flex: 1;
            background: #fff;
            border-radius: 5px;
            border: 1px solid lightgray;
            box-shadow: 0 2px 7px #13537a24;
            overflow: hidden;
        }

        table {
            background: inherit;
            width: 100%;
            border-collapse: collapse;
            text-align: left;
        }

        th {
            border-bottom: 2px solid gray;
            padding: 10px 12px;
            text-align: left;
            font-size: 0.9rem;
            color: #333;
            background-color: #fafafa;
            font-weight: 600;
            white-space: nowrap;
        }

        td {
            border-bottom: 1px solid lightgray;
            padding: 8px 10px;
            vertical-align: middle;
            font-size: 0.9rem;
            background: #fff;
        }

        tr:hover td {
            background-color: #f8faff;
        }

        tr.row-disabled td {
            background-color: #fcfcfc;
            opacity: 0.65;
        }

        .table-control {
            width: 100%;
            padding: 6px 8px;
            font-size: 0.85rem;
            color: #333;
            background-color: #fff;
            border: 1px solid #ced4da;
            border-radius: 4px;
            outline: none;
            font-family: inherit;
            transition: border-color .15s ease-in-out, box-shadow .15s ease-in-out;
        }

        .table-control:focus {
            border-color: var(--accent);
            box-shadow: 0 0 0 2px rgba(110, 121, 214, 0.25);
        }

        .checkbox-form-field {
            display: flex;
            align-items: center;
            justify-content: center;
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

        .badge {
            display: inline-block;
            padding: 2px 6px;
            font-size: 0.75rem;
            font-weight: 600;
            border-radius: 3px;
            white-space: nowrap;
            margin: 2px;
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

        .multiselect {
            position: relative;
            user-select: none;
            min-width: 160px;
        }

        .multiselect-select {
            display: flex;
            align-items: center;
            justify-content: space-between;
            flex-wrap: wrap;
            gap: 4px;
            min-height: 32px;
            padding: 3px 6px;
            background: #fff;
            border: 1px solid #ced4da;
            border-radius: 4px;
            cursor: pointer;
            font-size: 0.85rem;
            transition: border-color .15s ease-in-out, box-shadow .15s ease-in-out;
        }

        .multiselect-select:hover {
            border-color: #adb5bd;
        }

        .multiselect-select.active {
            border-color: var(--accent);
            box-shadow: 0 0 0 2px rgba(110, 121, 214, 0.25);
        }

        .multiselect-placeholder {
            color: #888;
            font-style: italic;
            font-size: 0.8rem;
        }

        .multiselect-options {
            display: none;
            position: absolute;
            top: 100%;
            left: 0;
            right: 0;
            z-index: 50;
            background: #fff;
            border: 1px solid #ced4da;
            border-radius: 4px;
            box-shadow: 0 4px 12px rgba(0,0,0,0.15);
            max-height: 200px;
            overflow-y: auto;
            margin-top: 2px;
            padding: 4px 0;
        }

        .multiselect-options.show {
            display: block;
        }

        .multiselect-option {
            display: flex;
            align-items: center;
            gap: 8px;
            padding: 6px 12px;
            cursor: pointer;
            font-size: 0.85rem;
        }

        .multiselect-option:hover {
            background-color: #f1f5f9;
        }

        .multiselect-option input {
            cursor: pointer;
            margin: 0;
            accent-color: var(--accent);
        }

        .toast {
            position: fixed;
            bottom: 24px;
            right: 24px;
            padding: 10px 18px;
            border-radius: 1rem;
            color: #fff;
            font-weight: 500;
            font-size: 0.9rem;
            z-index: 1000;
            box-shadow: 0 4px 12px rgba(0,0,0,0.25);
            display: none;
            animation: fadeIn 0.2s ease-in-out;
        }

        .toast.success { background-color: #2e7d32; }
        .toast.error { background-color: #c62828; }
        .toast.info { background-color: var(--main); }

        @keyframes fadeIn {
            from { opacity: 0; transform: translateY(10px); }
            to { opacity: 1; transform: translateY(0); }
        }

        .modal-backdrop {
            position: fixed;
            top: 0;
            left: 0;
            width: 100vw;
            height: 100vh;
            background: rgba(0,0,0,0.6);
            display: none;
            align-items: center;
            justify-content: center;
            z-index: 200;
        }

        .modal-backdrop.show {
            display: flex;
        }

        .modal-content {
            background: #fff;
            width: 90%;
            max-width: 650px;
            border-radius: 6px;
            box-shadow: 0 5px 20px rgba(0,0,0,0.3);
            display: flex;
            flex-direction: column;
            max-height: 85vh;
            overflow: hidden;
        }

        .modal-header {
            padding: 10px 16px;
            background: var(--main);
            color: white;
            display: flex;
            justify-content: space-between;
            align-items: center;
        }

        .modal-header h3 {
            margin: 0;
            font-size: 1rem;
            font-weight: 400;
        }

        .modal-body {
            padding: 16px;
            overflow-y: auto;
            flex: 1;
        }

        .modal-footer {
            padding: 10px 16px;
            border-top: 1px solid #e9ecef;
            display: flex;
            justify-content: flex-end;
            gap: 8px;
            background: #f8f9fa;
        }

        .code-textarea {
            width: 100%;
            height: 250px;
            font-family: monospace;
            font-size: 0.85rem;
            padding: 8px;
            border: 1px solid #ced4da;
            border-radius: 4px;
            resize: vertical;
            white-space: pre;
            outline: none;
        }

        .code-textarea:focus {
            border-color: var(--accent);
        }

        .empty-state {
            text-align: center;
            padding: 40px 20px;
            color: #6c757d;
        }

        .empty-state p {
            margin-bottom: 14px;
            font-size: 1rem;
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
            <h4 class="header-text --hidden-sm">Actions</h4>
        </div>
    </div>
    <div class="flex-center" style="gap: 10px;">
        <button type="button" class="btn btn-light" onclick="location.href='/'">Back</button>
        <button type="button" class="btn btn-accent" onclick="saveEvents()">Save Actions</button>
    </div>
</div>

<div class="main">
    <div class="section-container">
        <span class="section-header">Actions</span>
    </div>

    <div class="toolbar">
        <div class="toolbar-left">
            <button type="button" class="btn btn-dark btn-sm" onclick="addEventRow()">
                <svg xmlns="http://www.w3.org/2000/svg" width="1.2em" height="1.2em" viewBox="0 0 24 24"><title>add-rounded</title><path fill="currentColor" d="M11 13H6q-.425 0-.712-.288T5 12t.288-.712T6 11h5V6q0-.425.288-.712T12 5t.713.288T13 6v5h5q.425 0 .713.288T19 12t-.288.713T18 13h-5v5q0 .425-.288.713T12 19t-.712-.288T11 18z"/></svg>
                Add Action
            </button>
            <button type="button" class="btn btn-dark btn-sm" onclick="openJsonModal('view')">
                <svg xmlns="http://www.w3.org/2000/svg" width="1.2em" height="1.2em" viewBox="0 0 24 24"><title>file-json</title><path fill="currentColor" d="M12.823 15.122c-.517 0-.816.491-.816 1.146c0 .661.311 1.126.82 1.126c.517 0 .812-.49.812-1.146c0-.604-.291-1.126-.816-1.126"/><path fill="currentColor" d="M14 2H6a2 2 0 0 0-2 2v16a2 2 0 0 0 2 2h12a2 2 0 0 0 2-2V8zM8.022 16.704c0 .961-.461 1.296-1.2 1.296c-.176 0-.406-.029-.557-.08l.086-.615c.104.035.239.06.391.06c.319 0 .52-.145.52-.67v-2.122h.761zm1.459 1.291c-.385 0-.766-.1-.955-.205l.155-.631c.204.105.521.211.846.211c.35 0 .534-.146.534-.365c0-.211-.159-.331-.564-.476c-.562-.195-.927-.506-.927-.996c0-.576.481-1.017 1.277-1.017c.38 0 .659.08.861.171l-.172.615c-.135-.065-.375-.16-.705-.16s-.491.15-.491.325c0 .215.19.311.627.476c.596.22.876.53.876 1.006c.001.566-.436 1.046-1.362 1.046m3.306.005c-1.001 0-1.586-.755-1.586-1.716c0-1.012.646-1.768 1.642-1.768c1.035 0 1.601.776 1.601 1.707C14.443 17.33 13.773 18 12.787 18m4.947-.055h-.802l-.721-1.302a13 13 0 0 1-.585-1.19l-.016.005c.021.445.031.921.031 1.472v1.016h-.701v-3.373h.891l.701 1.236c.2.354.4.775.552 1.155h.014c-.05-.445-.065-.9-.065-1.406v-.985h.702zM14 9h-1V4l5 5z"/></svg>
                View JSON
            </button>
            <button type="button" class="btn btn-dark btn-sm" onclick="openJsonModal('import')">
                <svg xmlns="http://www.w3.org/2000/svg" width="1.2em" height="1.2em" viewBox="0 0 24 24"><title>json-fill</title><path fill="currentColor" fill-rule="evenodd" d="M7.835 3.97A1 1 0 0 1 6.865 5c-.41.012-.722.077-.955.17a.87.87 0 0 0-.398.29c-.051.085-.116.263-.116.606V9.23c0 .928-.25 1.782-.84 2.459l-.01.012q-.136.15-.288.281q.159.141.3.306c.592.666.838 1.515.838 2.436v3.231c0 .34.07.514.124.598l.012.019c.062.1.152.183.323.244l.033.013c.23.092.547.158.976.17a1 1 0 0 1-.057 2c-.591-.017-1.147-.11-1.646-.307a2.57 2.57 0 0 1-1.324-1.059c-.322-.5-.441-1.084-.441-1.678v-3.23c0-.568-.147-.9-.337-1.112l-.023-.026c-.18-.214-.53-.438-1.212-.56A1 1 0 0 1 1 12.044v-.132a1 1 0 0 1 .821-.984c.665-.12 1.032-.338 1.233-.558c.198-.231.342-.578.342-1.14V6.067c0-.605.118-1.204.447-1.71l.02-.028a2.86 2.86 0 0 1 1.304-1.015c.5-.2 1.053-.295 1.639-.313a1 1 0 0 1 1.029.97m8.33 0a1 1 0 0 1 1.03-.97c.585.018 1.138.113 1.638.313a2.86 2.86 0 0 1 1.324 1.043c.33.506.447 1.105.447 1.71V9.23c0 .56.144.908.343 1.139c.2.22.567.437 1.232.558a1 1 0 0 1 .821.984v.132a1 1 0 0 1-.824.984c-.682.122-1.033.346-1.212.56l-.023.026c-.19.211-.337.544-.337 1.111v3.231c0 .594-.12 1.179-.44 1.678a2.57 2.57 0 0 1-1.325 1.06c-.499.196-1.055.289-1.646.306a1 1 0 1 1-.057-2c.429-.012.746-.078.976-.17l.029-.011l.004-.002a.58.58 0 0 0 .323-.244l.012-.02c.055-.083.124-.257.124-.597v-3.23c0-.922.246-1.771.839-2.437q.14-.165.3-.306a3 3 0 0 1-.288-.281l-.011-.012c-.59-.677-.84-1.53-.84-2.46V6.067c0-.343-.065-.521-.116-.607a.87.87 0 0 0-.398-.289c-.233-.093-.544-.158-.955-.17a1 1 0 0 1-.97-1.03M9 14a1 1 0 0 1 1 1v1a1 1 0 1 1-2 0v-1a1 1 0 0 1 1-1m3 0a1 1 0 0 1 1 1v1a1 1 0 1 1-2 0v-1a1 1 0 0 1 1-1m3 0a1 1 0 0 1 1 1v1a1 1 0 1 1-2 0v-1a1 1 0 0 1 1-1" clip-rule="evenodd"/></svg>
                Import JSON
            </button>
            <span id="eventCountBadge" style="font-size: 0.85rem; color: #555; font-weight: 600; margin-left: 8px;">0 actions</span>
        </div>
        <div class="toolbar-right">
            <div class="form-field" style="width: 220px;">
                <input type="text" id="searchInput" placeholder="Search actions..." oninput="filterEvents()">
            </div>
        </div>
    </div>

    <div class="table-responsive">
        <table id="eventsTable">
            <thead>
                <tr>
                    <th style="width: 50px; text-align: center;">On</th>
                    <th style="width: 180px;">Name</th>
                    <th style="width: 220px;">Host / Endpoint</th>
                    <th>Payload</th>
                    <th style="width: 200px;">Triggers</th>
                    <th style="width: 170px;">Actions</th>
                    <th style="width: 80px; text-align: center;">Actions</th>
                </tr>
            </thead>
            <tbody id="eventsTableBody">
                <!-- Event rows generated dynamically -->
            </tbody>
        </table>
        <div id="emptyState" class="empty-state" style="display: none;">
            <p>No actions defined yet.</p>
            <button type="button" class="btn btn-dark btn-sm" onclick="addEventRow()">
                <svg xmlns="http://www.w3.org/2000/svg" width="1.2em" height="1.2em" viewBox="0 0 24 24"><title>add-rounded</title><path fill="currentColor" d="M11 13H6q-.425 0-.712-.288T5 12t.288-.712T6 11h5V6q0-.425.288-.712T12 5t.713.288T13 6v5h5q.425 0 .713.288T19 12t-.288.713T18 13h-5v5q0 .425-.288.713T12 19t-.712-.288T11 18z"/></svg>
                Add Your First Action
            </button>
        </div>
    </div>
</div>

<!-- JSON View / Import Modal -->
<div id="jsonModal" class="modal-backdrop">
    <div class="modal-content">
        <div class="modal-header">
            <h3 id="modalTitle">JSON Output</h3>
            <span style="cursor: pointer; font-size: 1.3rem; line-height: 1;" onclick="closeJsonModal()">&times;</span>
        </div>
        <div class="modal-body">
            <p id="modalDesc" style="margin-top: 0; font-size: 0.85rem; color: #666;">Array of actions payload:</p>
            <textarea id="jsonTextarea" class="code-textarea" spellcheck="false"></textarea>
        </div>
        <div class="modal-footer">
            <button id="modalActionBtn" type="button" class="btn btn-dark btn-sm" onclick="handleModalAction()">Copy to Clipboard</button>
            <button type="button" class="btn btn-light btn-sm" onclick="closeJsonModal()">Close</button>
        </div>
    </div>
</div>

<!-- Toast notification -->
<div id="toast" class="toast"></div>

<script>
    const TRIGGER_OPTIONS = [
        'SINGLE_PRESS',
        'SWITCH_ON',
        'SWITCH_OFF',
        'KEYSTORE_UPDATE',
        'REMOTE_TRIGGER'
    ];
    const ACTION_OPTIONS = [
        'HTTP_REQUEST',
        'SERIAL_DATA',
        'OI_OUTPUT'
    ];

    let eventsData = [];
    let modalMode = 'view';

    const DEFAULT_SAMPLE_EVENTS = [
        {
            name: "Toggle Light",
            host: "http://192.168.1.50/api/relay/toggle",
            payload: "{\"action\": \"toggle\"}",
            triggers: ["SINGLE_PRESS"],
            actions: ["HTTP_REQUEST"],
            enabled: true,
            scheduler: null
        },
        {
            name: "Notify Switch On",
            host: "http://192.168.1.10/notify",
            payload: "SWITCH ON OCCURRED",
            triggers: ["SWITCH_ON"],
            actions: ["HTTP_REQUEST", "SERIAL_DATA"],
            enabled: true,
            scheduler: null
        }
    ];

    document.addEventListener('DOMContentLoaded', () => {
        loadEvents();
        document.addEventListener('click', handleGlobalClick);
    });

    async function loadEvents() {
        try {
            const res = await fetch('/actions');
            if (res.ok) {
                const data = await res.json();
                if (Array.isArray(data)) {
                    eventsData = data;
                } else if (data.result && Array.isArray(data.result)) {
                    eventsData = data.result;
                } else if (data.actions && Array.isArray(data.actions)) {
                    eventsData = data.actions;
                } else {
                    eventsData = DEFAULT_SAMPLE_EVENTS;
                }
            } else {
                eventsData = DEFAULT_SAMPLE_EVENTS;
            }
        } catch (e) {
            console.warn("Could not fetch /actions, using sample data:", e);
            eventsData = DEFAULT_SAMPLE_EVENTS;
        }
        renderTable();
    }

    async function saveEvents() {
        syncEventsFromInputs();
        const payload = getEventsOutput();
        showToast("Saving actions...", "info");

        try {
            const res = await fetch('/actions', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify(payload)
            });

            if (res.ok) {
                const json = await res.json().catch(() => ({}));
                const msg = json.result || "Actions saved successfully!";
                showToast(msg, "success");
            } else {
                const json = await res.json().catch(() => ({}));
                const errMsg = json.error || `Server returned error (${res.status})`;
                showToast(errMsg, "error");
            }
        } catch (err) {
            console.error("Save error:", err);
            showToast("Network error: Could not save actions", "error");
        }
    }

    function renderTable() {
        const tbody = document.getElementById('eventsTableBody');
        tbody.innerHTML = '';

        const emptyState = document.getElementById('emptyState');
        const countBadge = document.getElementById('eventCountBadge');
        countBadge.innerText = `${eventsData.length} action${eventsData.length === 1 ? '' : 's'}`;

        if (eventsData.length === 0) {
            emptyState.style.display = 'block';
            return;
        }
        emptyState.style.display = 'none';

        eventsData.forEach((event, index) => {
            const tr = document.createElement('tr');
            if (!event.enabled) tr.classList.add('row-disabled');
            tr.dataset.index = index;

            const tdEnabled = document.createElement('td');
            tdEnabled.style.textAlign = 'center';
            tdEnabled.innerHTML = `
                <div class="checkbox-form-field">
                    <input type="checkbox" class="modern-checkbox" ${event.enabled ? 'checked' : ''} onchange="updateEventField(${index}, 'enabled', this.checked)">
                </div>
            `;

            const tdName = document.createElement('td');
            tdName.innerHTML = `<input type="text" class="table-control" value="${escapeHtml(event.name || '')}" placeholder="Action Name" oninput="updateEventField(${index}, 'name', this.value)">`;

            const tdHost = document.createElement('td');
            tdHost.innerHTML = `<input type="text" class="table-control" value="${escapeHtml(event.host || '')}" placeholder="http://... or host" oninput="updateEventField(${index}, 'host', this.value)">`;

            const tdPayload = document.createElement('td');
            tdPayload.innerHTML = `<input type="text" class="table-control" value="${escapeHtml(event.payload || '')}" placeholder="Payload string" oninput="updateEventField(${index}, 'payload', this.value)">`;

            const tdTriggers = document.createElement('td');
            tdTriggers.appendChild(createMultiSelect(index, 'triggers', TRIGGER_OPTIONS, event.triggers || [], 'badge-trigger', 'Select Triggers'));

            const tdActions = document.createElement('td');
            tdActions.appendChild(createMultiSelect(index, 'actions', ACTION_OPTIONS, event.actions || [], 'badge-action', 'Select Actions'));

            const tdControls = document.createElement('td');
            tdControls.style.textAlign = 'center';
            tdControls.innerHTML = `
                <div style="display: flex; gap: 6px; justify-content: center;">
                    <button type="button" class="btn btn-dark btn-icon" title="Duplicate action" onclick="duplicateEvent(${index})">
                        <svg xmlns="http://www.w3.org/2000/svg" width="1em" height="1em" viewBox="0 0 512 512"><title>duplicate</title><path fill="currentColor" d="M408 112H184a72 72 0 0 0-72 72v224a72 72 0 0 0 72 72h224a72 72 0 0 0 72-72V184a72 72 0 0 0-72-72m-32.45 200H312v63.55c0 8.61-6.62 16-15.23 16.43A16 16 0 0 1 280 376v-64h-63.55c-8.61 0-16-6.62-16.43-15.23A16 16 0 0 1 216 280h64v-63.55c0-8.61 6.62-16 15.23-16.43A16 16 0 0 1 312 216v64h64a16 16 0 0 1 16 16.77c-.42 8.61-7.84 15.23-16.45 15.23"/><path fill="currentColor" d="M395.88 80A72.12 72.12 0 0 0 328 32H104a72 72 0 0 0-72 72v224a72.12 72.12 0 0 0 48 67.88V160a80 80 0 0 1 80-80Z"/></svg>
                    </button>
                    <button type="button" class="btn btn-red btn-icon" title="Remove action" onclick="removeEvent(${index})">
                        <svg xmlns="http://www.w3.org/2000/svg" width="1em" height="1em" viewBox="0 0 24 24"><title>delete</title><path fill="currentColor" d="M7 21q-.825 0-1.412-.587T5 19V6q-.425 0-.712-.288T4 5t.288-.712T5 4h4q0-.425.288-.712T10 3h4q.425 0 .713.288T15 4h4q.425 0 .713.288T20 5t-.288.713T19 6v13q0 .825-.587 1.413T17 21zM17 6H7v13h10zm-6.287 10.713Q11 16.425 11 16V9q0-.425-.288-.712T10 8t-.712.288T9 9v7q0 .425.288.713T10 17t.713-.288m4 0Q15 16.426 15 16V9q0-.425-.288-.712T14 8t-.712.288T13 9v7q0 .425.288.713T14 17t.713-.288M7 6v13z"/></svg>
                    </button>
                </div>
            `;

            tr.appendChild(tdEnabled);
            tr.appendChild(tdName);
            tr.appendChild(tdHost);
            tr.appendChild(tdPayload);
            tr.appendChild(tdTriggers);
            tr.appendChild(tdActions);
            tr.appendChild(tdControls);

            tbody.appendChild(tr);
        });
    }

    function createMultiSelect(eventIndex, fieldKey, allOptions, selectedValues, badgeClass, placeholderText) {
        const container = document.createElement('div');
        container.className = 'multiselect';
        container.dataset.eventIndex = eventIndex;
        container.dataset.fieldKey = fieldKey;

        const selectBox = document.createElement('div');
        selectBox.className = 'multiselect-select';
        renderSelectBoxBadges(selectBox, selectedValues, badgeClass, placeholderText);

        const optionsDropdown = document.createElement('div');
        optionsDropdown.className = 'multiselect-options';

        allOptions.forEach(opt => {
            const optLabel = document.createElement('label');
            optLabel.className = 'multiselect-option';
            const isChecked = selectedValues.includes(opt);

            optLabel.innerHTML = `
                <input type="checkbox" value="${opt}" ${isChecked ? 'checked' : ''}>
                <span>${opt}</span>
            `;

            const chk = optLabel.querySelector('input');
            chk.addEventListener('change', (e) => {
                e.stopPropagation();
                let current = eventsData[eventIndex][fieldKey] || [];
                if (chk.checked) {
                    if (!current.includes(opt)) current.push(opt);
                } else {
                    current = current.filter(x => x !== opt);
                }
                eventsData[eventIndex][fieldKey] = current;
                renderSelectBoxBadges(selectBox, current, badgeClass, placeholderText);
            });

            optionsDropdown.appendChild(optLabel);
        });

        selectBox.addEventListener('click', (e) => {
            e.stopPropagation();
            const isOpen = optionsDropdown.classList.contains('show');
            closeAllDropdowns();
            if (!isOpen) {
                optionsDropdown.classList.add('show');
                selectBox.classList.add('active');
            }
        });

        container.appendChild(selectBox);
        container.appendChild(optionsDropdown);
        return container;
    }

    function renderSelectBoxBadges(selectBox, selectedValues, badgeClass, placeholderText) {
        selectBox.innerHTML = '';
        if (!selectedValues || selectedValues.length === 0) {
            selectBox.innerHTML = `<span class="multiselect-placeholder">${placeholderText}</span>`;
        } else {
            selectedValues.forEach(val => {
                const badge = document.createElement('span');
                badge.className = `badge ${badgeClass}`;
                badge.innerText = val;
                selectBox.appendChild(badge);
            });
        }
    }

    function closeAllDropdowns() {
        document.querySelectorAll('.multiselect-options.show').forEach(el => el.classList.remove('show'));
        document.querySelectorAll('.multiselect-select.active').forEach(el => el.classList.remove('active'));
    }

    function handleGlobalClick(e) {
        if (!e.target.closest('.multiselect')) {
            closeAllDropdowns();
        }
    }

    function updateEventField(index, field, value) {
        if (!eventsData[index]) return;
        eventsData[index][field] = value;
        if (field === 'enabled') {
            const tr = document.querySelector(`tr[data-index="${index}"]`);
            if (tr) {
                if (value) tr.classList.remove('row-disabled');
                else tr.classList.add('row-disabled');
            }
        }
    }

    function addEventRow() {
        eventsData.push({
            name: "New Action",
            host: "http://192.168.1.1/api",
            payload: "",
            triggers: ["SINGLE_PRESS"],
            actions: ["HTTP_REQUEST"],
            enabled: true,
            scheduler: null
        });
        renderTable();
        const tbody = document.getElementById('eventsTableBody');
        if (tbody.lastElementChild) {
            tbody.lastElementChild.scrollIntoView({ behavior: 'smooth', block: 'nearest' });
            const nameInput = tbody.lastElementChild.querySelector('input[type="text"]');
            if (nameInput) nameInput.focus();
        }
        showToast("New action added", "info");
    }

    function duplicateEvent(index) {
        if (!eventsData[index]) return;
        const copy = JSON.parse(JSON.stringify(eventsData[index]));
        copy.name += " (Copy)";
        eventsData.splice(index + 1, 0, copy);
        renderTable();
        showToast("Action duplicated", "info");
    }

    function removeEvent(index) {
        if (!confirm(`Are you sure you want to remove action "${eventsData[index].name || 'Action'}"?`)) return;
        eventsData.splice(index, 1);
        renderTable();
        showToast("Action removed", "info");
    }

    function syncEventsFromInputs() {
    }

    function filterEvents() {
        const query = document.getElementById('searchInput').value.toLowerCase().trim();
        const rows = document.querySelectorAll('#eventsTableBody tr');
        rows.forEach(tr => {
            const idx = tr.dataset.index;
            const ev = eventsData[idx];
            if (!ev) return;
            const matchName = (ev.name || '').toLowerCase().includes(query);
            const matchHost = (ev.host || '').toLowerCase().includes(query);
            const matchPayload = (ev.payload || '').toLowerCase().includes(query);
            const matchTriggers = (ev.triggers || []).some(t => t.toLowerCase().includes(query));
            const matchActions = (ev.actions || []).some(a => a.toLowerCase().includes(query));

            if (matchName || matchHost || matchPayload || matchTriggers || matchActions) {
                tr.style.display = '';
            } else {
                tr.style.display = 'none';
            }
        });
    }

    function getEventsOutput() {
        return eventsData.map(ev => ({
            name: String(ev.name || ""),
            host: String(ev.host || ""),
            payload: String(ev.payload || ""),
            triggers: Array.isArray(ev.triggers) ? ev.triggers : [],
            actions: Array.isArray(ev.actions) ? ev.actions : [],
            enabled: Boolean(ev.enabled),
            scheduler: ev.scheduler !== undefined ? ev.scheduler : null
        }));
    }

    function openJsonModal(mode) {
        modalMode = mode;
        const modal = document.getElementById('jsonModal');
        const title = document.getElementById('modalTitle');
        const desc = document.getElementById('modalDesc');
        const textarea = document.getElementById('jsonTextarea');
        const actionBtn = document.getElementById('modalActionBtn');

        if (mode === 'view') {
            title.innerText = "Export Actions JSON";
            desc.innerText = "Output array of actions (read-only):";
            textarea.value = JSON.stringify(getEventsOutput(), null, 2);
            textarea.readOnly = true;
            actionBtn.innerText = "Copy to Clipboard";
        } else {
            title.innerText = "Import Actions JSON";
            desc.innerText = "Paste an array of actions JSON to import:";
            textarea.value = "";
            textarea.readOnly = false;
            actionBtn.innerText = "Apply Import";
        }

        modal.classList.add('show');
    }

    function closeJsonModal() {
        document.getElementById('jsonModal').classList.remove('show');
    }

    function handleModalAction() {
        const textarea = document.getElementById('jsonTextarea');
        if (modalMode === 'view') {
            navigator.clipboard.writeText(textarea.value).then(() => {
                showToast("JSON copied to clipboard!", "success");
                closeJsonModal();
            }).catch(() => {
                textarea.select();
                document.execCommand('copy');
                showToast("JSON copied to clipboard!", "success");
                closeJsonModal();
            });
        } else {
            try {
                const parsed = JSON.parse(textarea.value.trim());
                if (!Array.isArray(parsed)) {
                    alert("Import error: Root structure must be a JSON Array []");
                    return;
                }
                eventsData = parsed.map(item => ({
                    name: item.name || "Imported Action",
                    host: item.host || "",
                    payload: item.payload || "",
                    triggers: Array.isArray(item.triggers) ? item.triggers : ["SINGLE_PRESS"],
                    actions: Array.isArray(item.actions) ? item.actions : ["HTTP_REQUEST"],
                    enabled: item.enabled !== undefined ? Boolean(item.enabled) : true,
                    scheduler: item.scheduler || null
                }));
                renderTable();
                closeJsonModal();
                showToast(`Successfully imported ${eventsData.length} actions!`, "success");
            } catch (err) {
                alert("Invalid JSON format: " + err.message);
            }
        }
    }

    function showToast(message, type = 'info') {
        const toast = document.getElementById('toast');
        toast.className = `toast ${type}`;
        toast.innerText = message;
        toast.style.display = 'block';
        clearTimeout(toast._timeout);
        toast._timeout = setTimeout(() => {
            toast.style.display = 'none';
        }, 3000);
    }

    function escapeHtml(text) {
        return String(text)
            .replace(/&/g, "&amp;")
            .replace(/</g, "&lt;")
            .replace(/>/g, "&gt;")
            .replace(/"/g, "&quot;")
            .replace(/'/g, "&#039;");
    }
</script>

</body>
</html>
)rawliteral";

#endif //EVENT_BUTTON_ACTION_EDITOR_HPP
