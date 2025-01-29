#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal.h>

// LCD setup
LiquidCrystal lcd(10, 11, 5, 4, 3, 2);

// LED Pins
const int ledPinYellow = 9;
const int ledPinRed = 12;
const int ledPinBlue = 13;

float temperature;
float humidity;

// DHT Sensor
#define DHTPIN 7
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Variables
float lastTemperature = -100.0, lastHumidity = -100.0;  // Store previous values
unsigned long lastReadTime = 0;
const unsigned long sensorInterval = 5000; // 5 seconds

void setup() {
  lcd.begin(16, 2);

  pinMode(ledPinYellow, OUTPUT);
  pinMode(ledPinBlue, OUTPUT);
  pinMode(ledPinRed, OUTPUT);

  Serial.begin(9600);
  Serial.println(F("DHT Test!"));

  dht.begin();
}

void loop() {
  unsigned long currentMillis = millis();

  // Read sensor every 5 seconds
  if (currentMillis - lastReadTime >= sensorInterval) {
    lastReadTime = currentMillis;

    // Read humidity & temperature
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    // Check if reads failed
    if (isnan(humidity) || isnan(temperature)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }

    // Update only if values changed
    if (temperature != lastTemperature || humidity != lastHumidity) {
      lastTemperature = temperature;
      lastHumidity = humidity;

      // Print to LCD
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Temp: ");
      lcd.print(temperature);
      lcd.print((char)223);
      lcd.print("C");

      lcd.setCursor(0, 1);
      lcd.print("Humidity: ");
      lcd.print(humidity);
      lcd.print("%");

      // Print to Serial Monitor
      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.println(" °C");

      Serial.print("Humidity: ");
      Serial.print(humidity);
      Serial.println(" %");

      // LED Logic
      digitalWrite(ledPinRed, LOW);
      digitalWrite(ledPinYellow, LOW);
      digitalWrite(ledPinBlue, LOW);

      if (temperature < 10) {
        digitalWrite(ledPinRed, HIGH);  // Cold
      } else if (temperature >= 10 && temperature < 20) {
        digitalWrite(ledPinYellow, HIGH);  // Moderate
      } else {
        digitalWrite(ledPinBlue, HIGH);  // Hot
      }
    }
  }
}
