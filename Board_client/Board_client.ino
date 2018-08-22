// THIS IS THE CLIENT SIDE PROGRAM 
/* this is the program for the esp in the switch board
 *  it reads the information in the host site and gets the particular information we need 
 *  it reads the data and converts it in  the binary digits for turning on and off
 *  consists of wifimulti library 
 *  it tries to connect to the parent device if the attempt is unsuccessfull then it tries to connect to the near ESP modules which inturn is connected to Parent
 */



#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>


#include <WiFiClient.h>



String ParentSSID = "PHANI";
String Parentpass = "wicked4me2c";
ESP8266WiFiMulti wifiMulti;
char pssid[20];
char ppass[20];

char  host[30] = "192.168.0.4";
String hosts = "192.168.0.4";

void setup(){
  Serial.begin(57600);
  ParentSSID.toCharArray(pssid,20);
  Parentpass.toCharArray(ppass,20);
  WiFi.begin(pssid,ppass);

  /* add some more IPadress and esp details for seamless connectivity
   *  wifiMulti.addAP();
   */
   Serial.println("");
  while(WiFi.status() != WL_CONNECTED){
    Serial.println(". . . ");
    delay(3000);
    }
  Serial.println("Connected");
  Serial.print(WiFi.SSID());

   Serial.println("Connecting to host " );
   Serial.print(host);
  }

  

 
void loop(){
 
  
 
  WiFiClient client;
  
  
  if (!client.connect(host, 80)) {
    Serial.println("connection failed");
    
  }

  // put request to the client to get information about the website

  client.print(String("GET ") + "/" + " HTTP/1.1\r\n" +
                                "Host: " + host + "\r\n" + 
                                "Connection: close\r\n\r\n");
  delay(5000);                              
 
    
  while(client.available()){
    Serial.println("Geting information");
  String line = client.readStringUntil('\r');
  
  Serial.print(line);
  } 

/* USE THE REPSONSE TO FIND THE CONFIGURATION OF THE PRESENT BOARD DATA
 *  AND THEN USING THE PINS PRESENT CONTROL THE SIGNALS
 */
    
    
  }






  
