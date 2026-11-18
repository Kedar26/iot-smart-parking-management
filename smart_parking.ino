#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>

const int irPins[4] = {13, 14, 27, 26};
const int servoPin = 18;

const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo gateServo;

void setup() {
  Serial.begin(115200);

  for (int i = 0; i < 4; i++) {
    pinMode(irPins[i], INPUT);
  }

  lcd.init();
  lcd.backlight();

  gateServo.attach(servoPin);
  gateServo.write(0);

  WiFi.begin(ssid, password);
}

void loop() {
  int occupied = 0;

  for (int i = 0; i < 4; i++) {
    // Many IR modules output LOW when an object is detected.
    if (digitalRead(irPins[i]) == LOW) {
      occupied++;
    }
  }

  int available = 4 - occupied;

  lcd.setCursor(0, 0);
  lcd.print("Slots Available");
  lcd.setCursor(0, 1);
  lcd.print(available);
  lcd.print(" of 4       ");

  Serial.print("Occupied: ");
  Serial.print(occupied);
  Serial.print("  Available: ");
  Serial.println(available);

  if (available > 0) {
    gateServo.write(90);   // Demonstration: open gate
  } else {
    gateServo.write(0);    // Close gate when full
  }

  delay(1000);
}
