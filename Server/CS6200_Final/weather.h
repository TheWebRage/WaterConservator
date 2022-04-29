#ifndef WEATHER_H
#define WEATHER_H

#include <WiFiNINA.h>
#include <Arduino_MKRIoTCarrier.h>
#include <Arduino_JSON.h>

#include "globals.h"

void updateWeather() {
  String latitude = "41.22";
  String longitude = "-111.97";
  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (weatherClient.connect(serverAddress, 80)) {
    Serial.println("connected to server");
    // Make a HTTP request:
    weatherClient.print("GET /data/2.5/onecall?");
    weatherClient.print("lat=33.44&lon=-94.04");// + latitude);
    // weatherClient.print("long=" + longitude);
    weatherClient.print("&exclude=current,minutely,hourly,alerts");
    weatherClient.print("&appid=" + apiKey);
    weatherClient.println("");
    weatherClient.println("Host: api.openweathermap.org");
    weatherClient.println("Connection: close");
    weatherClient.println();
  } else {
    Serial.println("unable to connect");
  }
  delay(1000);
 
  String line = "";
  if (weatherClient.connected()) {
    line = weatherClient.readStringUntil('\n');
    Serial.println(line);
    JSONVar myObject = JSON.parse(line);
 
    cityName = JSON.stringify(myObject["name"]);
    
    // Loop over the 3 day forcast and save to display to the sceen later
    for (int i = 0; i < 5; i++) {
      weatherDescription[i] = JSON.stringify(myObject["daily"][i]["weather"][0]["description"]);
    }
    
 
    //temperature = myObject["main"]["temp"];
    //feelsLike = myObject["main"]["feels_like"];
    //lon = myObject["coord"]["lon"];
    //lat = myObject["coord"]["lat"];
 
    //the windSpeed and pressure variables are new additions
    //windSpeed = myObject["wind"]["speed"];
    //pressure = myObject["main"]["pressure"];
 
    // carrier.Buzzer.sound(300);
    // delay(300);
    // carrier.Buzzer.noSound();
    // delay(300);
    // carrier.Buzzer.sound(300);
    // delay(300);
    // carrier.Buzzer.noSound();
  }
}
#endif