  int poc;
  int luup = 0;
  int sensorValue;



void setup() {
  Serial.begin(9600);

}
void loop() {
 
 luup = luup + 1;
 sensorValue = sensorValue + analogRead(A1);
 if (luup > 10){
    sensorValue = sensorValue / luup;
    Serial.println(sensorValue);
     
    }
   delay(100);
   
}
