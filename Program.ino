#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>

// Replace with your Wi-Fi credentials
const char* ssid = "Your_WiFi_Name";
const char* password = "Your_WiFi_Password";

// Telegram Bot Token from @BotFather
const char* BOTtoken = "YOUR_TELEGRAM_BOT_TOKEN";
const char* CHAT_ID = "YOUR_CHAT_ID";  // Get from @userinfobot

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

HardwareSerial GPSSerial(1);
TinyGPSPlus gps;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  // Connect to Wi-Fi
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWiFi Connected!");

  client.setInsecure();  // Use for TLS without certificate
  GPSSerial.begin(9600, SERIAL_8N1, 16, 17);  // GPS RX=16, TX=17
}

void loop() {
  while (GPSSerial.available()) {
    char c = GPSSerial.read();
    gps.encode(c);
  }

  if (gps.location.isUpdated()) {  
    float latitude = gps.location.lat();
    float longitude = gps.location.lng();
    
    String message = " *GPS Location Update*\n";
    message += " Latitude: " + String(latitude, 6) + "\n";
    message += " Longitude: " + String(longitude, 6) + "\n";
    message += " [Open in Google Maps](https://www.google.com/maps/search/?api=1&query=" + 
               String(latitude, 6) + "," + String(longitude, 6) + ")";

    Serial.println(message);
    bot.sendMessage(CHAT_ID, message, "Markdown");
    
    delay(10000);  // Send location every 10 seconds
  }
}
