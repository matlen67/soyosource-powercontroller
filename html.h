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
    .topnav { overflow: hidden; background-color: #263c5e; color: white; font-size:1.5rem; margin:0px; padding:0px }
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
    
    .flexBox1 {
      display: flex;
      flex-direction: row;
      justify-content: space-between;
      align-items: center;
      text-align: center;
    }

    .flexBox2 {
      display: flex;
      flex-direction: row;
      justify-content: space-around;
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

    .detailsFlexBox{
      display: flex;
      flex-direction: row;
      justify-content:flex-start;
      align-items: center;
      gap: 10px;
      margin-left: 10px;
      margin-top: 5px;
      margin-bottom: 5px;
    }

    .flexContainer{
      display: flex;
      flex-direction: row;
      justify-content:flex-start;
      align-items: center;
      margin-left: 10px;
      margin-top: 5px;
    }

    .flexColLeft{
      flex-basis: 65%;
      text-align: start;
      padding-left: 5px;
    }
  
    .flexColRight{
      flex-basis: 35%;
      text-align: end;
      padding-right: 25px;
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
      width: 140px;
      text-align: left;
    }

    .cellStyle2{
      width: 80px;
      text-align: left;
    }

   
    .version {
      top: -40%;
      right: -165px;
      font-size: 40%;
      margin-top: -25px;
      margin-bottom: 10px;
      position: relative;
    }

  </style>

</head>
<body onload="uhrzeit()">

  <div class="topnav">
    <h3>SoyoSource-PowerController</h3>
    <div class="version">v 1.240428.1</div>
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
                      <td width="150px">ESP</td>
                      <td class='alnright'><span> WiFi-Qualit&aumlt: <span id="WIFIQUALITI"></span> %</span></td>
                    </tr>
                  </tbody>
                </table>
              </b>
            </summary>
            <hr>
            <div class="detailsFlexBox">
              <div class="cellStyle1">Client ID:</div>
              <div id="CLIENTID"></div>
            </div>
            <div class="detailsFlexBox">
              <div class="cellStyle1">Wifi RSSI:</div>
              <div> <span><span id="WIFIRSSI"></span> dBm</span></div>
            </div>
            <div class="detailsFlexBox">
              <div class="cellStyle1">Uptime:</div>
              <div id="UPTIME"></div>
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
                    <td width="180px">SoyoSource Output</td>
                    <td class='alnright'><span id="SOYOPOWER"></span> W</td>
                  </tr>
                </tbody>
              </table>
            </b>
          </summary>
          <hr>
          <div class="flexContainer">
            <div class="flexColLeft">Teiler Output:</div>
            <div class="flexColRight"><input type="number" min="1" max="3" id="TOUT" /></div>
          </div>
          <hr>
          <span class="title1">Manuelle Steuerung</span>
          <div class="flexBox2">
            <div><button type="button" onclick="set_power('/m1');" class="btn">- 1</button></div>
            <div>Set Output [W]</div>
            <div><button type="button" onclick="set_power('/p1');" class="btn">+ 1</button></div>
          </div>
          <div class="flexBox2">
            <div><button type="button" onclick="set_power('/m10');" class="btn">- 10</button></div>
            <div><button type="button" onclick="set_power('/s0');;" class="btn">0</button></div>
            <div><button type="button" onclick="set_power('/p10');" class="btn">+ 10</button></div>
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
                    <td width="180px"><span id="METERNAME">no device</span></td>
                    <td class="alnright"><span id="METERPOWER"></span> W</td>
                  </tr>
                </tbody>
              </table>
            </b>
          </summary>
          <hr>
          <div class="flexContainer">
            <div class="flexColLeft"><input type="checkbox" onchange="toggleCheckbox(this)" id="CBMETERL1"/> L1 [W]:</div>
            <div class="flexColRight" id="METERL1">999</div>
          </div>
          <div class="flexContainer">
            <div class="flexColLeft"><input type="checkbox" onchange="toggleCheckbox(this)" id="CBMETERL2"/> L2 [W]:</div>
            <div class="flexColRight" id="METERL2">999</div>
          </div>
          <div class="flexContainer">
            <div class="flexColLeft"><input type="checkbox" onchange="toggleCheckbox(this)" id="CBMETERL3"/> L3 [W]:</div>
            <div class="flexColRight" id="METERL3">999</div>
          </div>
          <hr>
          <div class="flexContainer">
            <div class="flexColLeft">Interval [ms]:</div>
            <div class="flexColRight"><input type="number" min="500" max="5000" id="METERINTERVAL" /></div>
          </div>
          <div class="flexContainer">
            <div class="flexColLeft">Meter Ip:</div>
            <div class="flexColRight"><input type='text' id='METERIP' placeholder='xxx.xxx.xxx.xxx' required pattern='^((\d|[1-9]\d|1\d\d|2[0-4]\d|25[0-5])\.){3}(\d|[1-9]\d|1\d\d|2[0-4]\d|25[0-5])$'/></div>
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
                    <td width="180px"><span>Nulleinspeisung</span></td>
                    <td class="alnright"><span id="NULLSTATE"></span></td>
                  </tr>
                </tbody>
              </table>
            </b>
          </summary>
          <hr>
          <div class="flexContainer">
            <div class="flexColLeft">Max Output [W]:</div>
            <div class="flexColRight"><input type="number" min="0" max="2000" id="MAXWATTINPUT"/></div>
          </div>
          <div class="flexContainer">
            <div class="flexColLeft">Nullpunkt-Offset [W]:</div>
            <div class="flexColRight"><input type="number" min="0" max="200" id="NULLOFFSET" /></div>
          </div>
          <div class="flexContainer">
            <div class="flexColLeft">Interval [ms]:</div>
            <div class="flexColRight"><input type="number" min="500" max="30000" id="NULLINTERVAL" /></div>
          </div>
          <div class="detailsFlexBox">
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
          <summary>
            <b>
              <table>
                <tbody>
                  <tr>
                    <td width="180px"><span>MQTT</span></td>
                    <td class="alnright"><span id="MQTTSTATE"></span></td>
                  </tr>
                </tbody>
              </table>
            </b>
          </summary>
          <hr>
          <div class="flexContainer">
            <div class="flexColLeft">MQTT Server:</div>
            <div class="flexColRight"><input type='text' id='MQTTSERVER' placeholder='xxx.xxx.xxx.xxx' required pattern='^((\d|[1-9]\d|1\d\d|2[0-4]\d|25[0-5])\.){3}(\d|[1-9]\d|1\d\d|2[0-4]\d|25[0-5])$'/></div>
          </div>
          <div class="flexContainer">
            <div class="flexColLeft">MQTT Port:</div>
            <div class="flexColRight"><input type="number" id="MQTTPORT" min="0" max="65535" /></div>
          </div>
          <div class="flexContainer">
            <div style="padding-right: 50px;">Aktiv: <input type="checkbox" onchange="toggleCheckbox(this)" id="CBMQTTSTATE" /></div>
            <div>Status: </label><span id="MQTTSTATECL"></span></div>
          </div>
          <hr>
          <div class="fontBold">Publish Topics</div>
          <div class="flexContainer">
            <div class="flexColLeft"><span><span id="MQTTROOT1"></span>/power</span></div>
          </div>
          <br>
          <div class="fontBold">Subscribe Topics</div>
          <div class="flexContainer">
            <div class="flexColLeft"><span><span id="MQTTROOT2"></span>/power</span></div>
            <div class="flexColRight" id="MQTT_SUB_1">999</div>
          </div>
          <br>
          <div class="flexContainer">
            <div class="flexColLeft"><span>VenusOS/SmartShunt/voltage</span></div>
            <div class="flexColRight" id="MQTT_BAT_V">999</div>
          </div>
          <div class="flexContainer">
            <div class="flexColLeft"><span>VenusOS/SmartShunt/soc</span></div>
            <div class="flexColRight" id="MQTT_BAT_SOC">999</div>
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
                    <td width="180px"><span>Batterieschutz</span></td>
                    <td class="alnright"><span id="BATTSTATE"></span></td>
                  </tr>
                </tbody>
              </table>
            </b>
          </summary>
          <hr>
          <div class="flexContainer">
            <div class="flexColLeft">Stop Output bei SOC kleiner</div>
            <div class="flexColRight"><input type="number" id="BATSOCSTOP" min="10" max="100" placeholder="20" /> %</div>
          </div>
          <div class="flexContainer">
            <div class="flexColLeft">Restart Output bei SOC gr&ouml&szliger</div>
            <div class="flexColRight"><input type="number" id="BATSOCSTART" min="15" max="100" placeholder="80" /> %</div>
          </div>
          <div class="detailsFlexBox">
            <div>Aktiv:</div>
            <div><input type="checkbox" onchange="toggleCheckbox(this)" id="CBBATSCHUTZ"/></div>
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
                    <td width="180px"><span>Timer</span></td>
                    <td class="alnright"><span id="TIMERSTATE"></span></td>
                  </tr>
                </tbody>
              </table>
            </b>
          </summary>
          <hr>
          <div class="detailsFlexBox">
            <div>Aktiv: <input type="checkbox" onchange="toggleCheckbox(this)" id="CBTIMER1" /></div>
            <div>Timer 1: <input type="time" id="TIMER1TIME" /></div>
            <div>Watt: <input type="number" min="0" max="2000" id="TIMER1WATT"/></div>
          </div>
          <div class="detailsFlexBox">
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
        <div class="flexBox1">
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
      document.getElementById("MQTTSTATECL").innerHTML    = data_start.MQTTSTATECL
      document.getElementById("MQTTSTATE").innerHTML      = data_start.MQTTSTATE
      document.getElementById("NULLSTATE").innerHTML      = data_start.NULLSTATE
      document.getElementById("BATTSTATE").innerHTML      = data_start.BATTSTATE
      document.getElementById("TIMERSTATE").innerHTML     = data_start.TIMERSTATE
      document.getElementById("WIFIQUALITI").innerHTML    = data_start.WIFIQUALITI
      document.getElementById("TOUT").value               = data_start.TOUT
      document.getElementById("METERIP").value            = data_start.METERIP
      document.getElementById("METERINTERVAL").value      = data_start.METERINTERVAL
      document.getElementById("MAXWATTINPUT").value       = data_start.MAXWATTINPUT
      document.getElementById("NULLINTERVAL").value       = data_start.NULLINTERVAL
      document.getElementById("NULLOFFSET").value         = data_start.NULLOFFSET
      document.getElementById("TIMER1TIME").value         = data_start.TIMER1TIME
      document.getElementById("TIMER2TIME").value         = data_start.TIMER2TIME
      document.getElementById("TIMER1WATT").value         = data_start.TIMER1WATT
      document.getElementById("TIMER2WATT").value         = data_start.TIMER2WATT
      document.getElementById("CBNULL").checked           = data_start.CBNULL
      document.getElementById("CBMQTTSTATE").checked      = data_start.CBMQTTSTATE
      document.getElementById("CBTIMER1").checked         = data_start.CBTIMER1
      document.getElementById("CBTIMER2").checked         = data_start.CBTIMER2
      document.getElementById("MQTTSERVER").value         = data_start.MQTTSERVER
      document.getElementById("MQTTPORT").value           = data_start.MQTTPORT
      document.getElementById("BATSOCSTOP").value         = data_start.BATSOCSTOP
      document.getElementById("BATSOCSTART").value        = data_start.BATSOCSTART
      document.getElementById("CBBATSCHUTZ").checked      = data_start.CBBATSCHUTZ
      document.getElementById("CBMETERL1").checked        = data_start.CBMETERL1
      document.getElementById("CBMETERL2").checked        = data_start.CBMETERL2
      document.getElementById("CBMETERL3").checked        = data_start.CBMETERL3
      


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
      document.getElementById("METERNAME").innerHTML      = data.METERNAME
      document.getElementById("METERPOWER").innerHTML     = data.METERPOWER
      document.getElementById("METERL1").innerHTML        = data.METERL1
      document.getElementById("METERL2").innerHTML        = data.METERL2
      document.getElementById("METERL3").innerHTML        = data.METERL3
      document.getElementById("MQTT_SUB_1").innerHTML     = data.MQTT_SUB_1
      document.getElementById("MQTT_BAT_SOC").innerHTML   = data.MQTT_BAT_SOC
      document.getElementById("MQTT_BAT_V").innerHTML     = data.MQTT_BAT_V
      
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
    var nullinterval = document.getElementById("NULLINTERVAL").value;
    var nulloffset = document.getElementById("NULLOFFSET").value;

    var mqttserver = document.getElementById("MQTTSERVER").value;
    var mqttport = document.getElementById("MQTTPORT").value;

    var batsocstop = document.getElementById("BATSOCSTOP").value;
    var batsocstart = document.getElementById("BATSOCSTART").value;

    var tout = document.getElementById("TOUT").value;

    let text = "Save Settings!\nPress OK or Cancel.";
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/savesettings?t1=" + timer1_time + "&w1=" + timer1_watt + "&t2=" + timer2_time + "&w2=" + timer2_watt +
     "&meteripaddr=" + meteripaddr + "&meterinterval=" + meterinterval + "&maxwatt=" + maxwatt + "&nullinterval=" + nullinterval +
     "&nulloffset=" + nulloffset + "&mqttserver=" + mqttserver + "&mqttport=" + mqttport + "&batsocstop=" + batsocstop +
     "&batsocstart=" + batsocstart + "&tout=" + tout, true);
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
    zahl = (zahl < 10 ? '0' : '' ) + zahl;  
    return zahl;
  };

  var mydate = new Date();
  var options = { year: 'numeric', month: '2-digit', day: '2-digit' };
  document.getElementById("datum").textContent = mydate.toLocaleString('de-DE', options);

  
</script>

</body>
</html>
)rawliteral";