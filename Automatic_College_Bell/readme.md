# Automatic College Bell System

This project is an **ESP8266-based Automatic Bell System** designed to ring bells according to a pre-defined schedule. It includes a user-friendly web interface to:
- View the current time.
- Add new bell times.
- Remove existing bell times.

The system uses the NTP (Network Time Protocol) to ensure accurate timekeeping and can be managed remotely via a web browser.

---

## Features

1. **Predefined Bell Schedule**:
   - The system comes with an initial bell schedule.
   - Supports up to 20 bell times.

2. **Web Interface**:
   - View the current time.
   - Add new bell times by specifying hours and minutes.
   - Remove existing bell times with a single click.

3. **NTP-based Time Sync**:
   - Ensures the system clock is accurate by syncing with an NTP server.

4. **Customizable for Any Timezone**:
   - Easily adjustable for your timezone (e.g., UTC +5:30 for IST).

---

## Components Required

1. ESP8266 (e.g., NodeMCU)
2. LED or Buzzer for the bell
3. Resistors and jumper wires
4. Wi-Fi network for connectivity

---

## Circuit Diagram

1. Connect an LED or a buzzer to **GPIO16 (D0)**.
2. Use appropriate resistors to limit current for the LED.
3. Ensure the ESP8266 is powered properly and connected to a stable Wi-Fi network.

---

## How to Set Up

1. Clone this repository or download the code.
2. Install the required libraries in the Arduino IDE:
   - **ESP8266WiFi.h**
   - **NTPClient.h**
   - **WiFiUdp.h**
3. Update the following in the code:
   ```cpp
   const char* ssid = "your_wifi_ssid";       // Replace with your Wi-Fi SSID
   const char* password = "your_wifi_password"; // Replace with your Wi-Fi password
   const long utcOffsetInSeconds = 19800;      // Adjust for your timezone

    Upload the code to your ESP8266 using the Arduino IDE.
    Open the Serial Monitor to see the device's IP address.
    Visit the displayed IP address in your web browser to access the web interface.

## Web Interface

### The web interface displays:

    The current time synced with NTP.
    The list of scheduled bell times with "Delete" buttons.
    A form to add new bell times.

### Add New Bell Time

    Enter the hour and minute for the new bell time.
    Click Add Bell Time.

### Delete Bell Time

    Click Delete next to the desired bell time to remove it from the schedule.

## Example Usage

    Set up the ESP8266 and power it on.
    Access the web interface using the displayed IP address.
    Add or modify bell times as needed.
    The bell will automatically ring according to the schedule.

## Limitations

    Supports a maximum of 20 bell entries. This can be increased by modifying the MAX_BELLS constant.
    Requires a stable Wi-Fi connection for NTP synchronization.


## License

This project is licensed under the MIT License.

Feel free to contribute to the project by submitting issues or pull requests!
Author

Created by Adarshkrdubay


### Customization:
- Replace `your_wifi_ssid`, `your_wifi_password`, and `19800` with placeholders for user configuration.
- Add actual screenshots of your web interface.
- Replace `Your Name` and `https://github.com/yourusername` with your details.
- Include a `LICENSE` file in your repository if necessary.
