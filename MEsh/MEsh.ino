#include<ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include<WebSocketsClient.h>
#include<WebSocketsServer.h>
#include <ArduinoJson.h>
#include<WiFiUDP.h>
#include<FS.h>

WebSocketsClient websocket;
ESP8266WiFiMulti wifiMulti;
WebSocketsServer ChildSocket = WebSocketsServer(81);

char recievingdata[15];
int childsocketport = 81;
 WiFiUDP recievingudp;

String  ssid[]={"Board1","parent","PHANI"};
String psd[]={"password","password","wicked4me2c"};
char host[15];

bool socket = false;

int udplocalport = 4120;
int clients;

DynamicJsonBuffer  db,present;   
String data; //used for storing data from SPIFFS
int i = 1; // for room 1 board
void setup(){
  WiFi.mode(WIFI_AP_STA);
    Serial.begin(9600);
    MeshNet();
   // Connect();
    
    
    WiFi.softAP("Board1","password",1,false);  // true for hidden
    recievingudp.begin(udplocalport);
    GetFile();
}



void loop(){
    ClientChange();
    GetdataUDP();
    //  Getrequest();
    websocket.loop();
    ChildSocket.loop();
    // GET();
}







//------------------------------------------------------- FUNCTIONS-----------------------------------------------------------//
extern "C" {
    #include "user_interface.h"
}

void GetFile(){
     
    SPIFFS.begin();
    File Data;
    
    Data = SPIFFS.open("/data.json","r+");
    if (!Data) {
    Serial.println("file open failed");
    } 
    std::unique_ptr<char[]> buf {new char [1000]}; 
    Data.readBytes(buf.get(),1000);
    data = buf.get();
    // JsonObject& root = jsonBuffer.parseObject(data);
    
    // root.prettyPrintTo(database);
    //  Serial.println(database);
  }

void DataCheck(char* _datacheck){
    
      JsonObject& dbroot = db.parseObject(data);
      JsonObject& presentroot = present.parseObject(_datacheck);
      
     // Serial.println(dbroot["SwitchBoardDetails"][i - 1]["RoomId"].print);
        if(dbroot["SwitchBoardDetails"][i - 1]["RoomId"] == presentroot["RoomId"]){
          presentroot.printTo(Serial);
          
          
          }    
  }

void client_status(){
    unsigned char number_client;
    struct station_info *stat_info;
    struct ip_addr *IPaddress;
    IPAddress address;
    int i=1;
    number_client= wifi_softap_get_station_num(); // Count of stations which are connected to ESP8266 soft-AP
    stat_info = wifi_softap_get_station_info();
    Serial.print(" Total connected_client are = ");
    Serial.print(number_client);
    while (stat_info != NULL) {
    IPaddress = &stat_info->ip;
    address = IPaddress->addr;
    Serial.print("client= ");
    Serial.print(i);
    Serial.print(" ip adress is = ");
    Serial.print((address));
    Serial.print(" with mac adress is = ");
    Serial.print(stat_info->bssid[0],HEX);
    Serial.print(stat_info->bssid[1],HEX);
    Serial.print(stat_info->bssid[2],HEX);
    Serial.print(stat_info->bssid[3],HEX);
    Serial.print(stat_info->bssid[4],HEX);
    Serial.print(stat_info->bssid[5],HEX);
    SendUDP(WiFi.localIP().toString(),address);
    stat_info = STAILQ_NEXT(stat_info, next);
    i++;
    Serial.println();
    }
    delay(500);
} 
void  ClientChange(){
    if(clients != wifi_softap_get_station_num()){
    delay(3000);
    client_status();
    clients = clients = wifi_softap_get_station_num();
     }
  }
void SendUDP(String _data,IPAddress _address){
    WiFiUDP sendingUDP;
    sendingUDP.beginPacket(_address,udplocalport);
    char _datac[_data.length()+1];
    _data.toCharArray(_datac,_data.length()+1);
    sendingUDP.write(_datac);
    sendingUDP.endPacket();
    Serial.println("data send");
  }
void GetdataUDP(){
    int packetSize = recievingudp.parsePacket();
    if(packetSize){
    int len = recievingudp.read(recievingdata,15);
    Serial.print("Got data");
    Serial.println(recievingdata);
    websocket.begin(recievingdata,childsocketport,"/");
    websocket.onEvent(websocketevent);
    }
 }
