#include <Arduino.h>
#include <WiFiNINA.h>
#include <ThingSpeak.h>
#include <DHT.h>

// Replace with your network credentials
char ssid[] = "Galaxy";
char password[] = "1234567811";

// Replace with your ThingSpeak API key
const char* apiKey = "KWPVGTO697B58EWU";

// DHT sensor setup
#define DHTPIN 2       // Pin where the DHT22 is connected
#define DHTTYPE DHT22  // DHT 22 (AM2302)

DHT dht(DHTPIN, DHTTYPE);

// ThingSpeak channel setup
WiFiClient client;

// Replace with your ThingSpeak channel ID
unsigned long channelID = 2250586;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("WiFi module not found");
    while (true);
  }

  // Attempt to connect to WiFi network
  while (WiFi.begin(ssid, password) != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  ThingSpeak.begin(client);
}

void loop() {
  // Read DHT sensor data
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Check if any reads failed and exit if so
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(2000);
    return;
  }
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  // Send data to ThingSpeak
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);
  int response = ThingSpeak.writeFields(channelID, apiKey);
  
  if (response == 200) {
    Serial.println("Data sent to ThingSpeak");
  } else {
    Serial.print("Error sending data to ThingSpeak. Response code: ");
    Serial.println(response);
  }
  
  delay(20000); // Delay for 20 seconds before sending the next data
}
