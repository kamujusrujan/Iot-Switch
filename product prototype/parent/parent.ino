
#include <ESP8266WebServer.h>
#include<ESP8266WiFi.h>
#include<WebSocketsServer.h>
#include<WebSocketsClient.h>
#include <ArduinoJson.h>
#include<WiFiUDP.h>
#include <ESP8266mDNS.h>
#include <AsyncDelay.h>
#include <FS.h>

      String ssid;
      String password;
      WebSocketsServer localsocket = WebSocketsServer(81);
      
      WebSocketsClient clientfrommain;
      WiFiUDP udp;
      String revdata;
      String database;
      int udplocalport = 4120; 
      String IPs[2];
      AsyncDelay delay2;
      
      
      char mainserver[] = "webgenie.com"; 
      
      IPAddress local_IP(192,168,4,1);  // parent ip
      IPAddress gateway(192,168,4,1);
      IPAddress subnet(255,255,255,0);
      
      ESP8266WebServer server(80);
      WiFiClient client;
      
      char _reply[50];

      unsigned long referencetime;
      unsigned long currenttime;
      int clientstatus = 0;

        String data;

      

void setup(){
     
      WiFi.mode(WIFI_AP_STA);
      Serial.begin(9600);
      WiFi.softAP("parent","password",1,0);
      WiFi.begin("PHANI","wicked4me2c");
      
      
      
      
      // WiFi.config(local_IP, gateway, subnet);
      while ( WiFi.status() != WL_CONNECTED ) {
      
      Serial.print ( "." );
      Serial.println("Please wait while connecting to your Home Network " + ssid);
      Serial.println(WiFi.gatewayIP());
      delay(1000);
      }
      
      if (!MDNS.begin("esp8266")) {
      Serial.println("Error setting up MDNS responder!");
      
      }
      server.on("/",HomePage);
      server.on("/data",Database);
      server.on("/SSID",SetSSID);
      server.begin();
      Serial.println( WiFi.localIP());
      localsocket.begin();
      localsocket.onEvent(serverSocketEvent);  
      udp.begin(udplocalport);
      //  udpbroadcast(WiFi.localIP());
     
       delay2.start(3000, AsyncDelay::MILLIS);

}

void loop(){
      localsocket.loop();
      server.handleClient();
      GetdataUDP();
      ClientCheck(1);
     
}










//------------------------------------------------------- FUNCTIONS-----------------------------------------------------------//
extern "C" {
      #include "user_interface.h"
}

void GetFile(){
    DynamicJsonBuffer  jsonBuffer;    
    SPIFFS.begin();
    File Data;
    
    Data = SPIFFS.open("/data.json","r+");
    if (!Data) {
    Serial.println("file open failed");
    } 
    std::unique_ptr<char[]> buf {new char [1000]}; 
    Data.readBytes(buf.get(),1000);
    data = buf.get();
    JsonObject& root = jsonBuffer.parseObject(data);
    
    // root.prettyPrintTo(database);
    //  Serial.println(database);
  }
void ClientCheck(int interval){
  
          if(clientstatus != wifi_softap_get_station_num() ){
            
            udpips();
           // clientstatus = wifi_softap_get_station_num();
            }
}
void udpips(){
      unsigned char number_client;
      
      struct station_info *stat_info;
     
      struct ip_addr *IPaddress;
      IPAddress address;
      int i=1;
      
      number_client= wifi_softap_get_station_num(); // Count of stations which are connected to ESP8266 soft-AP
      clientstatus = number_client;
      
      
      Serial.print(" Total connected_client are = ");
      Serial.println(number_client);
      delay(2000);
       stat_info = wifi_softap_get_station_info();
       
       
    while (stat_info != NULL) {
      
      IPaddress = &stat_info->ip;
      address = IPaddress->addr;
      
      Serial.print("client= ");
      
      Serial.print(i);
      Serial.print(" ip adress is = ");
      Serial.print((address));
      udpbroadcast(WiFi.localIP().toString(),address);
      /*
      Serial.print(" with mac adress is = ");
      
      Serial.print(stat_info->bssid[0],HEX);
      Serial.print(stat_info->bssid[1],HEX);
      Serial.print(stat_info->bssid[2],HEX);
      Serial.print(stat_info->bssid[3],HEX);
      Serial.print(stat_info->bssid[4],HEX);
      Serial.print(stat_info->bssid[5],HEX);
      */
      stat_info = STAILQ_NEXT(stat_info, next);
      i++;
      Serial.println();
      }
      }
