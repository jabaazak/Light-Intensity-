ESP8266 LDR Live Intensity Monitor

A real-time light intensity monitoring project using an ESP8266 and LDR, with live graph visualization hosted on a web server.


🌟 Features

📈 Live graph of LDR intensity (0–1023 scale)

🌈 Dynamic color mapping from black (dark) to white (bright)

📲 Responsive web UI – view on mobile, tablet, or desktop

⏸️ Pause/Resume graph updates

💾 Save as CSV to download recent data

🎨 Gradient fill under graph for enhanced visual feedback

⚙️ Hardware Requirements

ESP8266 (NodeMCU or similar)

Light Dependent Resistor (LDR)

10kΩ resistor

Breadboard & jumper wires

🧰 Circuit:

LDR  ---->  A0 (ESP8266)
           |
          10kΩ
           |
          GND

🚀 Getting Started

Clone this repository

Open the .ino file in the Arduino IDE

Install required libraries:

ESP8266 board package

Replace Your_SSID and Your_PASSWORD with your Wi-Fi credentials

Upload the code to your ESP8266

Open the Serial Monitor and note the IP address

Open the IP in any browser on the same network

🧠 How It Works

ESP8266 reads analog light data from the LDR

A web server runs on the ESP8266

The webpage fetches /ldr data every second using JavaScript

Chart.js updates the graph and color box in real-time

📁 Output

Web Dashboard: Real-time chart and color feedback

CSV Export: Save current graph data with timestamps
