#include <Arduino.h>
#include <WiFiNINA.h>
#include <ThingSpeak.h>
#include <DHT.h>

char ssid[] = "Galaxy";
char password[] = "1234567811";

const char* apiKey = "KWPVGTO697B58EWU";

#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

WiFiClient client;

unsigned long channelID = 2250586;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("WiFi module not found");
    while (true);
  }

  while (WiFi.begin(ssid, password) != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  ThingSpeak.begin(client);
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(2000);
    return;
  }
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);
  int response = ThingSpeak.writeFields(channelID, apiKey);
  
  if (response == 200) {
    Serial.println("Data sent to ThingSpeak");
  } else {
    Serial.print("Error sending data to ThingSpeak. Response code: ");
    Serial.println(response);
  }
  
  delay(20000);
}
