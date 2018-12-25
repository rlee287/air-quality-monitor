/*
 * This project includes code from: 
 * - AdaFruit DHT library (https://github.com/adafruit/DHT-sensor-library)
 * - LiquidCrystal-I2C (https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library)
 */

#include "DHT.h"
#include <LiquidCrystal_I2C.h>

#define DHTPIN 13     // DHT digital pin

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);

// Initialize LCD output
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
  Serial.begin(9600);
  Serial.println("DHTxx test!");
  lcd.begin();
  lcd.setCursor(0,2);
  lcd.print("Initializing");
  dht.begin();
}

void loop() {
  lcd.setCursor(0,2);
  //         12345678901234567890
  lcd.print("Refreshing...  ");
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  //                 12345678901234567890 °
  String tempStr  = "Temperature *C: ";
  String humidStr = "Rel. Humidity:  ";

  // Check if reads failed
  lcd.setCursor(0,2);
  if (isnan(h)) {
    Serial.println("Failed to read humidity from DHT sensor!");
    humidStr+="ERR";
    //         12345678901234567890
    lcd.print("Refreshing failed");
    // Clear humidity number
    lcd.setCursor(16,1);
    lcd.print("    ");
  } else if (isnan(t)) {
    Serial.println("Failed to read temperature from DHT sensor!");
    tempStr+="ERR";
    //         12345678901234567890
    lcd.print("Refreshing failed");
    // Clear temperature number
    lcd.setCursor(16,0);
    lcd.print("    ");
  } else {
    tempStr+=String(t,0);
    humidStr+=String(h,0);
    //         12345678901234567890
    lcd.print("                    ");
  }
  lcd.setCursor(0,0);
  lcd.print(tempStr);
  lcd.setCursor(0,1);
  lcd.print(humidStr);
  
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C\n");
  // Wait a few seconds between measurements.
  delay(2000);
}
