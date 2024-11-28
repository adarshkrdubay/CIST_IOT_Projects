#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// WiFi Credentials
const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";

// Time Zone Offset (UTC +5:30 for IST)
const long utcOffsetInSeconds = 19800;

// Bell schedule structure
struct BellTime {
  int hour;
  int minute;
};

// Maximum bell entries
#define MAX_BELLS 20

// Define bell schedule
BellTime bellSchedule[MAX_BELLS] = {
  {8, 55}, {9, 0}, {10, 0}, {11, 0}, {11, 55},
  {12, 0}, {13, 30}, {13, 55}, {14, 0}, {15, 0}
};
int bellCount = 10; // Initial number of bells

// NTP Client
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

// LED pin
int LED = 16; // D0 on ESP8266

// Web server
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW); // Turn off the bell initially

  // Connect to WiFi
  Serial.print("Connecting to WiFi ");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Start NTP and server
  timeClient.begin();
  server.begin();
  Serial.println("Server started");
  Serial.printf("Visit http://%s/ to control the bell\n", WiFi.localIP().toString().c_str());
}

void loop() {
  timeClient.update();

  // Get current time
  int hh = timeClient.getHours();
  int mm = timeClient.getMinutes();
  int ss = timeClient.getSeconds();
  // Serial.printf("Current Time: %02d:%02d:%02d\n", hh, mm, ss);

  // Check bell schedule and ring the bell
  checkAndRingBell(hh, mm);

  // Handle HTTP requests
  WiFiClient client = server.available();
  if (client) {
    handleClientRequest(client, hh, mm, ss);
  }
}

// Function to check and ring the bell based on schedule
void checkAndRingBell(int hh, int mm) {
  for (int i = 0; i < bellCount; i++) {
    if (bellSchedule[i].hour == hh && bellSchedule[i].minute == mm) {
      digitalWrite(LED, HIGH); // Bell ON
      Serial.println("BELL ON");
      delay(5000); // Ring for 5 seconds
      digitalWrite(LED, LOW); // Bell OFF
      Serial.println("BELL OFF");
      delay(60000); // Prevent re-triggering within the same minute
    }
  }
}

// Function to handle client requests
void handleClientRequest(WiFiClient client, int hh, int mm, int ss) {
  // Read HTTP request
  String request = client.readStringUntil('\r');
  client.flush();

  // Parse and handle adding a new bell time
  if (request.indexOf("GET /add?") != -1) {
    int hour = parseParameter(request, "hour");
    int minute = parseParameter(request, "minute");
    if (hour != -1 && minute != -1 && bellCount < MAX_BELLS) {
      bellSchedule[bellCount++] = {hour, minute};
      Serial.printf("Added new bell time: %02d:%02d\n", hour, minute);
    }
  }

  // Parse and handle deleting a bell time
  if (request.indexOf("GET /delete?") != -1) {
    int index = parseParameter(request, "index");
    if (index >= 0 && index < bellCount) {
      for (int i = index; i < bellCount - 1; i++) {
        bellSchedule[i] = bellSchedule[i + 1];
      }
      bellCount--;
      Serial.printf("Deleted bell time at index: %d\n", index);
    }
  }

  // Respond with an HTML page
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<center><h1>Automatic College Bell</h1></center>");
  client.printf("<p>Current Time: %02d:%02d:%02d</p>", hh, mm, ss);

  // Form to add new bell time
  client.println("<h2>Add New Bell Time</h2>");
  client.println("<form action=\"/add\">");
  client.println("Hour: <input type=\"number\" name=\"hour\" min=\"0\" max=\"23\" required><br>");
  client.println("Minute: <input type=\"number\" name=\"minute\" min=\"0\" max=\"59\" required><br>");
  client.println("<button type=\"submit\">Add Bell Time</button>");
  client.println("</form>");

  // Display current bell schedule with delete buttons
  client.println("<h2>Current Bell Schedule</h2>");
  for (int i = 0; i < bellCount; i++) {
    client.printf("<p>%02d:%02d <a href=\"/delete?index=%d\"><button>Delete</button></a></p>", bellSchedule[i].hour, bellSchedule[i].minute, i);
  }

  client.println("</html>");
}

// Helper function to parse parameters
int parseParameter(String request, String paramName) {
  int start = request.indexOf(paramName + "=");
  if (start == -1) return -1;
  start += paramName.length() + 1;
  int end = request.indexOf('&', start);
  if (end == -1) end = request.indexOf(' ', start);
  return request.substring(start, end).toInt();
}
