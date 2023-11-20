/***************************************************************************
  soyosource_power_controller @matlen67
  Version: 1.0 /  14.11.2023

  # new in this Version
  

  Wiring
  NodeMCU D1 - RS485 RO
  NodeMCU D3 - RS485 DE/RE
  NodeMCU D4 - RS485 DI

****************************************************************************/
#include <FS.h>  
#include "Arduino.h"
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsync_WiFiManager.h>
#include <ESPAsyncWebServer.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <AsyncElegantOTA.h>
#include <Uptime.h>
#include <time.h>
#include "html.h"
#include <TelnetSpy.h>


#define DEBUG false

TelnetSpy SerialAndTelnet;
#define DEBUG_SERIAL  SerialAndTelnet


//#define DEBUG_SERIAL \
//  if (DEBUG) Serial


#define RXPin        D1  // Serial Receive pin (D1)
#define TXPin        D4  // Serial Transmit pin (D4)
 
//RS485 control
#define SERIAL_COMMUNICATION_CONTROL_PIN D3 // Transmission set pin (D3)
#define RS485_TX_PIN_VALUE HIGH
#define RS485_RX_PIN_VALUE LOW

// time server
#define MY_NTP_SERVER "de.pool.ntp.org"           
#define MY_TZ "CET-1CEST,M3.5.0/02,M10.5.0/03"   

 
SoftwareSerial RS485Serial(RXPin, TXPin); // RX, TX

WiFiClient espClient;
PubSubClient client(espClient);


// Uptime Global Variables
Uptime uptime;
uint8_t Uptime_Years = 0U, Uptime_Months = 0U, Uptime_Days = 0U, Uptime_Hours = 0U, Uptime_Minutes = 0U, Uptime_Seconds = 0U;
uint16_t Uptime_TotalDays = 0U; // Total Uptime Days
char Uptime_Str[37];  

//Timer
unsigned long lastTimeSoyo = 0;  
unsigned long timerDelaySoyo = 800;  // send readings timer

unsigned long lastTimeUptime = 0;  
unsigned long timerDelayUptime = 60000;  // send readings timer

unsigned long lastTimeShelly = 0;  
unsigned long timerDelayShelly = 1500;  // read shelly timer

unsigned long lastTimeNES = 0;  
unsigned long timerDelayNES = 10000;  // read shelly timer

String msg = "";
char msgData[64];

//mqtt
char mqtt_server[16] = "192.168.178.10";
char mqtt_port[5] = "1889";

//default custom static IP
char static_ip[16] = "192.168.178.250";
char static_gw[16] = "192.168.178.1";
char static_sn[16] = "255.255.255.0";

