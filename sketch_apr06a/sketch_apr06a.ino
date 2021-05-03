//DHT
#include "DHT.h"
#define pinDHT 6

//BMP
#include <Wire.h>
#include <Adafruit_BMP085.h>


#define typDHT22 DHT22  

Adafruit_BMP085 bmp180;


DHT mojeDHT(pinDHT, typDHT22);

int korekce = 58.6;




#define DEBUG true
String mySSID = "robotika";       // WiFi SSID
String myPWD = "nasratpanove"; // WiFi Password
String myAPI = "AZ9NLJGIQAE8A3WJ";   // API Key
String myHOST = "184.106.153.149";
String myPORT = "80";
String myFIELD = "field1";
String myFIELD2 = "field2"; 
String odesilanaHodnota = "1";
float tep;
float tlakPrepocet;

void setup() {

 Serial.begin(9600); 
 mojeDHT.begin();
 bmp180.begin();
  
  
  Serial.begin(9600);
  Serial1.begin(115200);

  espData("AT+RST", 1000, DEBUG);                      //Reset the ESP8266 module
  espData("AT+CWMODE=1", 1000, DEBUG);                 //Set the ESP mode as station mode
  espData("AT+CWJAP=\""+ mySSID +"\",\""+ myPWD +"\"", 1000, DEBUG);   //Connect to WiFi network
  espData("AT+CIFSR", 1000, DEBUG);
  //delay(1000);
}

void loop() {
  
  tep = mojeDHT.readTemperature();
  float vlh = mojeDHT.readHumidity();
  
  if (isnan(tep) || isnan(vlh)) {

     Serial.println("------------------------------");
    Serial.println("Chyba při čtení z DHT senzoru!");
     Serial.println("------------------------------");
  } else {
    
    Serial.println("------------------------------");
    Serial.print("Teplota: "); 
    Serial.print(tep);
    Serial.print(" stupnu Celsia, ");
    Serial.print("vlhkost: "); 
    Serial.print(vlh);
    Serial.println("  %");
    Serial.println("------------------------------");
  }
  
 Serial.print("Barometricky tlak: ");
 Serial.print((bmp180.readPressure()/100 + korekce ));
 Serial.println(" hPa");


 Serial.println("------------------------------");

 delay(5000);

  tlakPrepocet = bmp180.readPressure()/100 + korekce;
  
  
 

  esp8266();

    
  }



void esp8266(){
  String sendData = "GET /update?api_key="+ myAPI +"&"+ myFIELD +"="+String(tep)+"&"+ myFIELD2 +"="+String(tlakPrepocet);
  espData("AT+CIPMUX=0", 1000, DEBUG); 
  espData("AT+CIPSTART=\"TCP\",\""+ myHOST +"\","+ myPORT, 1000, DEBUG);
  //espData("AT+CIPSEND=0," +String(sendData.length()),1000,DEBUG); 
  Serial1.println("AT+CIPSEND=102");
  Serial1.find(">"); 
  Serial1.println(sendData);
  espData("AT+CIPCLOSE=0",1000,DEBUG);
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
