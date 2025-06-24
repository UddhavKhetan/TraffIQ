// // const int temt6000Pin = A0; // Sensor connected to analog pin A0
// // const float Vcc = 5.0; // Supply voltage (adjust if using 3.3V)
// // const float resistorValue = 10000; // Resistor value in ohms
// // const float k = 0.03162; // Constant from datasheet
// // const float m = 1.5; // Exponent from datasheet
// // float light;
// // int light_value;
// int hallSensor = A1;

// //setup

// void setup() {
//   Serial.begin(9600);
//   // pinMode(temt6000Pin, INPUT);
//   // pinMode(3, OUTPUT);
//   pinMode(hallSensor, INPUT);
//   pinMode(5, OUTPUT);
// }

// // the loop routine runs over and over again forever:
// void loop() {
//   // int rawValue = analogRead(temt6000Pin);
//   // float voltage = rawValue * (Vcc / 1023.0);
//   // float currentMicroA = (voltage / resistorValue) * 1E6; // Convert current to microamperes
//   // float light = pow(currentMicroA / k, 1 / m); // Calculate illuminance in lux
  
  
//   // int hallState=LOW;
//   //hallState=digitalRead(hallSensor);
//   // if(hallState==LOW) {
//   //   digitalWrite(5, LOW);
//   //   Serial.println("Hall low");
//   // }
//   // else {
//   //   digitalWrite(5,HIGH);
//   //   Serial.println("Hall high");
//   // }
//   int hallState=analogRead(hallSensor);
//   Serial.println(hallState);
//   if(hallState>990) {
//     digitalWrite(5, LOW);
//     Serial.println("Hall low");
//   }
//   else {
//     digitalWrite(5,HIGH);
//     Serial.println("Hall high");
//   }

//   delay(600);

//   // Serial.println(light);
//   // delay(500);
//   // if (light > 350) {
//   //   Serial.println(light);
//   //   digitalWrite(3, HIGH);
//   //   delay(250);
//   // }
//   // else {
//   //   digitalWrite(3, LOW);
//   // }
    
// }	//go back to the beginning of the loop





// Define pins
const int trigPin = 9; // Trigger pin of the ultrasonic sensor
const int echoPin = 10; // Echo pin of the ultrasonic sensor
const int ledPin = 5; // LED connected to pin 13

// Define variables
long duration;
int distance;
int distance1;

void setup() {
  // Initialize pins
  pinMode(trigPin, OUTPUT); // Set trigPin as output
  pinMode(echoPin, INPUT);  // Set echoPin as input
  pinMode(ledPin, OUTPUT);  // Set ledPin as output

  // Start serial communication for debugging
  Serial.begin(9600);
}

void loop() {
  // Clear the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Send a HIGH pulse to trigPin for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure the duration of the pulse on echoPin
  duration = pulseIn(echoPin, HIGH);

  // Calculate distance (in cm)
  distance = duration * 0.034 / 2;

  // Print the distance to Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Control LED based on distance threshold (e.g., <30 cm)
  if (distance <= 10) {
    int status = 1;
    for(int i=0; i<32000; i++) {
      for(int j=0; j<32000; j++) {
        for(int k=0; k<32000; k++) {
          for(int l=0; l<32000; l++) {
            if(distance>10) {
              status=0;
            }
          }
        }
      }
    }
    if(status==1) {
      delay(3000);
      // Clear the trigPin
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);

        // Send a HIGH pulse to trigPin for 10 microseconds
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);

        // Measure the duration of the pulse on echoPin
        duration = pulseIn(echoPin, HIGH);

        // Calculate distance (in cm)
        distance1 = duration * 0.034 / 2;
      if(distance == distance1 || distance==distance1-1 || distance==distance1+1) {
        digitalWrite(ledPin, HIGH); // Turn LED on
      }
    }
  } else {
    digitalWrite(ledPin, LOW); // Turn LED off
  }

  delay(500); // Small delay for stability
}

