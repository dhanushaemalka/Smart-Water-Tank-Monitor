#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL6oCHooQrk"
#define BLYNK_TEMPLATE_NAME "Water level monitor"
#define BLYNK_AUTH_TOKEN "qg0Xrr-hhsu7jUozOFAWu6t_J2KpUbis"

#include <Wire.h>
#include <LiquidCrystal_I2C.h>  // Include the LiquidCrystal I2C library
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "Pixel_5200";
char pass[] = "11112222";

// Pin Definitions
int ledPin = 2;  // GPIO 2 for LED
const int trigPin = 5;  // Ultrasonic TRIG pin
const int echoPin = 18;  // Ultrasonic ECHO pin
const int relayPin = 32;  // GPIO pin for relay
const int buzzerPin = 14;  // GPIO pin for the buzzer
const int PHSensorPin = 34;

// LCD Configuration
int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  // LCD at address 0x27

// Variables
long duration;
int distance;
bool buzzerActivated = false;  // Flag to track if the buzzer has been activated
int waterLevelPercentage = 0;  // Variable to store water level percentage

// Constants
const int maxDistance = 30;  // Maximum distance (e.g., when the tank is empty)
const int minDistance = 10;   // Minimum distance (e.g., when the tank is full)

//PH Sensor
float calibration_value = 22.15;
unsigned long int avgValue;
int buf[10], temp;

void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  startUpText();
  
  delay(2000);

  // Pin Modes
  pinMode(relayPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(PHSensorPin, INPUT);

  analogReadResolution(10);

  digitalWrite(relayPin, LOW);  // Start with relay off
}
void loop() {
  Blynk.run();

  delay(1000);
  
  // Ultrasonic Sensor: Measure Distance
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  for(int i = 0; i < 10; i++){
    buf[i] = analogRead(PHSensorPin);
    delay(10);
  }

  for(int i = 0; i < 9; i++){
    for(int j = i; j < 10; j++){
      if(buf[i] > buf[j]){
        temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
      }
    }
  }

  avgValue = 0;
  for(int i = 2; i < 8; i++){
    avgValue += buf[i];
  }

  float voltage = (float)avgValue * 3.3/1024/6;
  Serial.print("Voltage: ");
  Serial.println(voltage);

  float phValue = -5.70 * voltage + calibration_value;

  Serial.print("pH Value: ");
  Serial.println(phValue);
  Blynk.virtualWrite(V2, phValue);
  delay(1000);

  // Calculate water level percentage
  if (distance > maxDistance) {
    distance = maxDistance;  // Limit distance to maxDistance
  }
  if (distance < minDistance) {
    distance = minDistance;  // Limit distance to minDistance
  }

  // Calculate the water level percentage (0% = minDistance, 100% = maxDistance)
  waterLevelPercentage = map(distance, minDistance, maxDistance, 100, 0);

  // Send water level to Blynk virtual pin V1
  Blynk.virtualWrite(V1, waterLevelPercentage);
  Serial.print("Water Level: ");
  Serial.print(waterLevelPercentage);
  Serial.println("%");

  // Control Relay and Display on LCD
  if (distance >= 20){
    digitalWrite(relayPin, HIGH);  // Relay ON
    digitalWrite(ledPin, HIGH);
    
    // Only update pump status if it changes
    lcd.setCursor(0, 0);
    lcd.print("Pump Status: ON   ");  // Extra spaces to clear previous text
    displayWaterLevel(waterLevelPercentage);
    //Blynk.logEvent("water_notifi", "Warning!! Water level is too low -> water pump ON");
    delay(2000);
    
    // Send pump status to Blynk (V0)
    Blynk.virtualWrite(V0, "ON");
    buzzerActivated = false;  // Reset the flag when distance is >= 30 cm
  }

  if(distance <= 7 && !buzzerActivated){
    digitalWrite(relayPin, LOW);  // Relay OFF
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, HIGH);
    delay(1000);
    digitalWrite(buzzerPin, LOW);
    buzzerActivated = true;  // Set flag so the buzzer doesn't repeat
    
    // Only update pump status if it changes
    lcd.setCursor(0, 0);
    lcd.print("Pump Status: OFF  ");  // Extra spaces to clear previous text
    displayWaterLevel(waterLevelPercentage);
    delay(2000);
    
    // Send pump status to Blynk (V0)
    Blynk.virtualWrite(V0, "OFF");
  }

  // Always update the water level at the end
  displayWaterLevel(waterLevelPercentage);
}


void displayWaterLevel(int waterLevelPercentage){
  lcd.setCursor(0, 1);
  lcd.print("Water Lvl  : ");
  lcd.print(waterLevelPercentage);
  lcd.print("%");
}

void startUpText(){
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Initiating");
  for(int i=0;i<3;i++){
    lcd.print(".");
    delay(500);
  }
}


