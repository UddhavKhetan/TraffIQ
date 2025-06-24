int switchState = 0; 
int temt6000Pin = A0; 
float light; 
int light_value; 
 
//setup 
// the setup routine runs once when you press reset: 
void setup() { 
  // initialize the digital pin as an output. 
  // pinMode(3, OUTPUT); 
  Serial.begin(112500); 
  pinMode(temt6000Pin, INPUT); 
  pinMode(3, OUTPUT); 
  pinMode(2, INPUT); 
} 
 
// the loop routine runs over and over again forever: 
void loop() { 
  switchState = digitalRead(2);  //HIGH or LOW based on whether the button is on or off 
  Serial.print(switchState); 
  if (switchState == 0) { 
    //button is not pressed 
    /*digitalWrite(3, HIGH);  //turn green LED on 
    digitalWrite(4, LOW);  //red LED off 
    digitalWrite(3, LOW);  //red LED off*/ 
    light_value=analogRead(temt6000Pin); 
    light = (float)light_value * 0.0976; 
    delay(100); 
    if (light > 30) { 
      Serial.println(light); 
      digitalWrite(3, HIGH); 
      delay(250); 
    } 
    else { 
      digitalWrite(3, LOW); 
    } 
  } 
  else { 
    //button is pressed 
    digitalWrite(3, HIGH); 
    Serial.println("lig"); 
    delay (250); //wait a quarter second 
  } 
     
} //go back to the beginning of the loop