String dataReceived;
int data;
bool isDataReceived = false;
uint8_t byte0, byte1, byte2, byte3, byte4, byte5, byte6, byte7; 
int byteSend;
int data_array[8];
int soyo_hello_data[8] = {0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // bit7 org 0x00, CRC 0xFF
int soyo_power_data[8] = {0x24, 0x56, 0x00, 0x21, 0x00, 0x00, 0x80, 0x08}; // 0 Watt
int soyo_text_data[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

char buffer[8];
int old_value_power = 0;
int value_power = 0;

unsigned char mac[6];

char mqtt_root[32] = "SoyoSource/";
char clientId[12];

char topic_alive[40];
char topic_power[40];
char soyo_text[40];

long rssi;

time_t now;                       
tm tm;

const char checkbox_state_true[] = "checked";
const char checkbox_state_false[] = "";
const char value[] = "value='%S'";
const char shelly3empro[] = "Shelly 3EM Pro";
const char shelly3em[] = "Shelly 3EM";

const int shelly_3em_pro = 1;
const int shelly_3em = 2;
const int shelly_em = 3;
const int shelly_1pm = 4;

char currentTime[20];
char time_1[6] = "06:00";
char time_2[6] = "20:00";
char watt_1[6] = "0";
char watt_2[6] = "0";

char meteripaddr[16] = "";
char maxwatt[6] = "0";

//state checkboxes
bool checkboxT1 = false;
bool checkboxT2 = false;
bool mqttenabled = false;
bool nulleinspeisung = false;


char html_checkbox1[16];
char html_checkbox2[16];
char html_checkbox3[16];
char html_checkbox4[16];

char html_time1[16];
char html_time2[16];

char html_watt1[16];
char html_watt2[16];

char html_metertyp[24] = "Meter";
char html_meteripaddr[24];
char html_maxwatt[16];

char html_ShellyTyp[16];

char mqtt_state[20] = "disabled";

// variablen Shelly 3em
String shelly_ip = "";
int shelly_typ = 0 ; 

//nulleinspeisung
int ac_limit = 0;
int soyo_ac_out= 0;
int shelly_power = 0;


AsyncWebServer server(80);
AsyncEventSource events("/events");
AsyncDNSServer dns;

const char* PARAM_MESSAGE = "message";


//flag for saving data
bool shouldSaveConfig = false;

//callback notifying us of the need to save config
void saveConfigCallback () {
  DEBUG_SERIAL.println("Should save config");
  shouldSaveConfig = true;
}


void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}


String processor(const String& var){
  //DEBUG_SERIAL.println(var);

  if(var == "STATICPOWER"){
    return String(value_power);
  }
  else if(var == "WIFIRSSI"){
    return String(rssi);      
  }
  else if(var == "CLIENTID"){
    return String(clientId);
  }
  else if(var == "UPTIME"){
    myUptime();
    return Uptime_Str;
  }
  else if(var == "SOYOTEXT"){
    return soyo_text;
  }
  else if(var == "MQTTROOT"){
    return mqtt_root;
  }
  else if(var == "MQTTSTATE"){
    return mqtt_state;
  }
  else if(var == "CBSTATE1"){
    return html_checkbox1;
  }
  else if(var == "CBSTATE2"){
    return html_checkbox2;
  }
  else if(var == "CBSTATE3"){
    return html_checkbox3;
  }
  else if(var == "CBSTATE4"){
    return html_checkbox4;
  }
  else if(var == "TIME1"){
    return html_time1;
  }
  else if(var == "TIME2"){
    return html_time2;
  }
  else if(var == "WATT1"){
    return html_watt1;
  }
  else if(var == "WATT2"){
    return html_watt2;
  }
  else if(var == "METERTYP"){
    return html_metertyp;
  }
  else if(var == "METERIPADDR"){
    return html_meteripaddr;
  }
  else if(var == "MAXWATT"){
    return html_maxwatt;
  }
      
  return String();
}


void reconnect() {
  DEBUG_SERIAL.println("reconnect MQTT connection!");

  //set callback again
  client.setCallback(mqtt_callback);
  
  uint8_t timeout = 15;

  // wait for connection
  while (!client.connected()){

    DEBUG_SERIAL.println();
        
    if (client.connect(clientId)) {
      DEBUG_SERIAL.println("connection established");

      client.publish(topic_alive, "1");
      client.publish(topic_power, "0"); 
      client.subscribe(topic_power);

      strcpy(mqtt_state, "connected");

      DEBUG_SERIAL.print("subscrible: ");
      DEBUG_SERIAL.print(topic_power);
      DEBUG_SERIAL.println(" ");
    } else {
      DEBUG_SERIAL.println("reconnect failed! ");
      strcpy(mqtt_state, "connection error");
      
      //give a litle time to connect
      while (timeout){
        DEBUG_SERIAL.print(".");
        timeout--;
        delay(1000);
      }
    }

  }

  events.send(mqtt_state, "mqttstate", millis());

  if(!mqttenabled){
    strcpy(mqtt_state, "disabled");
    events.send(mqtt_state, "mqttstate", millis());
  }
  
}


//callback from mqtt
void mqtt_callback(char* topic, byte* payload, unsigned int length) {  
  unsigned int i = 0;
  for (i=0;i<length;i++) {
    buffer[i] = char(payload[i]);
  }

  buffer[i] = '\0';
    
  int arrived_value = atoi(buffer);
  if (arrived_value >= 0 && arrived_value <= 3000) {
    value_power = arrived_value;
  }
  
}


void setSoyoPowerData(int power){
  soyo_power_data[0] = 0x24;
  soyo_power_data[1] = 0x56;
  soyo_power_data[2] = 0x00;
  soyo_power_data[3] = 0x21;
  soyo_power_data[4] = power >> 0x08;
  soyo_power_data[5] = power & 0xFF;
  soyo_power_data[6] = 0x80;
  soyo_power_data[7] = calc_checksumme(soyo_power_data[1], soyo_power_data[2], soyo_power_data[3], soyo_power_data[4], soyo_power_data[5], soyo_power_data[6]);
}


int calc_checksumme(int b1, int b2, int b3, int b4, int b5, int b6 ){
  int calc = (0xFF - b1 - b2 - b3 - b4 - b5 - b6) % 256;
  return calc & 0xFF;
}


void myUptime(){
  uptime.calculateUptime();                                  

  // Get The Uptime Values To Global Variables
  Uptime_Years      = uptime.getYears();
  Uptime_Months     = uptime.getMonths();
  Uptime_Days       = uptime.getDays();
  Uptime_Hours      = uptime.getHours();
  Uptime_Minutes    = uptime.getMinutes();
  Uptime_Seconds    = uptime.getSeconds();
  Uptime_TotalDays  = uptime.getTotalDays();

  if (Uptime_Years == 0U) {                                  // Uptime Is Less Than One Year
    // First 60 Seconds
    if (Uptime_Minutes == 0U && Uptime_Hours == 0U && Uptime_Days == 0U && Uptime_Months == 0U)
      sprintf(Uptime_Str, "00:00:%02i", Uptime_Seconds);
    // First Minute
    else if (Uptime_Minutes == 1U && Uptime_Hours == 0U && Uptime_Days == 0U && Uptime_Months == 0U)
      sprintf(Uptime_Str, "00:%02i:%02i", Uptime_Minutes, Uptime_Seconds);
    // Second Minute And More But Less Than Hours, Days, Months
    else if (Uptime_Minutes >= 2U && Uptime_Hours == 0U && Uptime_Days == 0U && Uptime_Months == 0U)
      sprintf(Uptime_Str, "00:%02i:%02i", Uptime_Minutes, Uptime_Seconds);
    // First Hour And More But Less Than Days, Months
    else if (Uptime_Hours >= 1U && Uptime_Days == 0U && Uptime_Months == 0U)
      sprintf(Uptime_Str, "%02i:%02i:%02i", Uptime_Hours, Uptime_Minutes, Uptime_Seconds);
    // First Day And Less Than Month
    else if (Uptime_Days == 1U && Uptime_Months == 0U)
      sprintf(Uptime_Str, "%iday %02i:%02i:%02i", Uptime_Days, Uptime_Hours, Uptime_Minutes, Uptime_Seconds);
    // Second Day And More But Less Than Month
    else if (Uptime_Days >= 2U && Uptime_Months == 0U)
      sprintf(Uptime_Str, "%idays %02i:%02i:%02i", Uptime_Days, Uptime_Hours, Uptime_Minutes, Uptime_Seconds);
    // First Month And More But Less Than One Year
    else if (Uptime_Months >= 1U)
      sprintf(Uptime_Str, "%im, %id %02i:%02i", Uptime_Months, Uptime_Days, Uptime_Hours, Uptime_Minutes);
    // If There Is Any Error In This If Loop Then Make Full String.
    else sprintf(Uptime_Str, "%iy %im %id %02i:%02i", Uptime_Years, Uptime_Months, Uptime_Days, Uptime_Hours, Uptime_Minutes);
  } else                                                     // Uptime Is More Than One Year
    sprintf(Uptime_Str, "%iy %im %id %02i:%02i", Uptime_Years, Uptime_Months, Uptime_Days, Uptime_Hours, Uptime_Minutes);
}


void saveConfig(){

  #if defined(ARDUINOJSON_VERSION_MAJOR) && ARDUINOJSON_VERSION_MAJOR >= 6
    DynamicJsonDocument json(1024);
  #else
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
  #endif
  json["mqtt_server"] = mqtt_server;
  json["mqtt_port"] = mqtt_port;

  if(mqttenabled){
    json["mqttenabled"] = "1";
  }else{
    json["mqttenabled"] = "0";
  }

  if(nulleinspeisung){
    json["nulleinspeisung"] = "1";
  }else{
    json["nulleinspeisung"] = "0";
  }
  
  json["ip"] = WiFi.localIP().toString();
  json["gateway"] = WiFi.gatewayIP().toString();
  json["subnet"] = WiFi.subnetMask().toString();
  json["time1"] = time_1;
  json["watt1"] = watt_1;
  json["time2"] = time_2;
  json["watt2"] = watt_2;
  json["maxwatt"] = maxwatt;
  json["meteripaddr"] = meteripaddr;
  
  
  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    DEBUG_SERIAL.println("failed to open config file for writing");
  }

  #if defined(ARDUINOJSON_VERSION_MAJOR) && ARDUINOJSON_VERSION_MAJOR >= 6
    serializeJson(json, Serial);
    serializeJson(json, configFile);
  #else
    json.printTo(Serial);
    json.printTo(configFile);
  #endif
  configFile.close();

}


