const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <title>SoyoSource-PowerController</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    table { width: 100%;}
    table.font1{ font-size: clamp(0.9rem, 0.9vw, 1.2rem); }
    td { text-align: left; }
    th { text-align: center; }
    p {  font-size: 1.2rem;}
    body {  margin: 0;}
    .topnav { overflow: hidden; background-color: #263c5e; color: white; font-size: 1.5rem; }
    .content { padding: 10px; }
    .card { background-color: white; padding: 5px; box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5); }
    .card-grid { max-width: 700px; margin: 0 auto; display: grid; grid-gap: 2rem; grid-template-columns: repeat(auto-fit, minmax(300px, 1fr)); }
    .reading { font-size: 2.0rem; }
    .headline { font-size: 2.0rem; font-weight: bold; }
    .card.info { color: #034078; }
    .card.info-dark { color: #151515; }
    .card-title { font-size: 1.2rem; font-weight: bold; color: #034078; }
    .card-title-dark { font-size: 1.2rem; font-weight: bold; color: #151515; }
    .btn:active { transform: scale(0.80); box-shadow: 3px 2px 22px 1px rgba(0, 0, 0, 0.24); }
    .btn {
            text-decoration: none;
            border: none;
            width: 90px;
            padding: 10px 10px;
            margin: 15px 5px 15px 5px;
            text-align: center;
            font-size: 16px;
            background-color:#034078;
            color: #fff;
            border-radius: 5px;
            box-shadow: 7px 6px 28px 1px rgba(0, 0, 0, 0.24);
            cursor: pointer;
            outline: none;
            transition: 0.2s all;
          }
    
    .flex-container {
      display: flex;
      flex-direction: row;
      justify-content: space-between;
      align-items: center;
      text-align: center;
    }

    input:invalid {
        background-color: rgb(255, 179, 190);
    }

    input[type=text], select {
        padding: 5px 5px;
        margin: 8px 0;
        display: inline-block;
        border: 1px solid #ccc;
        border-radius: 4px;
        box-sizing: border-box;
    }

    input[type=number], select {
        padding: 5px 5px;
        margin: 8px 0;
        display: inline-block;
        border: 1px solid #ccc;
        border-radius: 4px;
        box-sizing: border-box;
    }

    input[type=time], select {
        padding: 5px 5px;
        margin: 8px 0;
        display: inline-block;
        border: 1px solid #ccc;
        border-radius: 4px;
        box-sizing: border-box;
    }
    
  </style>

</head>
<body onload="uhrzeit()">

  <div class="topnav">
    <h3>SoyoSource-PowerController</h3>
  </div>

  <div class="content">
    <div class="card-grid">
      <div class="card">
          <p class="card-title-dark">ESP Infos</p>
          <table class="font1 center">
            <tbody>
              <tr>
                <td width="80">ClientID:</td>
                <td width="100" id="client_id">%CLIENTID%</td>
                <th></th>
                <td width="50">Time:</td>
                <td width="180"><span id="datum"></span> <span id="uhr"></span></td>
              </tr>
              <tr>
                <td width="80">Wifi RSSI:</td>
                <td><span><span id="wifi_rssi">%WIFIRSSI%</span> dB</span></td>
                <th></th>
                <td width="50">Uptime:</td>
                <td width="180" id="up_time">%UPTIME%</td>
              </tr>
            </tbody>
          </table>
      </div>
    </div>
  </div>

  <div class="content">
    <div class="card-grid">
      <div class="card">
      <p class="card-title">SoyoSource</p>
        <!--<p class="card-title"><span class="headline">AC Output: </span><span class="reading"><span id="static_power">%STATICPOWER%</span> W</span></p> -->
        <table class="font1">
            <tbody>
              <tr>
                <td width=40%%><span>AC Output:</span></td>
                <td><span class="reading"></span><span class="reading"><span id="static_power">%STATICPOWER%</span> W</span></td>
              </tr>
            </tbody>
          </table>
        <hr style="color: #fafafa;">        

        <table>
          <tbody>
            <tr>
              <td style="text-align: center;"><button type="button" onclick="set_power('/m1');" class="btn">- 1</button></td>
              <td style="text-align: center;" class="card-title">Set AC Output</td>
              <td style="text-align: center;"><button type="button" onclick="set_power('/p1');" class="btn">+ 1</button></td>
            </tr>
            <tr>
              <td style="text-align: center;"><button type="button" onclick="set_power('/m10');" class="btn">- 10</button></td>
              <td style="text-align: center;"><button type="button" onclick="set_power('/s0');;" class="btn">0</button></td>
              <td style="text-align: center;"><button type="button" onclick="set_power('/p10');" class="btn">+ 10</button></td>
            </tr>
          </tbody>
        </table>
      </div>
    </div>
  </div>

  <div class="content">
    <div class="card-grid">
      <div class="card">
      <p class="card-title">%METERTYP%</p>
        <hr style="color: #fafafa;"> 
          <table class="font1">
            <tbody>
              <tr>
                <td width=40%%><span>Leistung aktuell:</span></td>
                <td><span class="reading"><span id="meter_power">%METERPOWER%</span> W</span></td>
              </tr>
              <tr>
                <td><span>Shelly IP:</span></td>
                <td><span><input type='text' id='meteripaddr' placeholder='xxx.xxx.xxx.xxx' required pattern='^((\d|[1-9]\d|1\d\d|2[0-4]\d|25[0-5])\.){3}(\d|[1-9]\d|1\d\d|2[0-4]\d|25[0-5])$' %METERIPADDR%/></span></td>
              </tr>
            </tbody>
          </table>
      </div>
    </div>
  </div>

  <div class="content">
    <div class="card-grid">
      <div class="card">
        <p class="card-title">Nulleinspeisung</p>
        <hr style="color: #fafafa;"> 
          <table class="font1">
            <tbody>
                <tr>
                    <td width=40%%><span>Max AC Leistung [W]:</span></td>
                    <td><span><input type='number' min='0' max='2000' id='maxwatt' %MAXWATT%/></span></td>
                </tr>
                <tr>
                    <td><span>Aktiv: <input type='checkbox' onchange='toggleCheckbox(this)' id='cb4' %CBSTATE4%/></span></td>
                    <td></td>
                </tr>
            </tbody>
          </table>
      </div>
    </div>
  </div>

  <div class="content">
    <div class="card-grid">
      <div class="card">
          <p class="card-title">MQTT</p>
          <table class="font1 center">
            <tbody>
              <tr>
                <th style="text-align: left">Publish Topics</th>
                <th style="text-align: left">Subscribe Topics </th>
              </tr>
              <tr>
                <td style="text-align: left"><span id="mqtt_root">%MQTTROOT%</span>/power</td>
                <td style="text-align: left"><span id="mqtt_root">%MQTTROOT%</span>/power</td>
              </tr>
              <tr>
                <td style="text-align: left"><span id="mqtt_root">%MQTTROOT%</span>/alive</td>
                <td style="text-align: left"></td>
              </tr>
              <tr>
                <td><label>Aktiv: <input type='checkbox' onchange='toggleCheckbox(this)' id='cb3' %CBSTATE3%/></label></td>
                <td><label>Status: </label><span id="mqtt_state">%MQTTSTATE%</span></td>
              </tr>
            </tbody>
          </table>
      </div>
    </div>
  </div>


<div class="content">
    <div class="card-grid">
      <div class="card">
          <p class="card-title">Timer</p>
          <table class="font2 left">
            <tbody>
              <tr>
                <td><label>Aktiv: <input type='checkbox' onchange='toggleCheckbox(this)' id='cb1' %CBSTATE1%/></label></td>
                <td><label>Time 1: <input type='time' id='t1' %TIME1% /></label></td>
                <td><label>Leistung [W] <input type='number' min='0' max='2000' id='w1' %WATT1%/></label></td>
              </tr>
              <tr>
                <td><label>Aktiv: <input type='checkbox' onchange='toggleCheckbox(this)' id='cb2' %CBSTATE2%/></label></td>
                <td><label>Time 2: <input type='time' id='t2' %TIME2% /></label></td>
                <td><label>Leistung [W] <input type='number' min='0' max='2000' id='w2' %WATT2%/></label></td>
              </tr>
            </tbody>
          </table>
      </div>
    </div>
  </div>

      
  <div class="content">
    <div class="card-grid">
      <div class="card">
        <div class="flex-container">
            <button class="btn" type="button" onclick="apmode();" style=" font-size: 10px; width: auto; margin: 5px 5px 5px 5px; background-color:#767676;">Delete WiFi Settings</button>
            <button class="btn" type="button" onclick="restart();" style=" font-size: 10px; width: auto; margin: 5px 5px 5px 5px; background-color:#767676;">Restart</button>
            <a href="update"><button class="btn" type="button" style="font-size: 10px; width: auto; margin: 5px 5px 5px 5px; background-color:#767676;"> FW Update</button></a>
            <button class="btn" type="button" onclick="savesettings()";" style=" font-size: 10px; width: auto; margin: 5px 5px 5px 5px; background-color:#767676;">Save Settings</button>
        </div>
      </div>
    </div>
  </div>

 

<script>
  if (!!window.EventSource) {
    var source = new EventSource('/events');

    source.addEventListener('open', function(e) {
      console.log("Events Connected");
    }, false);

    source.addEventListener('error', function(e) {
      if (e.target.readyState != EventSource.OPEN) {
        console.log("Events Disconnected");
      }
    }, false);
    
    source.addEventListener('message', function(e) {
      console.log("message:", e.data);
    }, false);
    
    source.addEventListener('staticpower', function(e) {
      console.log("staticpower:", e.data);
      document.getElementById('static_power').innerHTML = e.data;
    }, false);

    source.addEventListener('meterpower', function(e) {
      console.log("meterpower:", e.data);
      document.getElementById('meter_power').innerHTML = e.data;
    }, false);

    source.addEventListener('mqttroot', function(e) {
      console.log("mqttroot:", e.data);
      document.getElementById('mqtt_root').innerHTML = e.data;
    }, false);

    source.addEventListener('mqttstate', function(e) {
      console.log("mqttstate:", e.data);
      document.getElementById('mqtt_state').innerHTML = e.data;
    }, false);

    source.addEventListener('wifirssi', function(e) {
      console.log("wifirssi:", e.data);
      document.getElementById('wifi_rssi').innerHTML = e.data;
    }, false);

    source.addEventListener('clientid', function(e) {
      console.log("clientid:", e.data);
      document.getElementById('client_id').innerHTML = e.data;
    }, false);

    source.addEventListener('uptime', function(e) {
      console.log("uptime:", e.data);
      document.getElementById('up_time').innerHTML = e.data;
    }, false);

    source.addEventListener('currenttime', function(e) {
      console.log("currenttime:", e.data);
      document.getElementById('current_time').innerHTML = e.data;
    }, false);

    source.addEventListener('soyotext', function(e) {
      console.log("soyotext:", e.data);
      document.getElementById('soyo_text').innerHTML = e.data;
    }, false);

    source.addEventListener('CBSTATE1', function(e) {
      console.log("checkbox1:", e.data);
      document.getElementById('cb1').innerHTML = e.data;
    }, false);

    source.addEventListener('CBSTATE2', function(e) {
      console.log("checkbox2:", e.data);
      document.getElementById('cb2').innerHTML = e.data;
    }, false);

    source.addEventListener('CBSTATE3', function(e) {
      console.log("checkbox3:", e.data);
      document.getElementById('cb3').innerHTML = e.data;
    }, false);

    source.addEventListener('CBSTATE4', function(e) {
      console.log("checkbox4:", e.data);
      document.getElementById('cb4').innerHTML = e.data;
    }, false);
      
    source.addEventListener('time1', function(e) {
      console.log("time1:", e.data);
      document.getElementById('t1').innerHTML = e.data;
    }, false);

    source.addEventListener('time2', function(e) {
      console.log("time2:", e.data);
      document.getElementById('t2').innerHTML = e.data;
    }, false);

    source.addEventListener('watt1', function(e) {
      console.log("watt1:", e.data);
      document.getElementById('w1').value = parseInt(e.data);
    }, false);

    source.addEventListener('watt2', function(e) {
      console.log("watt2:", e.data);
      document.getElementById('w2').value = parseInt(e.data);
    }, false);

    source.addEventListener('metertyp', function(e) {
      console.log("metertyp:", e.data);
      document.getElementById('meter_typ').value = parseInt(e.data);
    }, false);

    source.addEventListener('meteripaddr', function(e) {
      console.log("meteripaddr:", e.data);
      document.getElementById('meter_ipaddr').value = parseInt(e.data);
    }, false);

     source.addEventListener('maxwatt', function(e) {
      console.log("maxwatt:", e.data);
      document.getElementById('max_watt').value = parseInt(e.data);
    }, false);

  }

  function set_power(value) {
    var xhr = new XMLHttpRequest();
    xhr.open("GET", '/acoutput?value=' + value, true);
    xhr.send();
  };
  
  function apmode() {
    let text = "Reset WiFi credentials and restart!\nPress OK or Cancel.";
    if (confirm(text) == true) {
      var xhr = new XMLHttpRequest();
      xhr.open("GET", "/apmode", true);
      xhr.send();
    } else {
      text = " canceled!";
    }
  };

  function restart() {
    let text = "Restart System!\nPress OK or Cancel.";
    if (confirm(text) == true) {
      var xhr = new XMLHttpRequest();
      xhr.open("GET", "/restart", true);
      xhr.send();
    } else {
      text = " canceled!";
    } 
  };

  
  function savesettings() {
    var time1 = document.getElementById("t1").value;
    var watt1 = document.getElementById("w1").value;

    var time2 = document.getElementById("t2").value;
    var watt2 = document.getElementById("w2").value;

    var meteripaddr = document.getElementById("meteripaddr").value;
    var maxwatt = document.getElementById("maxwatt").value;
    
    let text = "Save Settings!\nPress OK or Cancel.";
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/savesettings?t1=" + time1 +"&w1=" + watt1, true);
    xhr.send();

    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/savesettings?t2=" + time2 + "&w2=" + watt2, true);
    xhr.send();

    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/savesettings?meteripaddr=" + meteripaddr + "&maxwatt=" + maxwatt, true);
    xhr.send();
    
  };


 function toggleCheckbox(element) {
  var xhr = new XMLHttpRequest();
  if(element.checked){ xhr.open("GET", "/checkbox?cbid="+element.id+"&state=1", true); }
  else { xhr.open("GET", "/checkbox?cbid="+element.id+"&state=0", true); }
  xhr.send();
 }


  function uhrzeit() {
    var jetzt = new Date();
    h = jetzt.getHours();
    m = jetzt.getMinutes();
    s = jetzt.getSeconds();
    h = fuehrendeNull(h);
    m = fuehrendeNull(m);
    s = fuehrendeNull(s);
    document.getElementById('uhr').innerHTML = h + ':' + m + ':' + s;
    setTimeout(uhrzeit, 1000);
  };
  
  function fuehrendeNull(zahl) {
    zahl = (zahl < 10 ? '0' : '' )+ zahl;  
    return zahl;
  };

  var mydate = new Date();
  var options = { year: 'numeric', month: '2-digit', day: '2-digit' };
  document.getElementById("datum").textContent = mydate.toLocaleString('de-DE', options);

  
</script>

</body>
</html>
)rawliteral";