    #include<ESP8266WiFi.h>
    #include <ESP8266WiFiMulti.h>
    
    
    ESP8266WiFiMulti wifiMulti; 
    
    String ssid[]={"nigga","higa","trigga"};
    String psd[]={"nigga","higga","trigga"};
    
    IPAddress local_IP(192,168,4,22);
    IPAddress gateway(192,168,4,9);
    IPAddress subnet(255,255,255,0);
    
    void setup(){
    Serial.begin(115200);
    SoftAp();
    MeshNet();
 }
    
    void loop(){
      if(wifiMulti.run() != WL_CONNECTED) {
        Serial.println("WiFi not connected!");
        delay(1000);
    }
    }
    
    void MeshNet( ){
    for(int n = 0 ;n<=sizeof(ssid); n++){
    char ssidname[ssid[n].length()] ;
    char password[psd[n].length()];
    psd[n].toCharArray( password,psd[n].length()   );
    ssid[n].toCharArray( ssidname,ssid[n].length()  );
    wifiMulti.addAP(ssidname,password);
     if(wifiMulti.run() == WL_CONNECTED) {
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
    }
   }
}
    
    void SoftAp(){
    WiFi.softAPConfig(local_IP, gateway, subnet);
    WiFi.softAP("trigga1", "trigga", 1, false);
    }
    