void GetdataUDP(){
      char recievingdata[100];
      int packetSize = udp.parsePacket();
      if(packetSize ){
      int len = udp.read(recievingdata, 100);
      
      Serial.println(recievingdata);
      // host = recievingdata;
      
      }

}
void SetSSID(){
      String html = "<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js\"></script>\n\nSSID: <input type=\"text\" name=\"usrname\" id =\"SSID\"><br>\nPassword: <input type=\"text\" name=\"usrname\" id =\"Password\"><br>\n\n <input type=\"submit\" value=\"Submit\" id =\"send\">\n\n\n<script type=\"text/javascript\">\n\t\n\tvar SSID =  document.getElementById(\"SSID\");\n\tvar Password =  document.getElementById(\"Password\");\n\tvar btn = document.getElementById(\"send\");\n\n\tbtn.addEventListener(\"click\",function(){\n\n\t\t// alert(\tSSID.value + \"   \" + Password.value );\n\t\t$.post(\"http://192.168.4.1:80/SSID\",{SSID:SSID.value,Password:Password.value},function(){\n\t\t\talert(\"send\");\n\n\t\t});\n\n\t});\n</script>\n\n\n\n\n\n\n<link href=\"https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0-beta/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha384-/Y6pD6FV/Vv2HJnA6t+vslU6fwYXjCFtcEpHbNJ0lyAFsXTsjBbfaDjzALeQsN6M\" crossorigin=\"anonymous\">";
      server.send(200,"text/html",html);
      char ssid[50],password[50];
      server.arg("SSID").toCharArray(ssid,50);
      server.arg("Password").toCharArray(password,50);
      //  udpbroadcast( WiFi.localIP().toString() );
      Serial.println(WiFi.subnetMask() );
      Serial.println( server.arg("SSID")  + "   " +  server.arg("Password"));
    
      if( server.arg("SSID") != WiFi.SSID() ){
              
      
      WiFi.begin(ssid,password);
      Serial.println(WiFi.localIP() + "is IP");
      // udpbroadcast(WiFi.localIP());
      if (!MDNS.begin("esp8266")) {
      Serial.println("Error setting up MDNS responder!");
      } Serial.println("Sucesss local");
}}
void udpbroadcast(String _data,IPAddress ip){


      udp.beginPacket(ip,udplocalport);  
      char sendingdata[_data.length()];
      _data.toCharArray(sendingdata,_data.length()+ 1 );
      // udp.beginPacketMulticast(all, udplocalport, WiFi.localIP());
      udp.write(sendingdata);
      Serial.println("the data send is " + _data);
      
      udp.endPacket();
}
void HomePage(){

      String Mainpage ="<link href=\"https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0-beta/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha384-/Y6pD6FV/Vv2HJnA6t+vslU6fwYXjCFtcEpHbNJ0lyAFsXTsjBbfaDjzALeQsN6M\" crossorigin=\"anonymous\">\n\nSwitch State and change </br>\n\n<input type=\"submit\" value=\"ON 1\"  id=\"Switch1\">\n<input type=\"submit\" value=\"ON 2\" id=\"Switch2\">\n<input type=\"submit\" value=\"ON 3\" id=\"Switch3\">\n<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js\"></script>\n<script type=\"text/javascript\">\n//-------------------DATA MANUPULATION ----------------\n\tvar SwitchState = [0,0,0];\n\tvar host =\"http://esp8266.local/\";\n\tvar data = \"0\";\n\tvar Present = [0,0,0];\n\tvar switch1 = document.getElementById(\"Switch1\");\n\tvar switch2 = document.getElementById(\"Switch2\");\n\tvar switch3 = document.getElementById(\"Switch3\");\n\tswitch1.addEventListener(\"click\",function(){\n\t\t\t\tif(Present[0]==0){\n\t\t\tPresent[0]=1;\n\t\t\tswitch1.value = \"OFF1\";\n\t}else {\n\t\t\tPresent[0] = 0;\n\t\t\tswitch1.value = \"ON1\";\n\t\t}\n\t\tdata = {\n\t\t\t\tSwitchNo:Switch1,\n\t\t\t\tstate:Present[0]\n\t\t\t};\n\t\t$.post(host,{switchstatus:Present[0],switch:\"Switch1\"},function(){\n\t});\n});\n\tswitch2.addEventListener(\"click\",function(){\n\t\t\tif(Present[1]==0){\n\t\t\tPresent[1]=1;\n\t\t\tswitch2.value = \"OFF1\";\n\t\t\t}else {\n\t\t\tPresent[1] = 0;\n\t\t\tswitch2.value = \"ON1\";\n\t\t}\n\t\tdata = {\"SwitchNo\":\"Switch2\",\n\t\t\t\t\"state\":Present[1] \n\t\t\t\t};\n\t\t\t$.post(host,{switchstatus:Present[1],switch:\"Switch2\"},function(){\n\n\t\t\t});\n});\n\tswitch3.addEventListener(\"click\",function(){\n\t\t\tif(Present[2]==0){\n\t\t\tPresent[2]=1;\n\t\t\tswitch3.value = \"OFF1\";\n\t\t}else {\n\t\t\tPresent[2] = 0;\n\t\t\tswitch3.value = \"ON1\";\n\t\t}\n\t\tdata = {\"SwitchNo\":\"Switch3\",\n\t\t\t\t\"state\":Present[2]\n\t\t\t};\n\t\t\t$.post(host,{switchstatus:Present[2],switch:\"Switch3\"},function(){\n\t\t});\n});\n</script>\n\n";
      server.send(200,"text/html",Mainpage);
      String switchno  = server.arg("switch");
      String  switchstate = server.arg("switchstatus");
      // Serial.print("the switch to control is " + switchno );
      //  Serial.print("the "+ switchno +" to toggle to " + switchstate );
      DynamicJsonBuffer jsonBuffer;
      String input =
      "{\"switchno\":\"switch1\",\"switchstate\":0}";
      JsonObject& root = jsonBuffer.parseObject(input);
      root["switchno"] = switchno;
      root["switchstate"] = switchstate;
      data ="";
      root.printTo(data);
      Serial.println(data);
      
      localsocket.broadcastTXT(data);
      Serial.println("data sending");
      

}
void Database(){
      StaticJsonBuffer<500> localdb;
      
      database = "{\"totalswitches \": 3,\"Switchstate\":000,\"data\":[48.756080,2.302038]}" ;
      JsonObject& root = localdb.parseObject(database);
      changedb();
      String output;
      root.printTo(output);
      
      
      server.send(200,"text/JSON",output);

}
void changedb(){

}
void serverSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

      switch(type) {
      case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
      case WStype_CONNECTED:
      {
      IPAddress ip = localsocket.remoteIP(num);
      Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
      // send message to client
      // webSocket.sendTXT(num, "Connected");
      }
      break;
      case WStype_TEXT:
      
      Serial.printf("[%u] get Text: %s\n", num, payload);
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

