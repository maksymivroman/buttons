#!/usr/bin/env python3
"""
Local Development Server for Event Button
- Replaces %COMPONENT% placeholders dynamically from src/HTMLPage/html-page.hpp
- Serves all frontend pages (/, /flags, /logs, /server/editor, /dashboard)
- Provides mock API endpoints (/status, /settings, /networks, /events, /flagsData, etc.)
"""

import http.server
import json
import os
import re
import sys
import urllib.parse

PORT = 8080
BASE_DIR = os.path.dirname(os.path.abspath(__file__))
HTML_PAGE_HPP = os.path.join(BASE_DIR, "src", "HTMLPage", "html-page.hpp")

# In-memory mock state for dev server testing
MOCK_STATE = {
    "flags": {
        "ledRDisabled": False,
        "ledGDisabled": False,
        "ledBDisabled": False
    },
    "settings": {
        "wifiSsid": "DevNet_5G",
        "wifiPass": "secret123",
        "useDnsName": True,
        "loggerEnabled": True,
        "loggerLevel": 3,
        "wiFiMode": 1,
        "timezone": 2,
        "useSound": True,
        "statisticEnabled": False,
        "statisticLevel": 1,
        "statisticApi": "https://api.example.com/events",
        "saveLastState": True,
        "overrideLedConfig": False,
        "serialEvents": True,
        "customServer": True,
        "remoteTriggering": False,
        "restoreLastStateOnLoad": False,
        "keystoreEnabled": True,
        "remoteStateChange": False,
        "sendEventOnKeystoreUpdate": False,
        "delaySendEvents": False,
        "clientWebAccess": True,
        "enableOtaUpdate": True,
        "hotspotSsid": "EventButton-AP",
        "customHSsid": False,
        "serverPath": "/dashboard",
        "options": {
            "loggerLevels": [
                {"value": 0, "label": "OFF"},
                {"value": 1, "label": "ERROR"},
                {"value": 2, "label": "WARN"},
                {"value": 3, "label": "INFO"},
                {"value": 4, "label": "DEBUG"}
            ],
            "wiFiModes": [
                {"value": 1, "label": "Station (Client)"},
                {"value": 2, "label": "Access Point"}
            ],
            "timezones": [
                {"value": 0, "label": "UTC 0"},
                {"value": 2, "label": "UTC +2 (Kyiv)"},
                {"value": 3, "label": "UTC +3"}
            ],
            "statisticLevels": [
                {"value": 1, "label": "Basic"},
                {"value": 2, "label": "Detailed"}
            ]
        }
    },
    "events": {
        "S01": "PRESS:CLICK",
        "S02": "HOLD:2000",
        "N01": "SCENE:OFFICE_LIGHTS_TOGGLE"
    },
    "logs": {
        "100": "[INIT] DevServer started in mock mode",
        "101": "[WIFI] Connected to DevNet_5G (RSSI: -52 dBm)",
        "102": "[HTTP] Web server listening on port 8080",
        "103": "[FLAGS] Loaded flags: ledR=0, ledG=0, ledB=0"
    }
}


def load_components_from_hpp():
    """Extracts Components from src/HTMLPage/html-page.hpp namespace Components."""
    components = {}
    if os.path.exists(HTML_PAGE_HPP):
        try:
            with open(HTML_PAGE_HPP, "r", encoding="utf-8") as f:
                content = f.read()

            # Find Components namespace block
            ns_match = re.search(r"namespace\s+Components\s*\{(.*?)\}", content, re.DOTALL)
            if ns_match:
                block = ns_match.group(1)
                matches = re.findall(r'const\s+char\s+(\w+)\[\]\s+PROGMEM\s*=\s*R"rawliteral\((.*?)\)rawliteral";', block, re.DOTALL)
                for name, body in matches:
                    components[name] = body.strip()
        except Exception as e:
            print(f"[WARN] Error reading {HTML_PAGE_HPP}: {e}")

    return components


