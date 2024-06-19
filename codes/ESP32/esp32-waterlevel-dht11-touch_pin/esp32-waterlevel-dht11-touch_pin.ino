#include <WiFi.h>
#include <DHT.h>
#include <ThingSpeak.h>

#define DHTPIN 4       // Pin connected to DHT11 sensor
#define DHTTYPE DHT11   // DHT11 sensor type


#define TOUCH_PIN 2         // Pin connected to touch sensor
#define LED_PIN 13
#define POWER_PIN  17 // ESP32 pin GPIO17 connected to sensor's VCC pin
#define SIGNAL_PIN 36 // ESP32 pin GPIO36 (ADC0) connected to sensor's signal pin

int waterLevel = 0; // variable to store the sensor value


const char* ssid = "realme8";     // Your WiFi SSID
const char* password = "Siddem@2515"; // Your WiFi password

const char* server = "api.thingspeak.com"; // ThingSpeak server
const char* api_key = "094PGSCWRBWZ9Z2Z";   // ThingSpeak API key

DHT dht(DHTPIN, DHTTYPE);

WiFiClient client;

void setup() {
  Serial.begin(115200);
  pinMode(TOUCH_PIN, INPUT);

  pinMode(LED_PIN, OUTPUT);
   pinMode(POWER_PIN, OUTPUT);   // configure pin as an OUTPUT
  digitalWrite(POWER_PIN, LOW);
  dht.begin();
  connectWiFi();
}

void loop() {
   digitalWrite(POWER_PIN, HIGH);  // turn the sensor ON
  float temperature = dht.readTemperature();  // Read temperature from DHT11
  float humidity = dht.readHumidity();        // Read humidity from DHT11
  int waterLevel = analogRead(SIGNAL_PIN); // Read water level
  int touchState = digitalRead(TOUCH_PIN);     // Read touch sensor state
int calibratedValue = map(waterLevel, 0, 3000, 0, 100);

  digitalWrite(POWER_PIN, LOW);   // turn the sensor OFF
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }



  delay(1000);

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C\t Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t Fuel Level: ");
  Serial.print(calibratedValue);
  Serial.print("\t Touch Sensor: ");
  Serial.print(touchState);
 

  // Send data to ThingSpeak
  ThingSpeak.begin(client);
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);
  ThingSpeak.setField(3, calibratedValue);
  ThingSpeak.setField(4, touchState);
  

  // Turn on LED if touch pin is high, otherwise turn it off
  if (touchState == 1) {
    digitalWrite(LED_PIN, HIGH); 
    }
   else{
    digitalWrite(LED_PIN,LOW);
    }
   
  

  int status = ThingSpeak.writeFields(2479161 , api_key);
  if (status == 200) {
    Serial.println("Data sent to ThingSpeak successfully!");
  } else {
    Serial.println("Failed to send data to ThingSpeak.");
  }

  delay(20000); // Delay 20 seconds between updates
}

void connectWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}
