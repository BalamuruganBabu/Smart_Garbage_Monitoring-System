#include <Servo.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Pin Definitions
#define trigPin D5
#define echoPin D6
#define irSensorPin D1
#define flameSensorPin D2
#define metalSensorPin D4
#define servoPin D3

Servo lidServo;

char auth[] = "Your_Blynk_Auth_Token";
char ssid[] = "YourWiFi";
char pass[] = "YourPassword";

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, pass);
  Blynk.begin(auth, ssid, pass);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(irSensorPin, INPUT);
  pinMode(flameSensorPin, INPUT);
  pinMode(metalSensorPin, INPUT);

  lidServo.attach(servoPin);
  lidServo.write(0);
}

void loop() {
  Blynk.run();

  long duration;
  int distance;

  digitalWrite(trigPin, LOW); delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Distance: "); Serial.println(distance);
  if (distance < 10) {
    Blynk.logEvent("bin_full", "Dustbin is full!");
  }

  if (digitalRead(flameSensorPin) == LOW) {
    Blynk.logEvent("fire_alert", "🔥 Fire Detected!");
  }

  if (digitalRead(irSensorPin) == HIGH && digitalRead(metalSensorPin) == LOW) {
    lidServo.write(90);
    delay(2000);
    lidServo.write(0);
  }

  delay(1000);
}
