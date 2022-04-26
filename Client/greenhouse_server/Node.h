#ifndef NODE_H
#define NODE_H

// Holds information about the client node
class Node {
public:
  // Mac address of this node
  char* macAddress;
  
  // Sensor Readings
  int moistureLevel1;
  int moistureLevel2;
  int lightLevel;
  int humidity;
  int temperature;
  
  // Relay statuses - TODO: Do we need?
  int relay1Status;
  int relay2Status;
};

#endif