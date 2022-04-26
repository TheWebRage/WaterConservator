#ifndef CSV_H
#define CSV_H

#include "CSV_Parser.h"
#include "globals.h"

// Writes a value onto an existing file
bool writeToFile(char*fileName, char* strToWrite) {
  // Open the file
  dataFile = SD.open(fileName, FILE_WRITE);
  delay(1000);
 
  // init the CSV file with headers
  dataFile.println(strToWrite);
  Serial.println(strToWrite);
 
  // close the file and return
  dataFile.close();
  delay(100);
  return true;
}
bool writeToFile(char*fileName, String strToWrite) {
  // Open the file
  dataFile = SD.open(fileName, FILE_WRITE);
  delay(1000);
 
  // init the CSV file with headers
  dataFile.println(strToWrite);
  Serial.println(strToWrite);
 
  // close the file and return
  dataFile.close();
  delay(100);
  return true;
}

// Delete all connected nodes
bool deleteNodes() {
  // Remove file then init again with new header
  Serial.println("Reseting database");
  SD.remove(databaseFileName);
  writeToFile(databaseFileName, "macAddress,moistureLevel1,moistureLevel2,lightLevel,humidity,temperature,relay1Status,relay2Status");
  numNodes = 0;
  return true;
}

// Use this to parse the lines for each Node (https://github.com/michalmonday/CSV-Parser-for-Arduino)
Node* readNodesFromFile() {
  Serial.println("Reading file for nodes");
  dataFile = SD.open(databaseFileName, FILE_READ);
  delay(100);
  char* file;
  
  // while(dataFile.available()) {
  //   file += dataFile.read();
  // }
  // CSV_Parser cp(file, "sLLLLLLL");
  
  
  Serial.println("Going to parse file contents");
  // Initialize the parser with the fileString and format
  CSV_Parser cp("sLLLLLLL", true, ',');
  if (!cp.readSDfile(databaseFileName)) {
    Serial.print("\nCouldn't read ");
    Serial.println(databaseFileName);
  }
  
  
  Serial.println("Creating arrays of values");
  // Take values from parser object and load into variables
  char** macAddresses =   (char**)cp["macAddress"];
  int32_t* moistureLevel1s = (int32_t*)cp["moistureLevel1"];
  int32_t* moistureLevel2s = (int32_t*)cp["moistureLevel2"];
  int32_t* lightLevels =     (int32_t*)cp["lightLevel"];
  int32_t* humidities =      (int32_t*)cp["humidity"];
  int32_t* temperatures =    (int32_t*)cp["temperature"];
  int32_t* relay1Statuses =  (int32_t*)cp["relay1Status"];
  int32_t* relay2Statuses =  (int32_t*)cp["relay2Status"];
  
  
  Serial.println("Creating node array and node objects");
  // Take loaded variables and load into Node objects
  numNodes = cp.getRowsCount();
  Serial.println(numNodes);
  Node* nodes = new Node[numNodes];
  
  for (int i = 0; i < numNodes; i++) {
    nodes[i].macAddress = macAddresses[i];
    nodes[i].moistureLevel1 = moistureLevel1s[i];
    nodes[i].moistureLevel2 = moistureLevel2s[i];
    nodes[i].lightLevel = lightLevels[i];
    nodes[i].humidity = humidities[i];
    nodes[i].temperature = temperatures[i];
    nodes[i].relay1Status = relay1Statuses[i];
    nodes[i].relay2Status = relay2Statuses[i];
    
    String macAddress = nodes[i].macAddress;
    String nodeStr = macAddress + "," + nodes[i].moistureLevel1 + "," + nodes[i].moistureLevel2 + "," + nodes[i].lightLevel + "," + nodes[i].humidity + "," + nodes[i].temperature + "," + nodes[i].relay1Status + "," + nodes[i].relay2Status;
    
    Serial.println(nodeStr);
  }

  // Close the file and return the list of retrieved nodes
  dataFile.close();
  delay(100);
  
  Serial.println("Finished");
  return nodes;
}

// Write a list of nodes to the database and overwrite old entries
bool writeNodesToFile(Node* nodes) {
  // TODO:
  // - write col headers into string
  // - write values for nodes on each row into same string
  // - overwrite existing database file
  //   - Keep copy and revert incase failure
  // - return true if successful, false if failure
  return false;
}

// Adds a single node to the existing database
bool addNewNode(Node node) {
  // Format string into csv format
  Serial.print("Adding Node");
  String macAddress = node.macAddress;
  String nodeStr = macAddress + "," + (String)node.moistureLevel1 + "," + node.moistureLevel2 + "," + node.lightLevel + "," + node.humidity + "," + node.temperature + "," + node.relay1Status + "," + node.relay2Status;
  
  Serial.println(nodeStr);
  
  // Append (write) to file and return
  writeToFile(databaseFileName, nodeStr);
  return true;
}

#endif