// ArduinoJson - Version: Latest 
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

// ArduinoHttpClient - Version: Latest 
#include <ArduinoHttpClient.h>

#include <WiFiNINA.h>
#include <Arduino_MKRIoTCarrier.h>
#include <ctype.h>
MKRIoTCarrier carrier;

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

const char serverName[] = "http://192.168.0.11";
unsigned long lastTime = 0;
const unsigned long timerDelay = 15120;


int keyIndex = 0;                  // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;      // connection status
WiFiServer server(80);           // server socket

WiFiClient client = server.available();
byte mac[6];
String MAC = "";

int moistPin = A5;  //Set the analog pin

int moistValue;
float temperature;
float humidity;
int light;

//PROTOTYPES
void printWifiStatus();
void enable_WiFi();
void connect_WiFi();
void readSensors();
void printWEB();


void setup() {
  Serial.begin(9600);
  delay(2000);
  
  CARRIER_CASE = false;
  
  carrier.begin();
  carrier.display.setTextSize(2);
  carrier.display.setCursor(20, 50); 

  delay(2000);
  
  Serial.println("Enableing Wifi...");
  enable_WiFi();
  Serial.println("Connecting to Wifi...");
  connect_WiFi();
  Serial.println("Starting Server...");
  server.begin();
  printWifiStatus();
  
  WiFi.macAddress(mac);
  for(int i = 5; i >= 0; i--){
    MAC += mac[i];
    if(i != 0){
      MAC += ':';
    }
  }
  Serial.print("MAC: ");
  Serial.println(MAC);
  
}

void loop() {
    client = server.available();
    
    if(client){
      printWEB();
    }
    
    //override buttons
    carrier.Buttons.update();
    while(carrier.Buttons.getTouch(TOUCH1)){
      carrier.Relay1.open();
      carrier.Buttons.update();
    }
    carrier.Relay1.close();
    
    carrier.Buttons.update();
    while(carrier.Buttons.getTouch(TOUCH2)){
      carrier.Relay2.open();
      carrier.Buttons.update();
    }
    carrier.Relay2.close();
    
   //Send an HTTP POST request every 10 minutes
  if ((millis() - lastTime) > timerDelay) {
    Serial.println("Sending sensor data via POST");
    IPAddress server(192,168,0,11);
    WiFiClient postclient;
    
    //update sensor vars
    readSensors();
    //generate json object to send via POST
    DynamicJsonDocument doc(1024);
    doc["light"] = light;
    doc["temperature"] = temperature;
    doc["humidity"] = humidity;
    doc["moisture"] = moistValue;
    doc["mac"] = MAC;
    doc["ip"] = WiFi.localIP();
    
    
    String output;
    
    size_t sizeofT = serializeJson(doc, output);
    
    Serial.println(output);
    
    if (postclient.connect(server, 80)) {
      postclient.println("POST / HTTP/1.1");
      postclient.println("Host: 192.168.0.11");
      postclient.println("User-Agent: Arduino/1.0");
      postclient.println("Connection: close");
      postclient.print("Content-Length: ");
      postclient.println(sizeofT);
      postclient.println();
      postclient.println(output);
    }
    lastTime = millis();
    postclient.stop();
  }
}


void printWifiStatus(){
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
 
  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  
  //print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
  
  carrier.display.fillScreen(ST77XX_BLACK);
  carrier.display.println("  ** Successfully        Connected **");
    carrier.display.print("ip: ");
  carrier.display.println(ip);
  
}

void enable_WiFi(){
  // check for WiFi module:
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

void connect_WiFi(){
  // attept to connect to the wifi
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    carrier.display.println("Waiting for Connection ...");
    // wait 10 seconds for connection:
    delay(10000);
    
  }
  
}

void printWEB(){
  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character
 
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
                        // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            // the content of the HTTP response follows the header:
            client.println("<!DOCTYPE HTML>");
            //client.println("<meta http-equiv=\"refresh\" content=\"5\">");
            client.print("<head>");
            client.print("<style>");
 
            client.print("</style>");
            client.print("</head>");
                        //print moist level in plant
            client.println(currentLine);
            if(currentLine == ""){
              client.println("<p>Empty</p>");
            }
            
            //close the div
            client.println("</div>");
            client.println("</body>");
            client.println("</html>");
 
            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          }
          else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
        //Serial.println(currentLine);
        if(currentLine.endsWith("HTTP")){
          Serial.println(currentLine);
          String holder = currentLine;
          holder.replace(" HTTP", "");
          holder.replace("GET /", "");
          Serial.println(holder);
          int timeDelay = holder.toInt();
          
          carrier.Relay1.open();
          delay(timeDelay);
          carrier.Relay1.close();
          
        }
       
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}

void readSensors(){
  int rawMoistValue = analogRead(moistPin);
  moistValue = map(rawMoistValue, 0, 1023, 100, 0);
  
  temperature = carrier.Env.readTemperature();
  humidity = carrier.Env.readHumidity();
  
  if(carrier.Light.colorAvailable()){
    // read the color
    int none;
    carrier.Light.readColor(none, none, none, light);
    
  }
}


