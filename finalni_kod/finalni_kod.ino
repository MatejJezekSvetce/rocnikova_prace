//ESP
#define DEBUG true
String mySSID = "ToJeJedno";       // WiFi SSID
String myPWD = "20Sychr12"; // WiFi Password
String myAPI = "AZ9NLJGIQAE8A3WJ";   // API Key
String myHOST = "api.thingspeak.com";
String myPORT = "80";
String myFIELD = "field1"; 
String myFIELD2 = "field2";
String myFIELD3 = "field3";  
int sendVal;
int sendVal2;
int sendVal3;
//------------------------------------------------------------------
//DHT
#include "DHT.h"
#define pinDHT 6
#define typDHT22 DHT22 
DHT mojeDHT(pinDHT, typDHT22);
//------------------------------------------------------------------
//BMP
#include <Wire.h>
#include <Adafruit_BMP085.h>
Adafruit_BMP085 bmp180;
int korekce = 58.6;

//------------------------------------------------------------------

void setup()
{
  //ESP
  Serial.begin(9600);
  Serial1.begin(115200);
  
  //espData("AT+RST", 1000, DEBUG);                      //Reset the ESP8266 module
  espData("AT+CWMODE=1", 1000, DEBUG);                 //Set the ESP mode as station mode
  espData("AT+CWJAP=\""+ mySSID +"\",\""+ myPWD +"\"", 1000, DEBUG);   //Connect to WiFi network
  /*while(!esp.find("OK")) 
  {          
      //Wait for connection
  }*/
  delay(1000);
  espData("AT+CIFSR", 1000, DEBUG);
//----------------------------------------------------------------

 Serial.begin(9600); 
 mojeDHT.begin();
 bmp180.begin();


  
}

  void loop()
  {

//DHT+BMP
  float tep = mojeDHT.readTemperature();
  float vlh = mojeDHT.readHumidity();
  
  if (isnan(tep) || isnan(vlh)) {

     Serial.println("------------------------------");
    Serial.println("Chyba při čtení z DHT senzoru!");
     Serial.println("------------------------------");
  } else {
    
    Serial.println("------------------------------");
    Serial.print("Teplota: "); 
    Serial.print(tep);

    Serial.print("vlhkost: "); 
    Serial.print(vlh);

  }
  
 Serial.print("Barometricky tlak: ");
 float tlk = bmp180.readPressure()/100 + korekce;
 Serial.print(tlk);


 delay(5000);

 //-----------------------------------------------------------------------

  //ESP    
    sendVal = tep; // Send a random number between 1 and 1000
    sendVal2 = tlk;
    sendVal3 = vlh;
    //http://api.thingspeak.com/update?api_key=XVKHSUERMX3UPQD9&field1=73
    String sendData = "GET /update?api_key="+ myAPI +"&"+ myFIELD +"="+String(sendVal)+"&"+ myFIELD2 +"="+String(sendVal2)+"&"+ myFIELD3 +"="+String(sendVal3);
    espData("AT+CIPMUX=1", 1000, DEBUG);       //Allow multiple connections
    espData("AT+CIPSTART=0,\"TCP\",\""+ myHOST +"\","+ myPORT, 1000, DEBUG);
    espData("AT+CIPSEND=0," +String(sendData.length()+2),1000,DEBUG);  
    Serial1.find(">"); 
    Serial1.println(sendData);
    Serial.print("Value to be sent: ");
    Serial.println(sendData);
    Serial.print("size to be send: ");
    Serial.println(sendData.length());
     
    espData("AT+CIPCLOSE=0",1000,DEBUG);
    delay(6000);
  }

  String espData(String command, const int timeout, boolean debug)
{
  Serial.print("AT Command ==> ");
  Serial.print(command);
  Serial.println("     ");
  
  String response = "";
  Serial1.println(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (Serial1.available())
    {
      char c = Serial1.read();
      response += c;
    }
  }

    Serial.print(response);

  return response;
}
