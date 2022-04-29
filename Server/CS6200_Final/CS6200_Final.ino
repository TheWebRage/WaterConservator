#include <WiFiNINA.h>
#include <Arduino_MKRIoTCarrier.h>

#include "globals.h"
#include "Node.h"
#include "csv.h"
#include "printHTML.h"
#include "weather.h"
 
  // TODO:
  // - Recieve POST requests with information
  // - Each Node (Save?)
  //   - Method to send signal to water
  //   - Algorithm with recieved data
  //     - Update watering time when new data is received (No global timer, just default next watering time that is updated)
  //     - Weather forcast API
  //     - Sensor readings (From POST request)
  //     - Keep track of all nodes connected and display them in the 
  
void setup() {
  Serial.begin(9600);
  delay(1500);
  CARRIER_CASE = false;
  carrier.begin();
  
  carrier.display.setTextSize(2);
  carrier.display.setCursor(20, 50); 
  
  enable_WiFi();
  connect_WiFi();
  
  server.begin();
  printWifiStatus();
  
  updateWeather();
  
  //writeToFile(databaseFileName, "macAddress,moistureLevel1,moistureLevel2,lightLevel,humidity,temperature,relay1Status,relay2Status");
}
 
void loop() {
  // Override button for watering
  carrier.Buttons.update();
  // Serial.println("1");
  if (carrier.Buttons.onTouchDown(TOUCH2)) {
    WiFiClient waterClient;
    // String nodeAddress = "http://192.168.0.37/";
    // nodeAddress.concat(nodes[0].wateringTime);
    // char temp[] = "";
    // nodeAddress.toCharArray(temp, nodeAddress.length() + 1);
    IPAddress temp (192, 168, 0, 37);
    
    if (waterClient.connect(temp,80)) {
      waterClient.stop();
    }
    
    Serial.println("\nWatering Client\n");
    Serial.println(temp);
    Serial.println(nodes[0].wateringTime);
    
    // Reset the timer
    nodes[0].countdownTimer = millis();
  }
  
  client = server.available();
  if (client) {
    printWEB();
  }
  
  
  if((millis()-nodes[0].countdownTimer) > nodes[0].timeToWater) {
    // Send a signal to water
    WiFiClient waterClient;
    // String nodeAddress = "http://192.168.0.37/";
    // nodeAddress.concat(nodes[0].wateringTime);
    char temp[] = "http://192.168.0.37";
    // nodeAddress.toCharArray(temp, nodeAddress.length() + 1);
    // IPAddress temp (192, 168, 0, 37);
    
    if (waterClient.connect(temp,80)) {
      waterClient.println("");
      waterClient.println("GET /900 HTTP");
      waterClient.println("Host: 192.168.0.37");
      waterClient.println("Connection: close");
      waterClient.stop();
      Serial.println("\nSending\n");
    }
    
    Serial.println("\nWatering Client\n");
    Serial.println(temp);
    Serial.println(nodes[0].wateringTime);
    
    // Reset the timer
    nodes[0].countdownTimer = millis();
  }
  
  
}
 
void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  
  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
 
  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
 
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
 
  carrier.display.fillScreen(ST77XX_BLACK);
  carrier.display.println("  ** Successfully        Connected **");
  carrier.display.println("  Find link in the    Serial Monitor");
}
 
void enable_WiFi() {
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }
 
  String fv = WiFi.firmwareVersion();
  if (fv < "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }
}
 
void connect_WiFi() {
  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
 
    // wait 10 seconds for connection:
    delay(10000);
  }
}
 
 
void readSensors() {
 
  int rawMoistValue = analogRead(moistPin);
  int moistValueTemp = map(rawMoistValue, 0, 1023, 100, 0);
  moistValue = moistValueTemp * 2.777;
 
  temperature = carrier.Env.readTemperature();
  humidity = carrier.Env.readHumidity();
 
  if (carrier.Light.colorAvailable()) {
    // read the color
    int none; //not gonna be used
    carrier.Light.readColor(none, none, none, light);
    
  }
}