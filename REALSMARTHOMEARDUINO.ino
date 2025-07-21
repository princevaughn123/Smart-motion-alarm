
#include <LiquidCrystal.h>

// Pin definitions
#define PIR_PIN 8
#define BUZZER_PIN 9
#define LED_PIN 10
#define LDR_PIN A0

// LCD pin order: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

// Track previous states to prevent flicker
int lastMotionState = -1;
int lastLightState = -1;

void setup() {
  pinMode(PIR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  lcd.begin(16, 2);
  lcd.print("System Booting...");
  delay(2000);
  lcd.clear();

  Serial.begin(9600); // Optional: Monitor LDR readings
}

void loop() {
  int lightLevel = analogRead(LDR_PIN);
  int motion = digitalRead(PIR_PIN);

  Serial.print("Light level: ");
  Serial.println(lightLevel);

  // Adjust threshold based on testing
  bool isDark = lightLevel < 500;

  if (isDark) {
    if (motion == HIGH && lastMotionState != HIGH) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Motion Detected!");

      digitalWrite(LED_PIN, HIGH);

      // Buzzer pulses: bom bom bom... for ~3 sec
      for (int i = 0; i < 6; i++) {
        digitalWrite(BUZZER_PIN, HIGH);
        delay(250);
        digitalWrite(BUZZER_PIN, LOW);
        delay(250);
      }

      digitalWrite(LED_PIN, LOW);
    } 
    else if (motion == LOW && lastMotionState != LOW) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("System Armed");
      lcd.setCursor(0, 1);
      lcd.print("No Motion");
    }
    lastMotionState = motion;
    lastLightState = 1; // dark
  } 
  else {
    if (lastLightState != 0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Light Detected");
      lcd.setCursor(0, 1);
      lcd.print("System Off");

      digitalWrite(BUZZER_PIN, LOW);
      digitalWrite(LED_PIN, LOW);

      lastLightState = 0;
      lastMotionState = -1;
    }
  }

  delay(200); // Reduce flicker and debounce
}
