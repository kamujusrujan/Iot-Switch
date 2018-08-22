// THE SERVER SIDE PROGRAMMING 
/* 
 * it has main server.com host to  controlling the device  
 * server.com/home for main 
 * uses html and jquery to send information in POST request only
 * responsive design is main proirity
 * Server should be in STATION AND AP MODE
 */ 

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

ESP8266WebServer Server(80);
//details of Home NetWork
 char homessid [20];
 char homepassword[20] ;

String HomeSSID = "PHANI";
String HomePassword = "wicked4me2c";

static String data;
static String statusprint;
static String dataneed;

void setup(){
  

  pinMode(D0,OUTPUT);

  Serial.begin(9600);
  
  HomeSSID.toCharArray(homessid,20);
  WiFi.hostname("autohomes");
  HomePassword.toCharArray(homepassword,20);
  WiFi.begin(homessid,homepassword);
  WiFi.mode(WIFI_AP_STA);
  
    while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
    Serial.println("Please wait while connecting to your Home Network " + HomeSSID);
    delay(3000);
}

    Serial.println("begin");
    Serial.print(WiFi.localIP());
    Server.on("/",HomePage);
    Server.begin();
    
   
  

}



void loop(){
  Server.handleClient();
  LEDchange();
  if(data=="1"){
      statusprint= "off";
    }else statusprint ="on";




  Serial.print(data + " and " + statusprint);
  delay(3500);
  
  }



  void HomePage(){
/* it is the main html source for our main page
 * it includes js,jquery, and html and css shit in it 
 * gotto check the flash storage issues in the page
 *page information and shit  
 * snprintf converts our entire html code to char in HomeHTML var
 * and this var is the main Page
 * set variables in the HTML  code
 */
 /*VARIABLES ARE 
  *  data,status.
  */
 
Statuschanger();

String HomeHTML = 
"<!DOCTYPE HTML> \n"
"\n"
"<html>\n"
"<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js\"></script>\n"
"<h1>THIS IS THE SWITCH PAGE\t</h1>\n"
"<br>\n"
"<p>\n"
"<input type =\"submit\" id=\"AddButton\" value=\"add switch\">\n"
"</p>\n"
"<div id =\"SwitchBlock\">\n"
"SWITCH ONE STATE: \n"
"<input type =\"submit\" id=\"ONSTATE\" value=\"ON\">\n"
"<input type =\"submit\" id=\"OFFSTATE\" value=\"OFF\">\n"
"<input type = \"text\" id=\"Senddata\">\n"
"<br>\n"
"</div>\n"
"<div id=\"stateprint\">\n"
"</div>\n"
"<script>\n"
"var ONbutton = document.getElementById(\"ONSTATE\");\n"
"var OFFbutton = document.getElementById(\"OFFSTATE\");\n"
"var addswitch = document.getElementById(\"AddButton\");\n"
"var dataneed ;\n"
"var state=null;\n"
"var statusprint = null;\n"                                                                  //here
"\n"
"ONbutton.addEventListener(\"click\",function(){\n"
"state = 1;\n"
"dataneed = $(\"#Senddata\").val();\n"
"\n"
"\n"
"$.post(\"/\",{status : state , datasend : dataneed },function(){\n"
"\t\t$(\"#stateprint\").html(\"the Switch is turned on );\n"                   //here
"\n"
"});\n"
"});\n"
"OFFbutton.addEventListener(\"click\",function(){\n"
"state = 0;\n"
"dataneed = $(\"#Senddata\").val();\n"
"$.post(\"/\",{status : state, datasend : dataneed},function(){\n"
"$(\"#stateprint\").html(\" the Switch is turned  on           \");\n"                       //here
"\n"
"});\n"
"});\n"
"window.setInterval(function(){\n"
"DynamicChange();\t\n"
"}, 2000);\n"
"function DynamicChange(){\n"
"$(\"#stateprint\").html(\" still the switch is turned \"+ \" "   + Statuschanger()+                    " \" ) ;\n"              //here
"console.log(\"Updated\");\n"
"}\n"
"var i = 0;\n"
"function AddSwitch(number){\n"
"var text = \" SWITCH ONE STATE: <input type =\\\"submit\\\" \\\"id = switch \" + i + \" \\\"  \\\" value=\\\"ON\\\"> <input type =\\\"submit\\\"  value=\\\"OFF\\\"> <br> \";\n"
"for( var f=0;f<number;f++){\n"
"$(\"#SwitchBlock\").append (text);\n"
"i += 1;\n"
"console.log(\"switch\" + i);\n"
"}}\n"
"addswitch.addEventListener(\"click\",function(){\n"
"AddSwitch(1);\n"
"});\n"
"</script>\n"
"</html>\n"
"\n"
"\n"
"";

Server.send(200,"text/html",HomeHTML);
    data = Server.arg("status");
    dataneed = Server.arg("datasend");
    Serial.println(dataneed);
    Serial.println(data);
    }


  String Statuschanger(){
      if(data =="1"){
       return  statusprint ="On";
        }else return statusprint ="Off";
    }

void LEDchange(){
  if(data== "1"){
    digitalWrite(D0,HIGH);
    }else digitalWrite(D0,LOW);
  
  }


