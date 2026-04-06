#include <Servo.h>
#include <NewPing.h>

// Define pins for sensors and actuators
#define ULTRASONIC_TRIG_PIN 9
#define ULTRASONIC_ECHO_PIN 10
#define SERVO_PIN 11
#define LDR_PIN A0
#define SOIL_MOISTURE_PIN A1
#define PIR_PIN 12
#define WATER_PUMP_PIN 8
#define LIGHT_PIN 7

// Initialize objects
Servo myServo;
NewPing sonar(ULTRASONIC_TRIG_PIN, ULTRASONIC_ECHO_PIN, 200); // Maximum distance 200cm

// Define threshold values
const int DISTANCE_THRESHOLD = 10; // Distance threshold for opening the gate (in cm)
const int SOIL_MOISTURE_THRESHOLD = 500; // Soil moisture threshold for turning on the water pump
const int LIGHT_LEVEL_THRESHOLD = 500; // Light level threshold for turning on the light

void setup() {
  Serial.begin(9600);
  myServo.attach(SERVO_PIN);
  pinMode(LDR_PIN, INPUT);
  pinMode(SOIL_MOISTURE_PIN, INPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(WATER_PUMP_PIN, OUTPUT);
  pinMode(LIGHT_PIN, OUTPUT);
}

void loop() {
  while (true) {
    int distance = readUltrasonicSensor();
    int lightLevel = readLDRSensor();
    int soilMoisture = readSoilMoistureSensor();
    int motionDetected = readPIRSensor();

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.print(" cm, Light Level: ");
    Serial.print(lightLevel);
    Serial.print(", Soil Moisture: ");
    Serial.print(soilMoisture);
    Serial.print(", Motion Detected: ");
    Serial.println(motionDetected);

    controlGate(distance);
    controlWaterPump(soilMoisture);
    controlLight(lightLevel);

    delay(1000); // Wait for 1 second before the next loop
  }
}

int readUltrasonicSensor() {
  delay(50); // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  int distance = sonar.ping_cm();
  return distance;
}

int readLDRSensor() {
  int lightLevel = analogRead(LDR_PIN);
  return lightLevel;
}

int readSoilMoistureSensor() {
  int soilMoisture = analogRead(SOIL_MOISTURE_PIN);
  return soilMoisture;
}

int readPIRSensor() {
  int motionDetected = digitalRead(PIR_PIN);
  return motionDetected;
}

void controlGate(int distance) {
  if (distance < DISTANCE_THRESHOLD) {
    myServo.write(90); // Open the gate (move servo to 90 degrees)
    Serial.println("Gate opened");
  } else {
    myServo.write(0); // Close the gate (move servo to 0 degrees)
    Serial.println("Gate closed");
  }
}

void controlWaterPump(int soilMoisture) {
  if (soilMoisture < SOIL_MOISTURE_THRESHOLD) {
    digitalWrite(WATER_PUMP_PIN, HIGH); // Turn on the water pump
    Serial.println("Water pump turned on");
  } else {
    digitalWrite(WATER_PUMP_PIN, LOW); // Turn off the water pump
    Serial.println("Water pump turned off");
  }
}

void controlLight(int lightLevel) {
  if (lightLevel < LIGHT_LEVEL_THRESHOLD) {
    digitalWrite(LIGHT_PIN, HIGH); // Turn on the light
    Serial.println("Light turned on");
  } else {
    digitalWrite(LIGHT_PIN, LOW); // Turn off the light
    Serial.println("Light turned off");
  }
}