void telnetConnected() {
  DEBUG_SERIAL.println(F("Telnet connection established."));
}

void telnetDisconnected() {
  DEBUG_SERIAL.println(F("Telnet connection closed."));
}

void disconnectClientWrapper() {
  SerialAndTelnet.disconnectClient();
}



int getShellyTyp(){ // get shelly typ(3em, 3em pro, 1pm...)

  String shelly_url = "http://" + shelly_ip +  "/shelly";
  int typ = 0;
 
  DynamicJsonDocument doc(2048);
 
  WiFiClient client_shelly;
  HTTPClient http;

    
  
    if (http.begin(client_shelly, shelly_url)) { 
    int httpCode = http.GET();
    if (httpCode > 0) {
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = http.getString();   
        DeserializationError error = deserializeJson(doc, payload);
        
        if (error) {
          DEBUG_SERIAL.print(F("deserializeJson() failed: "));
          DEBUG_SERIAL.println(error.f_str());
        }

        //test auf Shelly 3EM Pro
        //index = payload.indexOf("SPEM");
        if( payload.indexOf("SPEM") >= 0 ) {
          typ = shelly_3em_pro;
          memset(html_metertyp, 0, sizeof(html_metertyp)); 
          strcat(html_metertyp, "Shelly 3EM Pro");     
          //snprintf(html_metertyp, sizeof(html_metertyp), value, maxwatt);
        } 

        //test auf Shelly 3EM
        if( payload.indexOf("SEM") >= 0 ){
          typ = shelly_3em;
          memset(html_metertyp, 0, sizeof(html_metertyp)); 
          strcat(html_metertyp, "Shelly 3EM");   
        }

        //test auf Shelly EM
        if(payload.indexOf("SHEM") >= 0 ){
          typ = shelly_em;
          memset(html_metertyp, 0, sizeof(html_metertyp)); 
          strcat(html_metertyp, "Shelly EM");   
        }

         //test auf Shelly 1PM
        if(payload.indexOf("SHSW-PM") >= 0){
          typ = shelly_1pm;
          memset(html_metertyp, 0, sizeof(html_metertyp)); 
          strcat(html_metertyp, "Shelly 1PM");   
        } 


      }
    }
    http.end();
    }
 
  return typ;
}


