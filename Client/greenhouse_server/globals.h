#ifndef GLOBALS_H
#define GLOBALS_H

#include <WiFiNINA.h>
#include <Arduino_MKRIoTCarrier.h>
#include <SPI.h>
#include <SD.h>

// Access to board
MKRIoTCarrier carrier;
 
// Wifi connection information
char ssid[] = "NETGEAR20";        // your network SSID (name) between the " "
char pass[] = "royallotus782";    // your network password between the " "

// Localhost server information
int keyIndex = 0;                 // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;      // connection status
WiFiServer server(80);            // server socket
WiFiClient client = server.available();

// Information for the weather API
char serverAddress[] = "api.openweathermap.org";
String apiKey = "b21d29b1d9adc4401e8889e7805701ee";
String location = "Salt Lake City, US";
String cityName;
String weatherDescription[] = {"Loading", "Loading", "Loading"};
WiFiClient weatherClient;
 
// Analog pin set
const int moistPin = A5;  //Set the analog pin
 
// State of the relay pin
String relayState = "A5";

// Sensor readings
int moistValue;
float temperature;
float humidity;
int light;

// File to read and write values for saving 
File dataFile;
char* databaseFileName = "nodes.db";

// Information about the connected nodes
// - Updates everytime there is a call to the readNodesFromFile() method
int numNodes = 0;

#endif