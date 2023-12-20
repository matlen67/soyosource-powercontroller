const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <title>SoyoSource-PowerController</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    table { width: 90%; table-layout: fixed; }
    table.font1{ font-size: clamp(.9rem, 0.9vw, 1.2rem); }
    table.style2 { width: 100%; table-layout: fixed; }
    td { text-align: left; }
    th { text-align: center; }
    p {  font-size: 1.2rem;}
    body {  margin: 0;}
    .title1 { font-size: 1.2rem; font-weight: bold; color: #034078; }
    .alnright { text-align: right; }
    .topnav { overflow: hidden; background-color: #263c5e; color: white; font-size: 1.5rem; }
    .content { padding: 10px; }
    .card { background-color: white; padding: 5px; box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5); }
    .card-grid { max-width: 700px; margin: 0 auto; display: grid; grid-gap: 2rem; grid-template-columns: repeat(auto-fit, minmax(20em, 1fr)); }
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
     
    hr {
        border: 0;
        height: 1px;
        background-image: linear-gradient(to right, rgba(0, 0, 0, 0), rgba(0, 0, 0, 0.75), rgba(0, 0, 0, 0));
    }
   
    summary {
        position: relative;
        padding: 12px;
        display: flex;
        color: #102752;
        transform: scaleX(-1);
    }

    summary b {
        display: inline-block;
        position: absolute;
        right: 6px;
        transform: scaleX(-1);
    }

    details>summary {
        list-style: none;
    }

    summary::-webkit-details-marker {
        display: none;
    }

    summary::after {
        content: '+';
        color: #999999;    
    }
    
    details[open] summary:after {
		  content: "-";
      color: #999999;  
    }

    .contentFlex{
      display: flex;
      flex-direction: row;
      justify-content:flex-start;
      align-items: center;
      gap: 10px;
      margin-left: 10px;
      margin-top: 5px;
      margin-bottom: 5px;
    }

    .fontBold{
      font-weight: bold;
      text-align: left;
      margin-left: 10px;
    }

    .fontNormal{
      font-weight: normal;
      text-align: left;
      margin-left: 10px;
    }

    .cellStyle1{
      width: 120px;
      text-align: left;
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
        <details>
            <summary>
              <b>
                <table>
                  <tbody>
                    <tr>
                      <td width="150px">ESP Infos</td>
                      <td class='alnright'><span id="UPTIME"></span></td>
                    </tr>
                  </tbody>  
                </table>
              </b>
            </summary>
            <hr>
            <div class="contentFlex">
              <div class="cellStyle1">Client ID:</div>
              <div id="CLIENTID"></div>
            </div>
            <div class="contentFlex">
              <div class="cellStyle1">Wifi RSSI:</div>
              <div> <span><span id="WIFIRSSI"></span> dB</span></div>
            </div>
        </details>
      </div>
    </div>
  </div>

  <div class="content">
    <div class="card-grid">
      <div class="card">
        <details>
          <summary>
            <b>
              <table>
                <tbody>
                  <tr>
                    <td width="180px">SoyoSource</td>
                    <td class='alnright'><span><span id="SOYOPOWER"></span> W</span></td>
                  </tr>
                </tbody>
              </table>
            </b>
          </summary>
          <hr>
          <span class="title1">Manuelle Vorgabe</span>
          <table class="style2">
            <tbody>
              <tr>
                <td style="text-align: center;"><button type="button" onclick="set_power('/m1');" class="btn">- 1</button></td>
                <td style="text-align: center;">Set AC Output</td>
                <td style="text-align: center;"><button type="button" onclick="set_power('/p1');" class="btn">+ 1</button></td>
              </tr>
              <tr>
                <td style="text-align: center;"><button type="button" onclick="set_power('/m10');" class="btn">- 10</button></td>
                <td style="text-align: center;"><button type="button" onclick="set_power('/s0');;" class="btn">0</button></td>
                <td style="text-align: center;"><button type="button" onclick="set_power('/p10');" class="btn">+ 10</button></td>
              </tr>
            </tbody>
          </table>
        </details>
      </div>
    </div>
  </div>

  <div class="content">
    <div class="card-grid">
      <div class="card">
        <details>
          <summary>
            <b>
              <table>
                <tr>
                  <td width="180px"><span id="METERNAME">no device</span></td>
                  <td class='alnright'><span id="METERPOWER"></span> W</span></td>
                </tr>
              </table>
            </b>
          </summary>
          <hr>
          <div class="contentFlex">
            <div class="cellStyle1">L1 [W]:</div>
            <div id="METERL1"></div>
          </div>
          <div class="contentFlex">
            <div class="cellStyle1">L2 [W]:</div>
            <div id="METERL2"></div>
          </div>
          <div class="contentFlex">
            <div class="cellStyle1">L3 [W]:</div>
            <div id="METERL3"></div>
          </div>
          <hr>
          <div class="contentFlex">
            <div class="cellStyle1">Interval [ms]:</div>
            <div><input type="number" min="500" max="5000" id="METERINTERVAL" /></div>
          </div>
          <hr>
          <div class="contentFlex">
            <div class="cellStyle1">Meter Ip:</div>
            <div><input type='text' id='METERIP' placeholder='xxx.xxx.xxx.xxx' required pattern='^((\d|[1-9]\d|1\d\d|2[0-4]\d|25[0-5])\.){3}(\d|[1-9]\d|1\d\d|2[0-4]\d|25[0-5])$'/></div>
          </div>
        </details>
      </div>
    </div>
  </div>

  <div class="content">
    <div class="card-grid">
      <div class="card">
        <details>
          <summary>
            <b>
              <table>
                <tr>
                  <td width="180px">Nulleinspeisung</td>
                  <td class='alnright'><span id="MAXWATT"></span> W</span></td>
                </tr>
              </table>
            </b>
          </summary>
          <hr>
          <div class="contentFlex">
            <div class="cellStyle1">Max Power [W]:</div>
            <div><input type="number" min="0" max="2000" id="MAXWATTINPUT"/></div>
          </div>
          <div class="contentFlex">
            <div>Aktiv:</div>
            <div><input type="checkbox" onchange="toggleCheckbox(this)" id="CBNULL"/></div>
          </div>
        </details>
      </div>
    </div>
  </div>

   <div class="content">
    <div class="card-grid">
      <div class="card">
        <details>
          <summary><b>MQTT</b></summary>
          <hr>
          <div class="fontBold">Publish Topics</div>
          <div class="fontNormal"><span><span id="MQTTROOT1"></span>/power</span></div>
          <br>
          <div class="fontBold">Subscribe Topics</div>
          <div class="fontNormal"><span><span id="MQTTROOT2"></span>/power</span></div>
          <br>
          <div class="contentFlex">
            <div style="padding-right: 50px;">Aktiv: <input type="checkbox" onchange="toggleCheckbox(this)" id="CBMQTTSTATE" /></div>
            <div>Status: </label><span id="MQTTSTATE"></span></div>
          </div>
        </details>
      </div>
    </div>
  </div>
  
  <div class="content">
    <div class="card-grid">
      <div class="card">
        <details>
          <summary><b>Timer</b></summary>
          <hr>
          <div class="contentFlex">
            <div>Aktiv: <input type="checkbox" onchange="toggleCheckbox(this)" id="CBTIMER1" /></div>
            <div>Timer 1: <input type="time" id="TIMER1TIME" /></div>
            <div>Watt: <input type="number" min="0" max="2000" id="TIMER1WATT"/></div>
          </div>
          <div class="contentFlex">
            <div>Aktiv: <input type="checkbox" onchange="toggleCheckbox(this)" id="CBTIMER2" /></div>
            <div>Timer 2: <input type="time" id="TIMER2TIME" /></div>
            <div>Watt: <input type="number" min="0" max="2000" id="TIMER2WATT"/></div>
          </div>       
        </details>
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
            <button class="btn" type="button" onclick="savesettings();" style=" font-size: 10px; width: auto; margin: 5px 5px 5px 5px; background-color:#767676;">Save Settings</button>
        </div>
      </div>
    </div>
  </div>

  <div>
    <span><span id="datum"></span> - <span id="uhrzeit"></span></span>
  </div>


<script>
  fetch('/json').then(function(response) {
      return response.json();
    }).then(function(data_start) {
      console.log(data_start);
      document.getElementById("CLIENTID").innerHTML       = data_start.CLIENTID
      document.getElementById("WIFIRSSI").innerHTML       = data_start.WIFIRSSI
      document.getElementById("METERNAME").innerHTML      = data_start.METERNAME
      document.getElementById("MQTTROOT1").innerHTML      = data_start.MQTTROOT
      document.getElementById("MQTTROOT2").innerHTML      = data_start.MQTTROOT
      document.getElementById("MQTTSTATE").innerHTML      = data_start.MQTTSTATE
      document.getElementById("METERIP").value            = data_start.METERIP
      document.getElementById("METERINTERVAL").value      = data_start.METERINTERVAL
      document.getElementById("MAXWATTINPUT").value       = data_start.MAXWATTINPUT
      document.getElementById("TIMER1TIME").value         = data_start.TIMER1TIME
      document.getElementById("TIMER2TIME").value         = data_start.TIMER2TIME
      document.getElementById("TIMER1WATT").value         = data_start.TIMER1WATT
      document.getElementById("TIMER2WATT").value         = data_start.TIMER2WATT
      document.getElementById("CBNULL").checked           = data_start.CBNULL
      document.getElementById("CBMQTTSTATE").checked      = data_start.CBMQTTSTATE
      document.getElementById("CBTIMER1").checked         = data_start.CBTIMER1
      document.getElementById("CBTIMER2").checked         = data_start.CBTIMER2
    });
</script>


<script>
  setInterval(getDataInterval, 1000);
  getDataInterval();

  function getDataInterval() {
    fetch('/json').then(function(response) {
        return response.json();
      }).then(function(data) {
      console.log(data);
      document.getElementById("UPTIME").innerHTML         = data.UPTIME
      document.getElementById("SOYOPOWER").innerHTML      = data.SOYOPOWER
      document.getElementById("METERPOWER").innerHTML     = data.METERPOWER
      document.getElementById("METERL1").innerHTML        = data.METERL1
      document.getElementById("METERL2").innerHTML        = data.METERL2
      document.getElementById("METERL3").innerHTML        = data.METERL3
      document.getElementById("MAXWATT").innerHTML        = data.MAXWATT
    });
  } 
</script>


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
    var timer1_time = document.getElementById("TIMER1TIME").value;
    var timer1_watt = document.getElementById("TIMER1WATT").value;

    var timer2_time = document.getElementById("TIMER2TIME").value;
    var timer2_watt = document.getElementById("TIMER2WATT").value;

    var meteripaddr = document.getElementById("METERIP").value;
    var meterinterval = document.getElementById("METERINTERVAL").value;
    var maxwatt = document.getElementById("MAXWATTINPUT").value;
    
    let text = "Save Settings!\nPress OK or Cancel.";
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/savesettings?t1=" + timer1_time + "&w1=" + timer1_watt + "&t2=" + timer2_time + "&w2=" + timer2_watt + "&meteripaddr=" + meteripaddr +  "&meterinterval=" + meterinterval +"&maxwatt=" + maxwatt , true);
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
    document.getElementById('uhrzeit').innerHTML = h + ':' + m + ':' + s;
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