class DevServerHandler(http.server.SimpleHTTPRequestHandler):
    def send_json(self, data, code=200):
        body = json.dumps(data).encode("utf-8")
        self.send_response(code)
        self.send_header("Content-Type", "application/json; charset=utf-8")
        self.send_header("Content-Length", str(len(body)))
        self.send_header("Access-Control-Allow-Origin", "*")
        self.end_headers()
        self.wfile.write(body)

    def send_text(self, text, code=200, content_type="text/plain"):
        body = text.encode("utf-8")
        self.send_response(code)
        self.send_header("Content-Type", f"{content_type}; charset=utf-8")
        self.send_header("Content-Length", str(len(body)))
        self.send_header("Access-Control-Allow-Origin", "*")
        self.end_headers()
        self.wfile.write(body)

    def serve_html_with_components(self, filepath):
        if not os.path.exists(filepath):
            self.send_error(404, f"File not found: {filepath}")
            return

        with open(filepath, "r", encoding="utf-8") as f:
            content = f.read()

        # Dynamic replacement of components
        components = load_components_from_hpp()
        for name, value in components.items():
            content = content.replace(f"%{name}%", value)

        self.send_text(content, 200, "text/html")

    def do_GET(self):
        url = urllib.parse.urlparse(self.path)
        path = url.path

        # 1. HTML Routes
        if path in ("/", "/index", "/index.html"):
            return self.serve_html_with_components(os.path.join(BASE_DIR, "index.html"))

        if path in ("/flags", "/flags.html"):
            return self.serve_html_with_components(os.path.join(BASE_DIR, "flags.html"))

        if path in ("/logs", "/logs.html"):
            return self.serve_html_with_components(os.path.join(BASE_DIR, "logs.html"))

        if path in ("/server/editor", "/editor", "/editor.html"):
            editor_path = os.path.join(BASE_DIR, "src", "editor.html")
            if not os.path.exists(editor_path):
                editor_path = os.path.join(BASE_DIR, "editor.html")
            return self.serve_html_with_components(editor_path)

        if path in ("/dashboard", "/dashboard.html"):
            return self.serve_html_with_components(os.path.join(BASE_DIR, "dashboard.html"))

        # 2. Mock API Routes
        if path == "/status":
            return self.send_json({
                "status": 200,
                "error": None,
                "result": {
                    "fwVersion": "2.4.0-dev",
                    "hwVersion": "ESP8266-12E",
                    "mac": "5C:CF:7F:12:34:56",
                    "ip": f"localhost:{PORT}",
                    "heap": 48200,
                    "ksKeys": 3,
                    "toggleState": "RELEASED",
                    "wiFiMode": "STA",
                    "rgbFlags": {
                        "rDisabled": MOCK_STATE["flags"]["ledRDisabled"],
                        "gDisabled": MOCK_STATE["flags"]["ledGDisabled"],
                        "bDisabled": MOCK_STATE["flags"]["ledBDisabled"]
                    }
                }
            })

        if path == "/settings":
            return self.send_json({"success": True, "result": MOCK_STATE["settings"]})

        if path == "/networks":
            return self.send_json({
                "success": True,
                "result": [
                    {"ssid": "DevNet_5G", "rssi": -48, "secure": True},
                    {"ssid": "Office_WiFi", "rssi": -65, "secure": True},
                    {"ssid": "Guest_Open", "rssi": -78, "secure": False}
                ]
            })

        if path in ("/events", "/eventsV2"):
            return self.send_json({"success": True, "result": MOCK_STATE["events"]})

        if path == "/flagsData":
            return self.send_json({"success": True, "result": MOCK_STATE["flags"]})

        if path == "/logsData":
            return self.send_json(MOCK_STATE["logs"])

        if path == "/server/load-config":
            return self.send_json({"path": MOCK_STATE["settings"]["serverPath"]})

        if path == "/server/load-html":
            return self.send_text("<h1>Hello from Custom Server</h1>")

        # Static assets fallback
        return super().do_GET()

    def do_POST(self):
        url = urllib.parse.urlparse(self.path)
        path = url.path
        content_length = int(self.headers.get("Content-Length", 0))
        post_body = self.rfile.read(content_length).decode("utf-8")

        if path == "/flags":
            parsed = urllib.parse.parse_qs(post_body)
            for k in ("ledRDisabled", "ledGDisabled", "ledBDisabled"):
                if k in parsed:
                    val = parsed[k][0]
                    MOCK_STATE["flags"][k] = val in ("1", "true", "True")
            print(f"[FLAGS POST] Updated Flags: {MOCK_STATE['flags']}")
            return self.send_text("Flags updated!", 200)

        if path in ("/updateData", "/settings", "/save"):
            print(f"[{path} POST] Received payload: {post_body}")
            return self.send_text("OK", 200)

        self.send_error(404, f"Unknown POST endpoint: {path}")


def main():
    if sys.stdout.encoding.lower() != 'utf-8':
        sys.stdout.reconfigure(encoding='utf-8', errors='replace')

    components = load_components_from_hpp()
    print("=" * 60)
    print(f"[*] Event Button Dev Server running at http://localhost:{PORT}")
    print("=" * 60)
    print("Available Pages:")
    print(f"  - Setup / Home:      http://localhost:{PORT}/")
    print(f"  - Flags Page:        http://localhost:{PORT}/flags")
    print(f"  - Logs Page:         http://localhost:{PORT}/logs")
    print(f"  - Server Editor:     http://localhost:{PORT}/server/editor")
    print(f"  - Dashboard:         http://localhost:{PORT}/dashboard")
    print("-" * 60)
    print(f"Registered SSR Components ({len(components)}):")
    for name in sorted(components.keys()):
        print(f"  - %{name}%")
    print("=" * 60)
    print("Press Ctrl+C to stop the server.\n")

    httpd = http.server.HTTPServer(("0.0.0.0", PORT), DevServerHandler)
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        print("\nDev server stopped.")
        sys.exit(0)


if __name__ == "__main__":
    main()

