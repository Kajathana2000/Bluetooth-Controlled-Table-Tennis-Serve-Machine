#include <Servo.h>

// Define pins for motors and servo
const int motorPin1 = 3;
const int motorPin2 = 4;
const int motorSpeedPin = 5;
const int servoPin = 6;

const int motorPin3 = 9;
const int motorPin4 = 10;
const int motorSpeedPin2 = 11;

const int irSensorPin = A0; // Analog pin connected to IR sensor output
const int threshold = 500;  // Adjust this threshold based on your sensor readings


int speed = 100;
int speedSq =100;
Servo myServo;

void setup() {
  // Initialize motor pins
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorSpeedPin, OUTPUT);
  pinMode(motorPin3, OUTPUT); 
  pinMode(motorPin4, OUTPUT);

  // Initialize servo
  myServo.attach(servoPin);
  myServo.write(90); // Initialize servo to the neutral position
  randomSeed(analogRead(0));

  // Initialize serial communication
  Serial.begin(9600);
}


void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');

    if (command == "START") {
      // Start motors speed dc
      digitalWrite(motorPin1, HIGH);
      digitalWrite(motorPin2, LOW);
      analogWrite(motorSpeedPin, speed);

      //Start motors sequence dc
      digitalWrite(motorPin3, HIGH);
      digitalWrite(motorPin4, LOW);
      analogWrite(motorSpeedPin2, speedSq);

      //SERVO SET 0
      moveServoToAngle(90);
      delay(1000);

    } else if (command == "STOP") {
      // Stop both motors
      digitalWrite(motorPin1, LOW);
      digitalWrite(motorPin2, LOW);

      digitalWrite(motorPin3, LOW);
      digitalWrite(motorPin4, LOW);

      moveServoToAngle(90);
      delay(1000);

    } else if (command == "INCSP") {
      // Increase speed
      speed = min(255, speed + 20);
      analogWrite(motorSpeedPin, speed);
    } else if (command == "DECSP") {
      // Decrease speed
      speed = max(0, speed - 20);
      analogWrite(motorSpeedPin, speed);
    } else if(command == "RSP"){
      speed = 100;
      analogWrite(motorSpeedPin, speed);

      //*****************
    }else if(command == "INCB"){
      speedSq = min(255, speedSq + 20);
      analogWrite(motorSpeedPin2, speedSq);
    }else if(command == "DECB"){
      speedSq = max(0, speedSq - 20);
      analogWrite(motorSpeedPin2, speedSq);
    }else if(command == "RB"){
      speedSq = 100;
      analogWrite(motorSpeedPin2, speedSq);
    //*********

    
    }else if (command == "FHAND") {
      // Set servo to single position F
      moveServoToAngle(0);
      delay(500);
      moveServoToAngle(65);
    } else if (command == "BHAND") {
      // Set servo to single position F
      moveServoToAngle(0);
      delay(500);
      moveServoToAngle(110);
    }else if (command == "MID") {
      // Set servo to single position F
      moveServoToAngle(0);
      delay(500);
      moveServoToAngle(90);

    } else if (command == "RANDOM") {
      randomMode(); 
    }
  }
}

void moveServoToAngle(int angle) {
  Serial.print("Moving servo to ");
  Serial.print(angle);
  Serial.println(" degrees");
  
  myServo.write(angle);  // set the servo position
}


void randomMode() {
  bool randomMode = true;  // Variable to control the random mode

  while (randomMode) {
    int sensorValue = analogRead(irSensorPin); // Read analog input from IR sensor
    Serial.println(sensorValue);               // Print sensor value for debugging

    // Check if ball is detected
    if (sensorValue < threshold) {
      // Move servo to a random angle between 65 and 115
      int randomAngle = random(65, 115);
      moveServoToAngle(randomAngle);
      Serial.print("Moved to angle: ");
      Serial.println(randomAngle);
      
      // Wait until ball is no longer detected
      while (analogRead(irSensorPin) < threshold) {
        delay(10); // Short delay to avoid rapid rechecking
      }
    }

    // Check for stop command
    if (Serial.available() > 0) {
      String command = Serial.readStringUntil('\n'); 
      if (command == "STOP") {
        randomMode = false;  // Exit random mode when 'STOP' is received
      }
    }

    delay(10); // Delay for stability
  }
}
