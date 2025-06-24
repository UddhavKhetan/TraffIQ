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
