#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "PHANIS";
const char* password = "wicked4me2c";

// ESP8266WebServer server(80);
WiFiServer server(80);

void setup(){
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("$$$$");
     }
  
  Serial.println("WiFi connected");
  
  //server.on ( "/sw1", Switch1Page );
  server.begin();
  Serial.println("server begin");
  WiFi.hostname("autohomes");
  server.print();
 // server.send( 200, "text/html ", "First LOL ");
  

  
  }

  void Switch1Page(){
      // html page for the n\button and shit
    }







  

/*
  void loop(){
  WiFiClient client () ;
   
    //checq if clinet is avail
    if (!client) {
    return;
  }
  
  // clint send data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
//put in req of client 
  String req = client.readStringUntil('\n');


  //response
  
   String response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n Switch is   turned on  ";
  client.print(response);
  // send response to thger client usin g client,pptiny
  


void loop(){
  server.handleClient();
  if(!client.available()){
    Serial.print("Sgyd");
    }
  }
  
  */


  void HomePage(){
        
    }
/*
    void loop(){
      
      server.handleClient();
       // send info to the client
      
      }
    */


    
    
  
  
  
