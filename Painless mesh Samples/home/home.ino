#include "painlessMesh.h"
#include<ESP8266WebServer.h>
#include<ESP8266WiFi.h>


const char* meshSSID = "MeshTest";
const char* meshPSD = "password";
int port = 1234;
painlessMesh mesh;
uint32_t parentnode;
int hidden = 0; // 1 for hidden

void setup() {
  parentnode = mesh.getNodeId(); 
  // put your setup code here, to run once:
      Serial.begin(9600);
     
      mesh.setDebugMsgTypes( ERROR | STARTUP );
      mesh.init(meshSSID, meshPSD,port,STA_AP,AUTH_WPA2_PSK,1,PHY_MODE_11G,82,hidden,4);
      mesh.onReceive(&receivedCallback);
      mesh.onNewConnection(&newConnectionCallback);
      mesh.onChangedConnections(&changedConnectionCallback);
      mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
      Serial.println( mesh.getNodeId() );
     WiFi.softAP("parent","test12345",1,0);
      
      
}

void loop() {
  // put your main code here, to run repeatedly:
  mesh.update();  
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
