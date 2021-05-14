  long x;
  long x2;
  long delta;
  long poc;
  long luup = 0;
  int sensorValue;



void setup() {
  Serial.begin(9600);

}
void loop() {
  x = millis;
  
  luup = luup + 1;
  sensorValue = sensorValue + analogRead(A1);
  if (poc > 1000){
   
    sensorValue = sensorValue / luup;
     Serial.println(sensorValue);
     
    
    
    poc = 0;
    }

    x2 = millis;
   delta = x2 - x;
   
   poc = poc + delta;
   
}
