/*
 * This project includes code from: 
 * - AdaFruit DHT library (https://github.com/adafruit/DHT-sensor-library)
 * - LiquidCrystal-I2C (https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library)
 * - DS3231 RTC library (https://github.com/NorthernWidget/DS3231)
 */

#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <DS3231.h>

#define DHTPIN 13     // DHT digital pin

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Loop delay in ms: adjust to be the max wait time of connected sensors
#define LOOP_DELAY 2000

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

uint8_t deg[8]={0xe,0xa,0xe,0x0,0x0,0x0,0x0};

String getTimeStr() {
  DateTime timeobj = RTClib::now();
  String timestr = "";
  // All these strings are supposed to be length 2 except for year
  timestr+=leftPadInt(timeobj.year(),4);
  timestr+="-";
  timestr+=leftPadInt(timeobj.month(),2);
  timestr+="-";
  timestr+=leftPadInt(timeobj.day(),2);
  timestr+="T";
  timestr+=leftPadInt(timeobj.hour(),2);
  timestr+=":";
  timestr+=leftPadInt(timeobj.minute(),2);
  timestr+=":";
  timestr+=leftPadInt(timeobj.second(),2);
  timestr+="Z";
  return timestr;
}

// Only properly handles positives but that is fine here
String leftPadInt(int number, int width) {
  String retStr = String(number);
  while (width>retStr.length()) {
    retStr="0"+retStr;
  }
  return retStr;
}
void setup() {
  Serial.begin(9600);
  Serial.println("AirMonitor initializing");
  lcd.begin();
  lcd.setCursor(0,3);
  lcd.print("Initializing");
  lcd.createChar(1,deg);
  //Wire.begin() // Init I2C protocol but lcd does this already
  dht.begin();
  Serial.println("AirMonitor initialized");
}

void loop() {
  lcd.setCursor(0,3);
  //         12345678901234567890
  lcd.print("Refreshing...    ");
  String timestring = getTimeStr();
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  //                 12345678901234567890 °
  String tempStr  = "T *C: ";
  String humidStr = "RH %: ";

  // Check if reads failed
  boolean hnan = isnan(h);
  boolean tnan = isnan(t);
  if (hnan) {
    Serial.println("Failed to read humidity from DHT sensor!");
    humidStr+="ERR";
    lcd.setCursor(0,3);
    //         12345678901234567890
    lcd.print("Refreshing failed");
    // Clear humidity number
    lcd.setCursor(6,2);
    lcd.print("      ");
  }
  lcd.setCursor(0,3);
  if (tnan) {
    Serial.println("Failed to read temperature from DHT sensor!");
    tempStr+="ERR";
    //         12345678901234567890
    lcd.print("Refreshing failed");
    // Clear temperature number
    lcd.setCursor(6,1);
    lcd.print("      ");
  } else if (!hnan) { // tnan is already false here
    tempStr+=String(t,1);
    humidStr+=String(h,1);
    //         12345678901234567890
    lcd.print("                    ");
    lcd.setCursor(0,0);
    lcd.print(timestring);
  }
  lcd.setCursor(0,1);
  // handle degree symbol with loop printing
  char* tempCharArr = tempStr.c_str();
  while(*tempCharArr!='\x00') {
    char charPrint = *tempCharArr;
    if (charPrint=='*') {
      lcd.write(1);
    } else {
      lcd.write(*tempCharArr);
    }
    tempCharArr++;
  }
  lcd.setCursor(0,2);
  lcd.print(humidStr);

  Serial.print(timestring);
  Serial.print(" : ");
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C");
  // Wait a few seconds between measurements
  // Loop time will be slightly above the specified time but that is acceptable
  delay(LOOP_DELAY);
}
