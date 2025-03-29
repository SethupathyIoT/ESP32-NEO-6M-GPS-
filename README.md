This project is a real-time GPS tracking system that combines an ESP32, a NEO-6M GPS module,
and a Telegram bot to send live location updates via Wi-Fi. The system runs on a lithium-ion
battery with a charging module, making it wireless and portable.

📌 How the Project Works:

1️⃣ ESP32 Connects to Wi-Fi
The ESP32 first connects to a Wi-Fi network to enable communication with the Telegram bot.

Without Wi-Fi, the system won’t be able to send location updates.

2️⃣ GPS Module Collects Real-Time Location
The NEO-6M GPS module continuously receives data from satellites and provides:

Latitude & Longitude (Current location)

Altitude (Height above sea level)

Speed (If the device is moving)

Time & Date (Synchronized with GPS satellites)

The ESP32 reads this data and extracts the latitude and longitude.

3️⃣ ESP32 Processes GPS Data
The ESP32 takes the raw GPS data and filters out any corrupt or invalid readings.

It checks if the GPS signal is stable before sending any data.

4️⃣ Send Data to Telegram Bot
The ESP32 formats the GPS data into a readable message.

It sends the message to a predefined Telegram bot every 10 seconds (or based on movement).

The message includes:
✅ Latitude & Longitude
✅ Google Maps Link for easy tracking
✅ Time of update

5️⃣ View GPS Data on Telegram
The user receives a message like this:

📍 GPS Location Update
🌍 Latitude: 11.138651
🌍 Longitude: 77.319642
🔗 Open in Google Maps: https://www.google.com/maps/search/?api=1&query=11.138651,77.319642
      Clicking on the Google Maps link will open the exact location of the device.

📡 Hardware & Connections:
📌 Components Used:

Component	                                Function
ESP32	                                  Microcontroller that processes GPS data and sends it via Telegram
NEO-6M GPS Module	                      Receives location data from satellites
Lithium-Ion Battery (3.7V)	            Powers the system wirelessly
TP4056 Charging Module	                Charges the battery safely
MT3608 Boost Converter	                Converts battery voltage (3.7V) to 5V for ESP32
Wi-Fi Module (built-in ESP32)	          Connects to the internet for Telegram communication

📌 Wiring Diagram

1️⃣ Connecting ESP32 and GPS Module
ESP32	      NEO-6M GPS
3.3V	      VCC
GND	        GND
RX2         (GPIO16)	TX
TX2         (GPIO17)	RX

2️⃣ Connecting Battery & Power System

TP4056       Module	Battery
B+	          Battery +
B-	          Battery -
OUT+	        Boost Converter IN+
OUT-	        Boost Converter IN-

💾 Program Code Explanation:

1️⃣ Include Required Libraries:

#include <WiFi.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include <UniversalTelegramBot.h>
WiFi.h → To connect ESP32 to the internet

TinyGPS++.h → To extract useful GPS data

HardwareSerial.h → To communicate with the GPS module

UniversalTelegramBot.h → To send messages to Telegram

2️⃣ Define Wi-Fi and Telegram Bot Credentials:

const char* ssid = "YourWiFi";  // Change to your Wi-Fi SSID
const char* password = "YourPassword";  // Change to your Wi-Fi Password

#define BOT_TOKEN "Your_Telegram_Bot_Token"
#define CHAT_ID "Your_Telegram_Chat_ID"

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);
Replace YourWiFi and YourPassword with your Wi-Fi credentials.

Replace Your_Telegram_Bot_Token with the Telegram bot token.

Replace Your_Telegram_Chat_ID with your Chat ID (so only you receive updates).

3️⃣ Initialize GPS and Serial Communication:

HardwareSerial GPSSerial(1);  // Using UART1
TinyGPSPlus gps;

void setup() {
  Serial.begin(115200);  // Debug output
  GPSSerial.begin(9600, SERIAL_8N1, 16, 17);  // GPS RX=16, TX=17
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi!");
}
GPSSerial.begin(9600, SERIAL_8N1, 16, 17); → Configures ESP32 to receive GPS data.

WiFi.begin() → Connects ESP32 to the internet.

Waits until Wi-Fi is connected before proceeding.

4️⃣ Extract GPS Data:

void loop() {
  while (GPSSerial.available()) {
    char c = GPSSerial.read();
    gps.encode(c);
  }

  if (gps.location.isUpdated()) {
    float latitude = gps.location.lat();
    float longitude = gps.location.lng();
    
    String message = " *GPS Location Update*\n";
    message += " *Latitude:* " + String(latitude, 6) + "\n";
    message += " *Longitude:* " + String(longitude, 6) + "\n";
    message += " [Open in Google Maps](https://www.google.com/maps/search/?api=1&query=" 
                + String(latitude, 6) + "," + String(longitude, 6) + ")";
    
    bot.sendMessage(CHAT_ID, message, "Markdown");
    Serial.println("Location Sent to Telegram!");

    delay(10000);  // Send update every 10 seconds
  }
}
Extracts GPS data (latitude and longitude).

Formats it into a Telegram message.

Sends the message via Telegram every 10 seconds.
