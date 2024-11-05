#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// WiFi credentials
const char* ssid = "SSID";
const char* password = "Pass";

// OpenWeatherMap API details
const String apiKey = "Nope Not Today";
const String city = "CITY";  // URL-encoded city name
const String countryCode = "CA";
String weatherUrl = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&appid=" + apiKey + "&units=metric";

// TFT Display setup
#define TFT_CS     17
#define TFT_DC     16
#define TFT_RST    5
#define TFT_LED    32   // Backlight control pin
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

// NTP setup
WiFiUDP ntpUDP;

// Define UTC offset in seconds for Eastern Time
// -5 hours for EST (Standard Time) = -18000 seconds
// -4 hours for EDT (Daylight Saving Time) = -14400 seconds
const long utcOffsetInSeconds = -18000;  // Change to -14400 during Daylight Saving Time
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds, 60000);  // Sync time every minute

void setup() {
  Serial.begin(115200);
  pinMode(TFT_LED, OUTPUT);  // Set LED pin as output
  digitalWrite(TFT_LED, HIGH);  // Turn on the backlight

  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);

  // Connect to WiFi
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  // Start NTP client
  timeClient.begin();
  updateWeather();
}

void loop() {
  timeClient.update();
  displayTimeAndDate();
  delay(60000);  // Update every minute
}

// Function to display time and date on the display
void displayTimeAndDate() {
  Serial.println("Updating Time and Date...");
  tft.fillRect(0, 0, 240, 120, ILI9341_BLACK);  // Clear area

  // Display Time
  tft.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
  tft.setTextSize(4);
  tft.setCursor(10, 20);
  String timeStr = formatTime();
  tft.print(timeStr);
  Serial.println("Time: " + timeStr);

  // Display Date
  tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setCursor(40, 70);
  String dateStr = formatDate();
  tft.print(dateStr);
  Serial.println("Date: " + dateStr);
}

// Format Time as HH:MM
String formatTime() {
  int currentHour = timeClient.getHours();
  int currentMinute = timeClient.getMinutes();
  return (currentHour < 10 ? "0" : "") + String(currentHour) + ":" + (currentMinute < 10 ? "0" : "") + String(currentMinute);
}

// Format Date as DD/MM/YYYY from epoch time
String formatDate() {
  unsigned long epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime((time_t *)&epochTime);
  int day = ptm->tm_mday;
  int month = ptm->tm_mon + 1;  // tm_mon is 0-11, so we add 1
  int year = ptm->tm_year + 1900;  // tm_year is years since 1900
  return String(day) + "/" + String(month) + "/" + String(year);
}

// Function to fetch and display weather information
void updateWeather() {
  Serial.println("Fetching Weather Data...");
  Serial.println("Request URL: " + weatherUrl);  // Print the full request URL for debugging
  
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.setTimeout(5000);  // Set timeout to 5 seconds
    http.begin(weatherUrl);
    int httpCode = http.GET();

    if (httpCode == 200) {
      String payload = http.getString();
      Serial.println("Weather Data: " + payload);
      DynamicJsonDocument doc(1024);
      DeserializationError error = deserializeJson(doc, payload);

      if (error) {
        Serial.print("Failed to parse JSON: ");
        Serial.println(error.c_str());
        return;
      }
      
      // Extract and display weather information
      String weather = doc["weather"][0]["description"].as<String>();
      float temp = doc["main"]["temp"].as<float>();

      // Display weather data with colors
      tft.fillRect(0, 120, 240, 120, ILI9341_BLACK);  // Clear previous weather data

      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft.setTextSize(2);
      tft.setCursor(10, 140);
      tft.print("Weather: ");
      tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
      tft.setCursor(10, 160);
      tft.print(weather);

      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft.setCursor(10, 190);
      tft.print("Temp: ");
      tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
      tft.setCursor(90, 190);
      tft.print(String(temp) + " C");
    } else {
      Serial.println("Failed to retrieve weather data. HTTP code: " + String(httpCode));
    }
    http.end();
  } else {
    Serial.println("WiFi not connected.");
  }
}
