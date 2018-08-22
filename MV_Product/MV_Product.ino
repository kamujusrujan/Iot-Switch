      
      
      #include <ESP8266WiFi.h>
      #include <WiFiClient.h>
      #include <ESP8266WebServer.h>
      #include <FS.h>
      #include <ArduinoJson.h>
      
      ESP8266WebServer Server(80);
      DynamicJsonBuffer databuffer;
      File JSONdatafile;
      
      
      IPAddress arduino_ip ( 192,  168,   10,  20);
      IPAddress dns_ip     (  8,   8,   8,   8);
      IPAddress gateway_ip ( 192,  168,   10,   90);
      IPAddress subnet_mask(255, 255, 255,   0);
      
      
      String HomeSSID = "PHANI";
      String Homepsk = "wicked4me2c";
      
      String Jsondata;
      String JsonState;
      
      
      
      
      void setup(){
       pinMode(D0,OUTPUT); 
      pinMode(D1,OUTPUT); 
      
      Serial.begin(57600);
      // initialize the wifi ssid in hidden
      SetSSID();
      // Connect to the Local Network
      ConnectHome();
      // Wifi mesh networking for client boards
      MeshNetwork();
      // get the files in flash storage and store in JSON var
      GetFile();
      // initailize the Server
      ServerBegin();
      Serial.println(JsonState.length() );
      Ledstate(JsonState);
      }
      
      void loop(){   
      Server.handleClient(); 
      }
      
      
      
      
      
      
      
      
      
      void UpdateJSON(String replyH,String replyD){
        String DataStored;
     
        JSONdatafile.println(replyH);
        }
      void SetSSID(){
      WiFi.config(arduino_ip, gateway_ip, subnet_mask);
      WiFi.softAP("ESP","12345678", 1, true);
      }
      void GetFile(){
      SPIFFS.begin();
       JSONdatafile =  SPIFFS.open("/info.json", "r+");               // path is JSON file and mode is r+ for reading and writing
      if (!JSONdatafile) {
      Serial.println("file open failed");
      } 
      
      /*  JSONdata.readBytes(data,1000);
      
      //  data.as<char>;
      String mydata = "Thius is esp data";
      char mydata2[100];
      mydata.toCharArray(mydata2,1000);
      JsonObject& root = databuffer.parseObject(data + mydata); 
      Jsondata = String(data);
      Serial.println(data);
      */
      
      std::unique_ptr<char[]> buf {new char [1000]}; 
      JSONdatafile.readBytes(buf.get(),1000);
      Jsondata = buf.get();
      JsonObject& root = databuffer.parseObject(Jsondata); 
      JsonState = root["state"].as<String>();
      Serial.println(JsonState);
      
      }
      void MeshNetwork(){ 
      // only for the client boards for efficient connection 
      }
      void JSONLink(){
      
      Server.send(200,"text/html",Jsondata);
      String replyH = Server.arg("replyH");  
      String replyD = Server.arg("replyD");
      UpdateJSON(replyH,replyD);
      }
      void ServerBegin(){
      Server.on("/homepage",HomePage);
      Server.on("/data",JSONLink);
      Server.begin();
      
      }
      void HomePage(){
      String HomeHTML;
      Server.send(200,"text/html",HomeHTML);
      }
      
      void ConnectHome(){
      char ssid[20];
      char psd[20];
      HomeSSID.toCharArray(ssid,20);
      Homepsk.toCharArray(psd,20); 
      WiFi.begin(ssid,psd);
      
      while ( WiFi.status() != WL_CONNECTED ) {
      delay ( 500 );
      Serial.print ( "." );
      Serial.println("Please wait while connecting to your Home Network " + HomeSSID);
      delay(3000);
      }
      
      Serial.print(WiFi.localIP());
      }
      
      void Ledstate(String state){
      char statebuf[state.length() + 1];
      state.toCharArray(statebuf,state.length() + 1);
      for(int i = 0; i<= state.length()  ; i++){
      Serial.println(statebuf[i]); 
      
      
      if(statebuf[i]== 1 && i ==0){
      digitalWrite(D0,HIGH);
      
      }
      if(statebuf[i]==1  && i == 1){
      digitalWrite(D1,HIGH);
      
      }
      
      }
      
      }
      
      

