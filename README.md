# ESP32 Weather Clock with ILI9341 Display

This project is a simple weather and clock display built with an **ESP32** and a **2.8" ILI9341 TFT display**. It connects to WiFi to retrieve the current time via an **NTP (Network Time Protocol)** server and fetches weather information from **OpenWeatherMap**. The time and weather are displayed on the TFT screen.

## Features
- Displays the current time and date in a readable format.
- Shows the current weather and temperature using data from OpenWeatherMap.
- Customizable UTC offset for accurate local time display.
- Auto-updates time and weather at specified intervals.

## Components
- ESP32 Development Board
- 2.8" ILI9341 TFT Display (with SPI interface)
- Internet connection (WiFi)

  

## Wiring

| ILI9341 Pin | ESP32 Pin   |
|-------------|-------------|
| VCC         | 3.3V        |
| GND         | GND         |
| CS          | GPIO 17     |
| RESET       | GPIO 5      |
| D/C         | GPIO 16     |
| SD_MOSI     | GPIO 23     |
| SD_SCK      | GPIO 18     |
| LED         | GPIO 32     |
| SD_MISO     | GPIO 19     |
| T_CLK       | GPIO 18     |
| T_CS        | GPIO 21     |
| T_DI        | GPIO 23     |
| T_DO        | GPIO 19     |
| SD_CS       | GPIO 12     |

## Setup Instructions
Open the project in the Arduino IDE.

Install the required libraries:

- Adafruit GFX Library
- Adafruit ILI9341
- ArduinoJson
- NTPClient (already included with ESP32 board setup)
- Update WiFi credentials and OpenWeatherMap API key in the code:
## Update These
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";
const String apiKey = "YOUR_API_KEY";

Upload the code to your ESP32 :)