// read shelly3EM
int http_get(int typ) {
  String shelly_url;
  int power = 0;
  int power1 = 0;
  int power2 = 0;
  int power3 = 0; 
  
  DynamicJsonDocument doc(2048);
 
  WiFiClient client_shelly;
  HTTPClient http;
   
  
  if (typ == 1) { 
    shelly_url = "http://" + shelly_ip +  "/rpc/Shelly.GetStatus"; // Shelly PRO 3EM
  } else if(typ == 2) {
    shelly_url = "http://" + shelly_ip +  "/status";  // Shelly 3EM und Andere
  } else{
    return 0;
  }             
  

  if (http.begin(client_shelly, shelly_url))  {  
    int httpCode = http.GET();         
    if (httpCode > 0) {
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = http.getString();   
        DeserializationError error = deserializeJson(doc, payload);
                
        if (error) {
          DEBUG_SERIAL.print(F("deserializeJson() failed: "));
          DEBUG_SERIAL.println(error.f_str());
        }

        if (typ == 1) {
          power1 = doc["em:0"]["a_act_power"];  //Shelly 3EM PRO
          power2 = doc["em:0"]["b_act_power"];
          power3 = doc["em:0"]["c_act_power"]; 
        } else if (typ == 2) {
          power1 = doc["emeter"]["0"]["power"]; //Shelly 3EM
          power2 = doc["emeter"]["1"]["power"]; 
          power3 = doc["emeter"]["2"]["power"]; 
        } else if (typ == 3) {
          power1 = doc["meters"]["0"]["power"]; // Shelly EM Kanal 1
          power2 = doc["meters"]["1"]["power"]; // Shelly EM Kanal 2
          power3 = 0; 
        } else if (typ == 4) {
          power1 = doc["meters"]["power"]; // Shelly 1PM
          power2 = 0;
          power3 = 0;   
        }
        
        power = power1 + power2 + power3;
      }
    } else {
      DEBUG_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      shelly_typ = 0;
    }
    http.end();
  } else {
    DEBUG_SERIAL.printf("[HTTP} Unable to connect\n");
    shelly_typ = 0;
  }

  return power;
}



