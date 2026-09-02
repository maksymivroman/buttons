#ifndef EVENT_BUTTON_SERVER_EDITOR_PAGE_HPP
#define EVENT_BUTTON_SERVER_EDITOR_PAGE_HPP

#include <Arduino.h>

const char editor_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="uk">
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta charset="UTF-8">
    <title>Event button | Server Editor</title>
    <style>
        :root { --blue: #1e5e9d; --red: #cb1d38; --bg: #eeeaea; }

        body {
            font-family: Arial, Helvetica, sans-serif;
            margin: 0;
            background: var(--bg);
            display: flex;
            flex-direction: column;
            height: 100vh;
        }

        .header {
            height: 55px;
            background-color: var(--blue);
            padding: 0 16px;
            display: flex;
            align-items: center;
            justify-content: space-between;
            color: white;
            box-shadow: 0 2px 4px rgba(0,0,0,0.2);
            z-index: 10;
        }

        .header-title { display: flex; align-items: center; gap: 10px; font-weight: 300; }
        .header-title h2 { margin: 0; font-size: 1.2rem; }
        .header-title h4 { margin: 0; color: lightgray; font-size: 1rem; }

        .btn {
            color: #fff;
            border: none;
            padding: 8px 16px;
            font-size: 0.95rem;
            border-radius: 4px;
            cursor: pointer;
            transition: 0.2s;
        }
        .btn:disabled { background-color: gray; cursor: not-allowed; }
        .btn-red { background-color: var(--red); }
        .btn-red:hover:not(:disabled) { background-color: #a6132f; }

        .content {
            display: flex;
            flex-direction: column;
            padding: 16px;
            flex-grow: 1;
            overflow: hidden;
            gap: 16px;
        }

        .border {
            background: white;
            border: 1px solid lightgray;
            border-radius: 5px;
            box-shadow: 0 2px 7px #13537a24;
            padding: 16px;
        }

        .settings-row {
            display: flex;
            align-items: center;
            gap: 12px;
            flex-wrap: wrap;
        }

        .control {
            padding: 8px 12px;
            font-size: 1rem;
            border: 1px solid #ced4da;
            border-radius: 4px;
            min-width: 250px;
        }
        .control:focus { outline: none; border-color: var(--blue); }

        .editor-container {
            flex-grow: 1;
            display: flex;
            flex-direction: column;
            padding: 0;
            overflow: hidden;
        }

        .editor-header {
            padding: 10px 16px;
            background: #f8f9fa;
            border-bottom: 1px solid lightgray;
            font-weight: bold;
            color: #333;
            display: flex;
            justify-content: space-between;
            align-items: center;
        }

        .char-count {
            font-weight: normal;
            font-size: 0.8rem;
            color: #666;
            background: #e9ecef;
            padding: 2px 8px;
            border-radius: 10px;
        }

        #codeEditor {
            flex-grow: 1;
            width: 100%;
            border: none;
            padding: 16px;
            font-family: 'Courier New', Courier, monospace;
            font-size: 14px;
            line-height: 1.5;
            resize: none;
            background: #1e1e1e;
            color: #d4d4d4;
            box-sizing: border-box;
        }
        #codeEditor:focus { outline: none; }

        #status { font-size: 0.85rem; }
    </style>
</head>
<body>

<div class="header">
    <div class="header-title">
        ^DEVICE_LOGO^
        <h2>Event button</h2>
        <h4>| Server Editor</h4>
    </div>
    <div>
        <button id="saveBtn" class="btn btn-red">Save & Apply</button>
        <button class="btn btn-red" onclick="location.href='/'">Back</button>
    </div>
</div>

<div class="content">
    <div class="border settings-row">
        <label for="dashPath" style="font-weight: bold; color: #333;">Server URL:</label>
        <input type="text" id="dashPath" class="control" value="/dashboard" placeholder="server url">
        <a id="previewLink" href="/dashboard" target="_blank" style="color: var(--blue); margin-left: auto;">Open ↗</a>
    </div>

    <div class="border editor-container">
        <div class="editor-header">
            <div>
                HTML Code
                <span id="charCount" class="char-count">0 chars</span>
            </div>
            <span id="status" style="color: #667;">Ready</span>
        </div>
        <textarea id="codeEditor" spellcheck="false" placeholder="Put HTML code here..."></textarea>
    </div>
</div>

<script>
    const pathInput = document.getElementById('dashPath');
    const editor = document.getElementById('codeEditor');
    const saveBtn = document.getElementById('saveBtn');
    const statusTxt = document.getElementById('status');
    const charCountTxt = document.getElementById('charCount');
    const previewLink = document.getElementById('previewLink');

    editor.addEventListener('keydown', function(e) {
        if (e.key === 'Tab') {
            e.preventDefault();
            const start = this.selectionStart;
            const end = this.selectionEnd;

            this.value = this.value.substring(0, start) + "    " + this.value.substring(end);

            this.selectionStart = this.selectionEnd = start + 4;
            updateCharCount();
        }
    });

    function updateCharCount() {
        const len = editor.value.length;
        charCountTxt.innerText = len.toLocaleString() + " chars";

        if (len > 32768) {
            charCountTxt.style.background = "#f8d7da";
            charCountTxt.style.color = "#721c24";
        } else {
            charCountTxt.style.background = "#e9ecef";
            charCountTxt.style.color = "#666";
        }
    }

    editor.addEventListener('input', updateCharCount);

    pathInput.addEventListener('input', () => {
        let val = pathInput.value.trim();
        if(!val.startsWith('/')) val = '/' + val;
        previewLink.href = val;
    });

    function loadData() {
        statusTxt.innerText = "Loading config...";

        fetch('/server/load-config')
            .then(res => res.json())
            .then(data => {
                if(data.path) {
                    pathInput.value = data.path;
                    previewLink.href = data.path;
                }

                statusTxt.innerText = "Loading HTML...";
                return fetch('/server/load-html');
            })
            .then(res => res.text())
            .then(html => {
                editor.value = html;
                updateCharCount();
                statusTxt.innerText = "Done";
            })
            .catch(err => {
                statusTxt.innerText = "New File or Error";
                console.error(err);
            });
    }

    saveBtn.onclick = () => {
        let pathVal = pathInput.value.trim() || "/dashboard";
        if(!pathVal.startsWith('/')) pathVal = '/' + pathVal;

        const htmlContent = editor.value;
        const blob = new Blob([htmlContent], { type: 'text/html' });
        const formData = new FormData();

        formData.append("file", blob, "index.html");

        saveBtn.disabled = true;
        statusTxt.innerText = "Uploading (" + Math.round(blob.size / 1024) + " KB)...";

        fetch(`/server/upload?path=${encodeURIComponent(pathVal)}`, {
            method: 'POST',
            body: formData
        })
            .then(res => {
                if (res.ok) {
                    statusTxt.innerText = "Saved!";
                    statusTxt.style.color = "green";
                } else throw new Error();
            })
            .catch(() => {
                statusTxt.innerText = "Upload Failed!";
                statusTxt.style.color = "red";
            })
            .finally(() => {
                saveBtn.disabled = false;
                setTimeout(() => {
                    statusTxt.style.color = "#667";
                    if(statusTxt.innerText === "Saved!") statusTxt.innerText = "Ready";
                }, 3000);
            });
    };

    loadData();
</script>
</body>
</html>
)rawliteral";

#endif //EVENT_BUTTON_SERVER_EDITOR_PAGE_HPP
