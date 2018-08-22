#include "painlessMesh.h"
#include<ESP8266WebServer.h>
#include<ESP8266WiFi.h>


const char* meshSSID = "MeshTest";
const char* meshPSD = "password";
int port = 1234;
painlessMesh mesh;
uint32_t parentnode = 2133077307;
int hidden = 0; // 1 for hidden

ESP8266WebServer server(80);

void setup() {
  // put your setup code here, to run once:
      Serial.begin(9600);
      mesh.setDebugMsgTypes( ERROR | STARTUP );
      mesh.init(meshSSID, meshPSD,port,STA_AP,AUTH_WPA2_PSK,1,PHY_MODE_11G,82,hidden,4);
      mesh.onReceive(&receivedCallback);
      mesh.onNewConnection(&newConnectionCallback);
      mesh.onChangedConnections(&changedConnectionCallback);
      mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
      Serial.println( mesh.getNodeId() );
      server.on("/",Sampledata);
      server.begin();
     
      
      
}

void loop() {
  // put your main code here, to run repeatedly:
  mesh.update();  
  server.handleClient();
}






// ------------------------------------------------------------------
void Broadcastdata(String _data){
      mesh.sendBroadcast(_data);
      Serial.println(_data + " ----> is send");
  
  }
void CheckData(String _msg){
  
  }
void SendParent(String _data){
    mesh.sendSingle( parentnode ,_data);
    Serial.println("data send to parent is " + _data);
  }
void Sampledata(){
      String data = "\n\n  <link href=\"https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0-beta/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha384-/Y6pD6FV/Vv2HJnA6t+vslU6fwYXjCFtcEpHbNJ0lyAFsXTsjBbfaDjzALeQsN6M\" crossorigin=\"anonymous\">\n\n\n\n\n\nSwitch State and change </br>\n\n<input type=\"submit\" value=\"ON 1\"  id=\"Switch1\">\n<input type=\"submit\" value=\"ON 2\" id=\"Switch2\">\n<input type=\"submit\" value=\"ON 3\" id=\"Switch3\">\n<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js\"></script>\n<script type=\"text/javascript\">\n//-------------------DATA MANUPULATION ----------------\n\tvar SwitchState = [0,0,0];\n\tvar host =\"http://esp8266.local/\";\n\tvar data = \"0\";\n\tvar Present = [0,0,0];\n\tvar switch1 = document.getElementById(\"Switch1\");\n\tvar switch2 = document.getElementById(\"Switch2\");\n\tvar switch3 = document.getElementById(\"Switch3\");\n\tswitch1.addEventListener(\"click\",function(){\n\t\t\t\tif(Present[0]==0){\n\t\t\tPresent[0]=1;\n\t\t\tswitch1.value = \"OFF1\";\n\t}else {\n\t\t\tPresent[0] = 0;\n\t\t\tswitch1.value = \"ON1\";\n\t\t}\n\t\tdata = {\n\t\t\t\tSwitchNo:Switch1,\n\t\t\t\tstate:Present[0]\n\t\t\t};\n\t\t$.post(host,{switchstatus:Present[0],switch:\"Switch1\"},function(){\n\t});\n});\n\tswitch2.addEventListener(\"click\",function(){\n\t\t\tif(Present[1]==0){\n\t\t\tPresent[1]=1;\n\t\t\tswitch2.value = \"OFF1\";\n\t\t\t}else {\n\t\t\tPresent[1] = 0;\n\t\t\tswitch2.value = \"ON1\";\n\t\t}\n\t\tdata = {\"SwitchNo\":\"Switch2\",\n\t\t\t\t\"state\":Present[1] \n\t\t\t\t};\n\t\t\t$.post(host,{switchstatus:Present[1],switch:\"Switch2\"},function(){\n\n\t\t\t});\n});\n\tswitch3.addEventListener(\"click\",function(){\n\t\t\tif(Present[2]==0){\n\t\t\tPresent[2]=1;\n\t\t\tswitch3.value = \"OFF1\";\n\t\t}else {\n\t\t\tPresent[2] = 0;\n\t\t\tswitch3.value = \"ON1\";\n\t\t}\n\t\tdata = {\"SwitchNo\":\"Switch3\",\n\t\t\t\t\"state\":Present[2]\n\t\t\t};\n\t\t\t$.post(host,{switchstatus:Present[2],switch:\"Switch3\"},function(){\n\t\t});\n});\n</script>\n\n";
      server.send(200,"text/html",data);
      String switchno  = server.arg("switch");
      String  switchstate = server.arg("switchstatus");
      SendParent(switchno);
  }

void receivedCallback( uint32_t from, String &msg ) {
    Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
    CheckData(msg);
}
void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}
void changedConnectionCallback() {
    Serial.printf("Changed connections %s\n",mesh.subConnectionJson().c_str());
}
void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}
