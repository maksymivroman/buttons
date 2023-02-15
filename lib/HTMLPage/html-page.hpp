#ifndef EVENT_BUTTON_HTML_PAGE_HPP
#define EVENT_BUTTON_HTML_PAGE_HPP

#include <Arduino.h>

extern const char index_html[] PROGMEM = R"rawliteral(
  <!DOCTYPE html>
  <html lang="en">
  <head>
      <meta name="viewport" content="width=device-width, initial-scale=1">
      <meta charset="UTF-8">
      <title>button config page</title>
      <style>
          html{ color: #333333; font-size: 16px}
          .container{ background: #565f60; display: flex; flex-direction: column; align-items: center;}
          .wifi-credentials{ display: flex; flex-direction: row; justify-content: center; }
          .main-container{display: flex; flex-direction: column; width: 20vw; margin: 1rem}
          .control {
              display: block;
              width: 100%;
              min-width: 220px;
              padding: .375rem .75rem;
              font-size: 1rem;
              line-height: 1.5;
              color: #495057;
              background-color: #fff;
              background-clip: padding-box;
              border: 1px solid #ced4da;
              border-radius: .25rem;
              transition: border-color .15s ease-in-out,box-shadow .15s ease-in-out;
          }
          .btn {
              color: #fff;
              background-color: #cb1d38;
              display: inline-block;
              font-weight: 400;
              text-align: center;
              white-space: nowrap;
              vertical-align: middle;
              -webkit-user-select: none;
              -moz-user-select: none;
              -ms-user-select: none;
              user-select: none;
              border: 1px solid #cb1d38;
              padding: .375rem .75rem;
              font-size: 1rem;
              line-height: 1.5;
              border-radius: .25rem;
              transition: color .15s ease-in-out,background-color .15s ease-in-out,border-color .15s ease-in-out,box-shadow .15s ease-in-out;
          }
          .btn:hover{
            background-color: #a6132f;
            transition: all 200ms;
          }
          .label {
              color: white;
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
          @media screen and (max-width: 1080px) {
              .wifi-credentials{flex-direction: column}
              .main-container{width: 60vw}
          }
      </style>

  </head>
  <body class="container">
      <h1 style="color:#ddf2ff">Button Setup Page</h1>

      <div class="wifi-credentials">
        %BUTTONPLACEHOLDER%
      </div>

      <div class="main-container" style="width: 60vw;">
          <textarea id="saved" cols=120 rows=15  class="control" style="margin-bottom: 1rem"></textarea>
          <button type="button" class="btn" onclick="sentcontent()">GET Save And Reboot</button>
          <p></p>
          <button type="button" class="btn" onclick="sentPOSTcontent()">POST Save And Reboot</button>
      </div>
  </body>
  </html>

  <script>
      function sentcontent(){
          var name = document.getElementById('wifiname').value;
          var pass = document.getElementById('wifipass').value;

          data='/get?inputdata={ "inputdata" :{"wifiname":"' + name + '","wifipass":"'+ pass +'","eventdata":'+ document.getElementById('saved').value + '}';
          data.replace(/" /g, '');
          data.replace(/ "/g, '');
          window.location.href=data;
      }

          function sentPOSTcontent(){
          var name = document.getElementById('wifiname').value;
          var pass = document.getElementById('wifipass').value;

          data='{ "inputdata" :{"wifiname":"' + name + '","wifipass":"'+ pass +'","eventdata":'+ document.getElementById('saved').value + '}';
          data.replace(/" /g, '');
          data.replace(/ "/g, '');
          srvURL=window.location.protocol + "//" + window.location.host + "/";
            var pst = new XMLHttpRequest();
            pst.open("POST", srvURL, true);
            pst.setRequestHeader("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
            pst.send(data);
            pst.responseType = 'text';
              pst.onreadystatechange = function() {
                if (pst.readyState === pst.DONE) {
          if (pst.status === 200) {
              console.log(pst.response);
              window.location.href="/save";
              //console.log(pst.responseText);
          }
      }
            }
          //window.location.href="/save";
      }

      function showJSON() {
            var wifiname = document.getElementById('wifiname').value;
          var wifipass = document.getElementById('wifipass').value;
          var h1 = document.getElementById('host1').value;
          var h2 = document.getElementById('host2').value;
          var h3 = document.getElementById('host3').value;
          var e1 = document.getElementById('event1').value;
          var e2 = document.getElementById('event2').value;
          var e3 = document.getElementById('event3').value;
              var ugly ='{ "inputdata" :{"wifiname":"' + wifiname + '","wifipass":"'+ wifipass +'","eventdata": {"'+
              h1 +'":"' + e1 +'",'+
              '"' + h2 +'":"'  + e2 +'",'+
              '"' + h3 +'":"' + e3 +'"}}}';
              var obj = JSON.parse(ugly);
              var pretty = JSON.stringify(obj, undefined, 4);
              document.getElementById('myTextArea').value = pretty;
          }
      function showSaved() {

              var obj = JSON.parse(document.getElementById('savedJSON').innerHTML);
              var showJSON = JSON.stringify(obj, undefined, 4);
              document.getElementById('saved').value = showJSON;
        }

        function update() {
          var select = document.getElementById('networks');
          var option = select.options[select.selectedIndex];
          document.getElementById('wifiname').value = option.value;
        }
          window.onload = showSaved;
  </script>)rawliteral";

#endif //EVENT_BUTTON_HTML_PAGE_HPP
