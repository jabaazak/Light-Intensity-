#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "jabaaz";           // Replace with your WiFi SSID
const char* password = "singh3456";   // Replace with your WiFi Password

ESP8266WebServer server(80);

void handleLDR() {
  int ldrVal = analogRead(A0);
  server.send(200, "text/plain", String(ldrVal));
}

void handleRoot() {
  String html = R"rawliteral(
  <!DOCTYPE html>
  <html>
  <head>
    <title>LDR Live Chart</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <style>
      body {
        font-family: Arial, sans-serif;
        margin: 10px;
        background: #f9f9f9;
        text-align: center;
      }
      h2 {
        margin-bottom: 0;
      }
      canvas {
        width: 100% !important;
        max-width: 900px;
        background: #fff;
        border-radius: 10px;
        box-shadow: 0 0 8px rgba(0,0,0,0.1);
      }
      #colorBox {
        width: 40px;
        height: 40px;
        border-radius: 6px;
        border: 2px solid #444;
        display: inline-block;
        vertical-align: middle;
      }
      #colorLabel {
        font-weight: bold;
        margin-left: 10px;
        color: #4b0082;
      }
      .corner-box {
        position: absolute;
        top: 15px;
        right: 15px;
        display: flex;
        align-items: center;
      }
      #buttons {
        margin-top: 10px;
      }
      button {
        background: #4b0082;
        color: white;
        border: none;
        padding: 6px 12px;
        margin: 4px;
        border-radius: 5px;
        cursor: pointer;
      }
    </style>
  </head>
  <body>
    <h2>📊 LDR Light Intensity</h2>
    <canvas id="ldrChart"></canvas>
    <div class="corner-box">
      <div id="colorBox"></div>
      <span id="colorLabel">Waiting...</span>
    </div>
    <div id="buttons">
      <button onclick="togglePause()">Pause</button>
      <button onclick="downloadCSV()">Save CSV</button>
    </div>

    <script>
      let paused = false;
      const labels = [];
      const dataPoints = [];

      const ctx = document.getElementById('ldrChart').getContext('2d');
      const gradient = ctx.createLinearGradient(0, 0, 0, 400);
      gradient.addColorStop(0, 'rgba(0, 123, 255, 0.4)');
      gradient.addColorStop(1, 'rgba(255, 255, 255, 0)');

      const chart = new Chart(ctx, {
        type: 'line',
        data: {
          labels: labels,
          datasets: [{
            label: 'Intensity',
            data: dataPoints,
            borderColor: '#4b0082',
            backgroundColor: gradient,
            fill: true,
            tension: 0.4,
            pointRadius: 2,
          }]
        },
        options: {
          animation: false,
          scales: {
            x: {
              title: { display: true, text: 'Time' }
            },
            y: {
              title: { display: true, text: 'Intensity (0–1023)' },
              min: 0,
              max: 1023
            }
          }
        }
      });

      function updateColorBox(val) {
        const label = document.getElementById('colorLabel');
        const box = document.getElementById('colorBox');
        let color = 'black', text = 'Very Dark (Black)';

        if (val < 100)       { color = 'black'; text = 'Very Dark (Black)'; }
        else if (val < 200)  { color = 'indigo'; text = 'Dim Purple'; }
        else if (val < 400)  { color = 'blue'; text = 'Low Blue'; }
        else if (val < 600)  { color = 'green'; text = 'Medium Green'; }
        else if (val < 750)  { color = 'yellow'; text = 'Bright Yellow'; }
        else if (val < 900)  { color = 'orange'; text = 'Orange-Red'; }
        else                 { color = 'white'; text = 'White Light'; }

        box.style.background = color;
        label.textContent = text;
      }

      function fetchData() {
        if (paused) return;
        fetch('/ldr')
          .then(res => res.text())
          .then(val => {
            const now = new Date().toLocaleTimeString();
            if (labels.length >= 40) {
              labels.shift();
              dataPoints.shift();
            }
            labels.push(now);
            dataPoints.push(Number(val));
            chart.update();
            updateColorBox(Number(val));
          });
      }

      function togglePause() {
        paused = !paused;
      }

      function downloadCSV() {
        let csv = 'Time,Intensity\\n';
        for (let i = 0; i < labels.length; i++) {
          csv += `${labels[i]},${dataPoints[i]}\\n`;
        }
        const blob = new Blob([csv], { type: 'text/csv' });
        const url = URL.createObjectURL(blob);
        const a = document.createElement('a');
        a.href = url;
        a.download = 'ldr_data.csv';
        a.click();
        URL.revokeObjectURL(url);
      }

      setInterval(fetchData, 1000);
    </script>
  </body>
  </html>
  )rawliteral";

  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected!");
  Serial.println("IP: " + WiFi.localIP().toString());

  server.on("/", handleRoot);
  server.on("/ldr", handleLDR);
  server.begin();
}

void loop() {
  server.handleClient();
}