//#################### SETUP #######################
void setup()  {

  SerialAndTelnet.setWelcomeMsg(F("Welcome to the TelnetSpy example\r\n\n"));
  SerialAndTelnet.setCallbackOnConnect(telnetConnected);
  SerialAndTelnet.setCallbackOnDisconnect(telnetDisconnected);
  SerialAndTelnet.setFilter(char(1), F("\r\nNVT command: AO\r\n"), disconnectClientWrapper);

  DEBUG_SERIAL.begin(115200);
  delay(250);

  WiFi.macAddress(mac);
  
  DEBUG_SERIAL.println("Start");
  DEBUG_SERIAL.printf("ESP_%02X%02X%02X", mac[3], mac[4], mac[5]);
  DEBUG_SERIAL.println();

  configTime(MY_TZ, MY_NTP_SERVER);
  
  //generate espid in hex like soyo_18d88d
  //clientId = "soyoxxxxxx";
  sprintf(clientId, "soyo_%02x%02x%02x", mac[3], mac[4], mac[5] );
  
  //mqtt_root = "SoyoSource/soyo_xxxxxx";
  strcat(mqtt_root, clientId);
  
  //topic_alive = "SoyoSource/soyo_xxxxxx/alive";
  strcat(topic_alive, mqtt_root);
  strcat(topic_alive, "/alive");

  //topic_power = "SoyoSource/soyo_xxxxxx/power";
  strcat(topic_power, mqtt_root);
  strcat(topic_power, "/power");
  
  // initalize basic html data
  snprintf(html_time1, sizeof(html_time1), value, time_1);
  snprintf(html_time1, sizeof(html_time2), value, time_2);
  snprintf(html_watt1, sizeof(html_watt1), value, watt_1);
  snprintf(html_watt2, sizeof(html_watt2), value, watt_2);
  snprintf(html_meteripaddr, sizeof(html_meteripaddr), value, meteripaddr);
  snprintf(html_maxwatt, sizeof(html_maxwatt), value, maxwatt);

   
  pinMode(SERIAL_COMMUNICATION_CONTROL_PIN, OUTPUT);
  digitalWrite(SERIAL_COMMUNICATION_CONTROL_PIN, RS485_RX_PIN_VALUE);
  RS485Serial.begin(4800);   // set RS485 baud

  //read configuration from FS json
  DEBUG_SERIAL.println("mounting FS...");

  if (SPIFFS.begin()) {
    DEBUG_SERIAL.println("mounted file system");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      DEBUG_SERIAL.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        DEBUG_SERIAL.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);

        #if defined(ARDUINOJSON_VERSION_MAJOR) && ARDUINOJSON_VERSION_MAJOR >= 6
          DEBUG_SERIAL.println("ARDUINOJSON_VERSION_MAJOR) && ARDUINOJSON_VERSION_MAJOR >= 6");
          DynamicJsonDocument json(1024);
          auto deserializeError = deserializeJson(json, buf.get());
          serializeJson(json, Serial);
          if ( ! deserializeError ) {
        #else
          DynamicJsonBuffer jsonBuffer;
          JsonObject& json = jsonBuffer.parseObject(buf.get());
          json.printTo(Serial);
          if (json.success()) {
        #endif
          DEBUG_SERIAL.println("\nparsed json");

          strcpy(mqtt_server, json["mqtt_server"]);
          strcpy(mqtt_port, json["mqtt_port"]);

          if(json.containsKey("mqttenabled")){
            char json_mqttstate[2];
            strcpy(json_mqttstate, json["mqttenabled"]);

            if(strcmp(json_mqttstate, "1") == 0){
              mqttenabled = true;
              memset(html_checkbox3, 0, sizeof(html_checkbox3));
              strcpy(html_checkbox3, checkbox_state_true);
            }else{
              mqttenabled = false;
              memset(html_checkbox3, 0, sizeof(html_checkbox3));
              strcpy(html_checkbox3, checkbox_state_false);
            }
          }

          if(json.containsKey("nulleinspeisung")){
            char json_nullstate[2];
            strcpy(json_nullstate, json["nulleinspeisung"]);

            if(strcmp(json_nullstate, "1") == 0){
              nulleinspeisung = true;
              memset(html_checkbox4, 0, sizeof(html_checkbox4));
              strcpy(html_checkbox4, checkbox_state_true);
            }else{
              nulleinspeisung = false;
              memset(html_checkbox4, 0, sizeof(html_checkbox4));
              strcpy(html_checkbox4, checkbox_state_false);
            }
          }

          if(json.containsKey("time1")){
            strcpy(time_1, json["time1"]);            
            memset(html_time1, 0, sizeof(html_time1));
            snprintf(html_time1, sizeof(html_time1), value, time_1);
          }

          if(json.containsKey("time2")){
            strcpy(time_2, json["time2"]);
            memset(html_time2, 0, sizeof(html_time2));
            snprintf(html_time2, sizeof(html_time2), value, time_2);
          }

          if(json.containsKey("watt1")){
            strcpy(watt_1, json["watt1"]);
            memset(html_watt1, 0, sizeof(html_watt1));
            snprintf(html_watt1, sizeof(html_watt1), value, watt_1);
          }

          if(json.containsKey("watt2")){
            strcpy(watt_2, json["watt2"]);  
            memset(html_watt2, 0, sizeof(html_watt2));
            snprintf(html_watt2, sizeof(html_watt2), value, watt_2);
          }

          if(json.containsKey("maxwatt")){
            strcpy(maxwatt, json["maxwatt"]);  
            memset(html_maxwatt, 0, sizeof(html_maxwatt));
            snprintf(html_maxwatt, sizeof(html_maxwatt), value, maxwatt);
          }

          if(json.containsKey("meteripaddr")){
            strcpy(meteripaddr, json["meteripaddr"]);  
            memset(html_meteripaddr, 0, sizeof(html_meteripaddr));
            snprintf(html_meteripaddr, sizeof(html_meteripaddr), value, meteripaddr);
            shelly_ip = String(meteripaddr);
          }

        } else {
          DEBUG_SERIAL.println("failed to load json config");
        }
      }
    }
  } else {
    DEBUG_SERIAL.println("failed to mount FS");
  }
  //end read

  
  WiFi.persistent(true); // sonst verliert er nach einem Neustart die IP !!!

  ESPAsync_WMParameter custom_mqtt_server("server", "mqtt server", mqtt_server, 40);
  ESPAsync_WMParameter custom_mqtt_port("port", "mqtt port", mqtt_port, 5); 

  ESPAsync_WiFiManager wifiManager(&server,&dns);

  
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  wifiManager.setConfigPortalTimeout(180);
  wifiManager.addParameter(&custom_mqtt_server);
  wifiManager.addParameter(&custom_mqtt_port);

  String apName = "soyo_esp_" + String(mac[3], HEX) + String(mac[4], HEX) + String(mac[5], HEX);

  bool res;
  res = wifiManager.autoConnect(apName.c_str());

  if(!res) {
    DEBUG_SERIAL.println("Failed to connect");
    ESP.restart();
  }
  else {
    //if you get here you have connected to the WiFi    
    DEBUG_SERIAL.print("WiFi connected to ");
    DEBUG_SERIAL.println(String(WiFi.SSID()));
    DEBUG_SERIAL.print("RSSI = ");
    DEBUG_SERIAL.print(String(WiFi.RSSI()));
    DEBUG_SERIAL.println(" dB");
    DEBUG_SERIAL.print("IP address  ");
    DEBUG_SERIAL.println(WiFi.localIP());
    DEBUG_SERIAL.println();

    //read updated parameters
    strcpy(mqtt_server, custom_mqtt_server.getValue());
    strcpy(mqtt_port, custom_mqtt_port.getValue());
    
    //save the custom parameters to FS
    if (shouldSaveConfig) {
      saveConfig();
    }

    DEBUG_SERIAL.println("set mqtt server!");
    DEBUG_SERIAL.println(String("mqtt_server: ") + mqtt_server);
    DEBUG_SERIAL.println(String("mqtt_port: ") + mqtt_port);

    client.setServer(mqtt_server, atoi(mqtt_port));
    client.setCallback(mqtt_callback);
    client.publish(topic_alive, "0");
    

    // Handle Web Server
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "text/html", index_html, processor);
    });

    // Handle Web Server Events
    events.onConnect([](AsyncEventSourceClient *client){
      if(client->lastId()){
        DEBUG_SERIAL.println();
        DEBUG_SERIAL.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
        DEBUG_SERIAL.println();
      }
      client->send("hello!", NULL, millis(), 10000);

    });

    
    // start AP Mode
    server.on("/apmode", HTTP_GET, [](AsyncWebServerRequest *request) {
      ESPAsync_WiFiManager wifiManager(&server,&dns);
      wifiManager.resetSettings();
      
      ESP.restart();
      request->send_P(200, "text/html", index_html, processor);
    });


    // restart system
    server.on("/restart", HTTP_GET, [](AsyncWebServerRequest *request) {   
      DEBUG_SERIAL.println("/restart");  
      ESP.restart();
      request->send_P(200, "text/html", index_html, processor);
    });


    server.on("/acoutput", HTTP_GET, [] (AsyncWebServerRequest *request) {
      String parm1;
  
      if (request->hasParam("value") ) {
        parm1 = request->getParam("value")->value();
        DEBUG_SERIAL.print("/acoutput?value = ");
        DEBUG_SERIAL.println(parm1);

        if(parm1.equals("/s0") ){
          value_power = 0;
          sprintf(msgData, "%d", value_power);
          if(mqttenabled){
            client.publish(topic_power, msgData);
          }
        }
        else if(parm1.equals("/p1")){
          value_power +=1;
          sprintf(msgData, "%d", value_power);
          if(mqttenabled){
            client.publish(topic_power, msgData);
          }
        }
        else if(parm1.equals("/p10")){
          value_power +=10;
          sprintf(msgData, "%d", value_power);
          if(mqttenabled){
            client.publish(topic_power, msgData);
          }
        }
        else if(parm1.equals("/m1")){
          value_power -=1;
          if(value_power < 0){
            value_power = 0;
           }
          sprintf(msgData, "%d", value_power);
          if(mqttenabled){
            client.publish(topic_power, msgData);
          }
        }
        else if(parm1.equals("/m10")){
          value_power -=10;
          if(value_power < 0){
            value_power = 0;
           }
          sprintf(msgData, "%d", value_power);
          if(mqttenabled){
            client.publish(topic_power, msgData);
          }
        }
      }
      request->send_P(200, "text/html", index_html, processor);
    });


    
    server.on("/checkbox", HTTP_GET, [] (AsyncWebServerRequest *request) {
      String checkbox_id;
      String checkbox_value;
     
      if (request->hasParam("cbid") && request->hasParam("state")) {
        checkbox_id = request->getParam("cbid")->value();
        checkbox_value = request->getParam("state")->value();
       
        if(checkbox_id.equals("cb1")){
          memset(html_checkbox1, 0, sizeof(html_checkbox1));

          if(checkbox_value.equals("1")){
            strcpy(html_checkbox1, checkbox_state_true);
            checkboxT1 = true;
          }
          else {
            strcpy(html_checkbox1, checkbox_state_false);
            checkboxT1 = false;
          }
        }
        else if(checkbox_id.equals("cb2")){
          memset(html_checkbox2, 0, sizeof(html_checkbox2));

          if(checkbox_value.equals("1")){
            strcpy(html_checkbox2, checkbox_state_true);
            checkboxT2 = true;
          }
          else { 
            strcpy(html_checkbox2, checkbox_state_false);
            checkboxT2 = false;
          }
        }
        else if(checkbox_id.equals("cb3")){
          memset(html_checkbox3, 0, sizeof(html_checkbox3));

          if(checkbox_value.equals("1")){
            strcpy(html_checkbox3, checkbox_state_true);
            mqttenabled = true;
          }
          else {
            strcpy(html_checkbox3, checkbox_state_false);
            mqttenabled = false;
          }
        }
        else if(checkbox_id.equals("cb4")){
          memset(html_checkbox4, 0, sizeof(html_checkbox4));

          if(checkbox_value.equals("1")){
            strcpy(html_checkbox4, checkbox_state_true);
            nulleinspeisung = true; 
          }
          else {
            strcpy(html_checkbox4,checkbox_state_false);
            nulleinspeisung = false; 
          }
        }
      }
      
      request->send_P(200, "text/html", index_html, processor);
    });


    server.on("/savesettings", HTTP_GET, [] (AsyncWebServerRequest *request) {
      String value1;
      String value2;
    
      if (request->hasParam("t1") && request->hasParam("w1")) {
        
        value1 = request->getParam("t1")->value();
        value2 = request->getParam("w1")->value();

        memset(time_1, 0, sizeof(time_1)); 
        strcat(time_1, value1.c_str());     
        
        memset(watt_1, 0, sizeof(watt_1)); 
        strcat(watt_1, value2.c_str());     

        memset(html_time1, 0, sizeof(html_time1));
        snprintf(html_time1, sizeof(html_time1), value, time_1);

        memset(html_watt1, 0, sizeof(html_watt1));
        snprintf(html_watt1, sizeof(html_watt1), value, watt_1);
      }
      else if (request->hasParam("t2") && request->hasParam("w2")) {
        
        value1 = request->getParam("t2")->value();
        value2 = request->getParam("w2")->value();

        memset(time_2, 0, sizeof(time_2)); 
        strcat(time_2, value1.c_str());     
        
        memset(watt_2, 0, sizeof(watt_2)); 
        strcat(watt_2, value2.c_str());     
               
        memset(html_time2, 0, sizeof(html_time2));
        snprintf(html_time2, sizeof(html_time2), value, time_2);
       
        memset(html_watt2, 0, sizeof(html_watt2));
        snprintf(html_watt2, sizeof(html_watt2), value, watt_2);
      }
      else if (request->hasParam("maxwatt") && request->hasParam("meteripaddr")) {
        
        value1 = request->getParam("maxwatt")->value();
        value2 = request->getParam("meteripaddr")->value();

        memset(maxwatt, 0, sizeof(maxwatt)); 
        strcat(maxwatt, value1.c_str());     
        
        memset(meteripaddr, 0, sizeof(meteripaddr)); 
        strcat(meteripaddr, value2.c_str());      

        memset(html_maxwatt, 0, sizeof(html_maxwatt));
        snprintf(html_maxwatt, sizeof(html_maxwatt), value, maxwatt);
       
        memset(html_meteripaddr, 0, sizeof(html_meteripaddr));
        snprintf(html_meteripaddr, sizeof(html_meteripaddr), value, meteripaddr);
      }

      saveConfig(); 
      shelly_ip = String(meteripaddr);

      request->send_P(200, "text/html", index_html, processor);
    });

   
    AsyncElegantOTA.begin(&server);    

    server.onNotFound(notFound);
    server.addHandler(&events);
    server.begin();
    DEBUG_SERIAL.println("Server start");

    //initial update webif & uptime  & mqtt_root 
    rssi = WiFi.RSSI();
    events.send(String(rssi).c_str(), "wifirssi", millis());
   
    myUptime();
    events.send(Uptime_Str, "uptime", millis());
    events.send(mqtt_root, "mqttroot", millis());
    events.send(watt_1, "w1", millis());
    events.send(watt_2, "w2", millis());
    events.send(mqtt_state, "mqttstate", millis());
    events.send(meteripaddr,"meteripaddr", millis());
    events.send(maxwatt, "maxwatt", millis());

    digitalWrite(SERIAL_COMMUNICATION_CONTROL_PIN, RS485_TX_PIN_VALUE); // RS485 Modul -> set board to transmit 
  }

  shelly_typ = getShellyTyp(); // get shelly typ, 3em / 3empro
  
  // end setup()  
}


