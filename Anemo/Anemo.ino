  int poc;
  int luup = 0;
  int sensorValue;



void setup() {
  Serial.begin(9600);

}
void loop() {
 
     sensorValue = analogRead(A1);
    Serial.println(sensorValue);
     
   
}
