#define DEBUG true
String mySSID = "robotika";       // WiFi SSID
String myPWD = "nasratpanove"; // WiFi Password
String myAPI = "AZ9NLJGIQAE8A3WJ";   // API Key
String myHOST = "api.thingspeak.com";
String myPORT = "80";
String myFIELD = "field1";
int sendVal;


void setup()
{
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
}

  void loop()
  {
    /* Here, I'm using the function random(range) to send a random value to the 
     ThingSpeak API. You can change this value to any sensor data
     so that the API will show the sensor data  
    */
    
    sendVal = 15; // Send a random number between 1 and 1000
    //http://api.thingspeak.com/update?api_key=XVKHSUERMX3UPQD9&field1=73
    String sendData = "GET /update?api_key="+ myAPI +"&"+ myFIELD +"="+String(sendVal);
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