void loop() {

  SerialAndTelnet.handle();

  if(mqttenabled){
    if (!client.connected()) {
      DEBUG_SERIAL.println("lost mqtt connection -> start reconncect");
      reconnect();
    }
    client.loop(); 
  }

  
  // send power to SoyoSource every 1000 ms
  if ((millis() - lastTimeSoyo) > timerDelaySoyo) {

    setSoyoPowerData(value_power);

    // send data to RS485 
    for(int i=0; i<8; i++){
      RS485Serial.write(soyo_power_data[i]);
      //DEBUG_SERIAL.print(soyo_power_data[i], HEX);
      //DEBUG_SERIAL.print(" ");
    }
    
    if(value_power != old_value_power){
      old_value_power = value_power;

      // send data to website if new data available
      events.send(String(value_power).c_str(),"staticpower", millis());
      
      DEBUG_SERIAL.print("new value_power = ");
      DEBUG_SERIAL.print(value_power);
      DEBUG_SERIAL.print(" ( ");
      DEBUG_SERIAL.printf("%02X %02X %02X %02X %02X %02X %02X %02X",soyo_power_data[0],soyo_power_data[1],soyo_power_data[2],soyo_power_data[3],soyo_power_data[4],soyo_power_data[5],soyo_power_data[6],soyo_power_data[7]);
      DEBUG_SERIAL.print(" ) ");
      DEBUG_SERIAL.println();
      DEBUG_SERIAL.println();
    }
    
    
    if(mqttenabled){
      client.publish(topic_alive, "0");
      client.publish(topic_alive, "1");
    }
    
    //DEBUG_SERIAL.println();
        
    if(checkboxT1 || checkboxT2){
      time(&now);
      localtime_r(&now, &tm);
    }
      
    if (checkboxT1 == true){      
      int t1_hour = String(time_1).substring(0,2).toInt();
      int t1_min = String(time_1).substring(3).toInt();

      if((tm.tm_hour == t1_hour && tm.tm_min == t1_min && tm.tm_sec == 0) || (tm.tm_hour == t1_hour && tm.tm_min == t1_min && tm.tm_sec == 1) ){
        value_power = atoi(watt_1);
        sprintf(msgData, "%d", value_power);
        if(mqttenabled){
          client.publish(topic_power, msgData);
        }
      }
    }

    if (checkboxT2 == true){    
      int t2_hour = String(time_2).substring(0,2).toInt();
      int t2_min = String(time_2).substring(3).toInt();  
      
      if((tm.tm_hour == t2_hour && tm.tm_min == t2_min && tm.tm_sec == 0) || (tm.tm_hour == t2_hour && tm.tm_min == t2_min && tm.tm_sec == 1)){
        value_power = atoi(watt_2);
        sprintf(msgData, "%d", value_power);
        if(mqttenabled){
          client.publish(topic_power, msgData);
        }
      }
    }
    
    lastTimeSoyo = millis();
  }


  // timer to update html 'Uptime' (60 sek)
  if ((millis() - lastTimeUptime) > timerDelayUptime) {  
    myUptime();
    events.send(Uptime_Str, "uptime", millis());
    lastTimeUptime = millis();
  }


  // timer to get Shelly3EM data (2 sek)
  if ((millis() - lastTimeShelly) > timerDelayShelly) {  

    if (shelly_typ > 0){
      shelly_power = http_get(shelly_typ); // get shelly power
      events.send(String(shelly_power).c_str(),"meterpower", millis()); //update html

      //DEBUG_SERIAL.print("Shelly3EM power = ");
      //DEBUG_SERIAL.print(shelly_power);
      //DEBUG_SERIAL.println();
    } else{
      DEBUG_SERIAL.println("Kein Shelly erkannt! Bitte IP eintragen, Speichern und Gerät neu starten.");
    }
    
    lastTimeShelly = millis();
  }


  //timer to update soyo power nulleinspeisung (10 sek)
  if ((millis() - lastTimeNES) > timerDelayNES) { 

    if(nulleinspeisung){      
      int limit = String(maxwatt).toInt();
        
      if(shelly_power > 20){  
        soyo_ac_out += shelly_power - 10;

        if(soyo_ac_out > ac_limit){
          soyo_ac_out = limit;
        } 
      } 

      if(shelly_power < 0){
        soyo_ac_out -= shelly_power + 10;

        if(soyo_ac_out < 0){
          soyo_ac_out = 0;
        } 
      }

      value_power = soyo_ac_out;
    }

    lastTimeNES = millis();
  }


  
}