void MeshNet( ){ 

  int numberOfNetworks = WiFi.scanNetworks();
 
  for(int i =0; i<numberOfNetworks; i++){
 
      Serial.println("Network name: ");
      Serial.println(WiFi.SSID(i));
      Serial.print("Signal strength: ");
      Serial.println(WiFi.RSSI(i));
      Serial.println("-----------------------");
 
  }
    char ssidname1[ssid[0].length() +1], ssidname2[ssid[1].length()+1 ],   ssidname3[ssid[2].length()+1] ;
    char pass1[psd[0].length()+1],  pass2[psd[1].length()+ 1],  pass3[psd[2].length()+1];
    ssid[0].toCharArray(ssidname1,ssid[0].length() +1);
    ssid[1].toCharArray(ssidname2,ssid[1].length()+1 );
    ssid[2].toCharArray(ssidname3,ssid[2].length()+1 );
    psd[0].toCharArray(pass1,psd[0].length()+1 ); 
    psd[1].toCharArray(pass2,psd[1].length()+ 1);  
    psd[2].toCharArray(pass3,psd[2].length()+1 );   
    delay(10);
    Serial.println('\n');
    wifiMulti.addAP(ssidname1,pass1);   // add Wi-Fi networks you want to connect to
    wifiMulti.addAP(ssidname2,pass2);
    wifiMulti.addAP(ssidname3,pass3);
    Serial.println("Connecting ...");
    int i = 0;
    while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(1000);
    Serial.print('.');
    }
    Serial.println('\n');
    Serial.print("Connected to ");
    Serial.println(WiFi.SSID());              // Tell us what network we're connected to
    Serial.print("IP address:\t");
    Serial.println(WiFi.localIP());           // Send the IP address of the ESP8266 to the computer
    ChildSocket.begin();
    ChildSocket.onEvent(childevent);
}
void Connect(){
    // WiFi.config(local_IP, gateway, subnet);
  WiFi.begin("Board","password");
    while(WiFi.status() != WL_CONNECTED){Serial.println(".");delay(1000);}
}
void GET(){
    if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    Serial.println("request");
    HTTPClient http;  //Declare an object of class HTTPClient
    
    http.begin("192.168.0.10");  //Specify request destination
    
    int httpCode = http.GET();                                                                  //Send the request
    delay(2000);
    if (httpCode > 0) { //Check the returning code
    
    String payload = http.getString();   //Get the request response payload
    Serial.println(payload);                     //Print the response payload
    }
    http.end();   //Close connection
    }
    }
void Getrequest(){
    //Serial.print("connecting to ");
    // Serial.println(host);
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
    }
    String url = "/";
    //Serial.print("Requesting URL: ");
    //Serial.println(url);
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
    "Host: " + host + "\r\n" + 
    "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
    if (millis() - timeout > 8000) {
    // Serial.println(">>> Client Timeout !");
    client.stop();
    return;
    }
    }
    // Read all the lines of the reply from server and print them to Serial
    while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
    }
    Serial.println();
    // Serial.println("closing connection");
}
void statuschange(char* _data){
   DynamicJsonBuffer databuffer(200) ;   
   JsonObject& root = databuffer.parseObject(_data);
   if(root["switchno"] == "Switch1"){
          if(root["switchstate"] == "1"){
            //ledon 
            Serial.println("Switch 1 is on ");
            }
     } 
     if(root["switchno"] == "Switch2"){
          if(root["switchstate"] == "1"){
            //ledon 
            }
     } 
     if(root["switchno"] == "Switch3"){
          if(root["switchstate"] == "1"){
            //ledon 
            }
     } 
  }
void websocketevent(WStype_t type, uint8_t * payload, size_t length) {
    switch(type) {
    case WStype_DISCONNECTED:
    Serial.printf("[WSc] Disconnected!\n");
    break;
    case WStype_CONNECTED: {
    Serial.println("Listening to data");
    // Serial.printf("[WSc] Connected to url: %s\n", payload);
  //socket = true;
    
    // send message to server when Connected
    //  websocket.sendTXT("Connected");
    }
    break;
    case WStype_TEXT:
    Serial.printf("[WSc] get text: %s\n", payload);
    char* data;
    data = (char*)payload;
    ChildSocket.broadcastTXT(data);
    DataCheck(data);
    // send message to server
    // webSocket.sendTXT("message here");
    break;
    case WStype_BIN:
    Serial.printf("[WSc] get binary length: %u\n", length);
    // hexdump(payload, length);
    // send data to server
    // webSocket.sendBIN(payload, length);
    break;
    }
}
void childevent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    switch(type) {
    case WStype_DISCONNECTED:
    Serial.printf("[%u] Disconnected!\n", num);
    break;
    case WStype_CONNECTED:
    {
    IPAddress ip = ChildSocket.remoteIP(num);
    
    Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
    // send message to client
    // webSocket.sendTXT(num, "Connected");
    }
    break;
    case WStype_TEXT:
    
    //  Serial.printf("[%u] get Text: %s\n", num, payload);
    
    char* datagot;
    datagot = (char*)payload;
    Serial.print("data got");
    Serial.print(datagot);
    websocket.sendTXT(datagot);  // sending data to parent Server
    // send message to client
    // webSocket.sendTXT(num, "message here");
    // send data to all connected clients
    // webSocket.broadcastTXT("message here");
    break;
    case WStype_BIN:
    Serial.printf("[%u] get binary length: %u\n", num, length);
    //  hexdump(payload, length);
    // send message to client
    // webSocket.sendBIN(num, payload, length);
    break;
    }
      
      }


