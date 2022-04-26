#ifndef PRINTHTML_H
#define PRINTHTML_H

#include <WiFiNINA.h>
#include <Arduino_MKRIoTCarrier.h>

#include "Node.h"

void printWeatherInfo() {
  
  client.println("<div style=\"background-color:Azure;border-radius:5px;\">");
  
  client.println("<h2>Weather</h2>");
  client.println("<p>Overcast: </p>");
  client.println("<table>");
  client.println("<tr><td>Today</td><td>Tomorrow</td><td>3-Days</td><td>4-Days</td><td>5-Days</td></tr>");
  
  client.println("<tr>");
  // Display the 3 day forcast for the weather
  for (int i = 0; i < 5; i++) {
    client.println("<td>");
    client.println(weatherDescription[i]);
    client.println("</td>");
  }
  client.println("</tr>");
  client.println("</table>");
  
  client.println("</div>");
}

void printNodeHTML(Node node) {
  
  //create a div to contain our properties
  client.println("<center><div class='textField'>");

  //print moist level in plant
  client.println("<i class='fab fa-pagelines'></i>");
  client.print("The moisture level in your plant: ");
  client.print(node.moistureLevel1);
  client.print(" %");
  client.println("<br><br>");

  //print light level
  client.println("<i class='fas fa-sun'></i>");
  client.print("Illuminance is: ");
  client.print(node.lightLevel);
  client.print(" LUX");
  client.println("<br><br>");

  //print humidity
  client.println("<i class='fas fa-tint'></i>");
  client.print("The local humidity is: ");
  client.print(node.humidity);
  client.print(" %");
  client.println("<br><br>");

  //print temperature
  client.println("<i class='fas fa-thermometer-quarter'></i>");
  client.print("The local temperature is: ");
  client.print(node.temperature);
  client.print(" celsius");
  client.println("<br><br>");

  //state of relay 
  client.println("<i class='fas fa-bolt'></i>");
  client.print("The relay is currently: ");
  client.print(node.relay1Status);
  client.println("<br><br>");

  //create the buttons
  client.print("<button class='on' type='submit' value='ON' onmousedown=location.href=");
  client.print("'/H\'");
  client.println(">RELAY ON</button>");

  client.print("<button class='off' type='submit' value='OFF' onmousedown=location.href=");
  client.print("'/L\'");
  client.println(">RELAY OFF</button>");

  //close the div
  client.println("</div></center>");
}
 
void printWEB() {
 
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
            client.println("<meta http-equiv=\"refresh\" content=\"5\">");
            client.print("<head>");
            client.print("<script src='https://kit.fontawesome.com/0a4ce480d6.js'></script>");
            client.print("<style>");
            // client.print(".textField {font-size: 48px; font-family: Verdana, Geneva, Tahoma, sans-serif;  margin-top: 150px;}");
 
            // //moisture
            // if (moistValue > 40) {
            //   client.print(".fa-pagelines {font-size: 48px; color: green;}");
            // }
            // else {
            //   client.print(".fa-pagelines {font-size: 48px; color: red;}");
            // }
 
            // //humidity
            // if (humidity >= 30 && humidity <= 50) {
            //   client.print(".fa-tint {font-size: 48px; color: green;}");
            // }
            // else {
            //   client.print(".fa-tint {font-size: 48px; color: red;}");
            // }
 
            // //light
            // if (light > 300) {
            //   client.print(".fa-sun {font-size: 48px; color: green;}");
            // }
            // else {
            //   client.print(".fa-sun {font-size: 48px; color: red;}");
            // }
            
            // //Temperature
            // if (temperature >= 22 && temperature <= 28) {
            //   client.print(".fa-thermometer-quarter {font-size: 48px; color: green;}");
            // }
            // else {
            //   client.print(".fa-thermometer-quarter {font-size: 48px; color: red;}");
            // }
 
            // if (relayState == "ON") {
            //   client.print(".fa-bolt {font-size: 48px; color: green;}");
            // }
            // else {
            //   client.print(".fa-bolt {font-size: 48px; color: red;}");
            // }
 
 
            // client.print(".on {border: none; background-color: green; color: white;font-size: 36px; padding: 15px; border-radius: 10px;}");
            // client.print(".off {border: none; background-color: red; color: white; font-size: 36px; padding: 15px; border-radius: 10px;}");
            client.print("table{border-collapse:collapse;border:1px solid #FF0000;}table td{border:1px solid #FF0000;}");
            client.print("input[type='checkbox'] { display: none; } .wrap-collabsible { margin: 1.2rem 0; } .lbl-toggle { display: block; font-weight: bold; font-family: monospace; font-size: 1.2rem; text-transform: uppercase; text-align: center; padding: 1rem; color: #DDD; background: #0069ff; cursor: pointer; border-radius: 7px; transition: all 0.25s ease-out; } .lbl-toggle:hover { color: #FFF; } .lbl-toggle::before { content: ' '; display: inline-block; border-top: 5px solid transparent; border-bottom: 5px solid transparent; border-left: 5px solid currentColor; vertical-align: middle; margin-right: .7rem; transform: translateY(-2px); transition: transform .2s ease-out; } .toggle:checked+.lbl-toggle::before { transform: rotate(90deg) translateX(-3px); } .collapsible-content { max-height: 0px; overflow: hidden; transition: max-height .25s ease-in-out; } .toggle:checked + .lbl-toggle + .collapsible-content { max-height: 350px; } .toggle:checked+.lbl-toggle { border-bottom-right-radius: 0; border-bottom-left-radius: 0; } .collapsible-content .content-inner { background: rgba(0, 105, 255, .2); border-bottom: 1px solid rgba(0, 105, 255, .45); border-bottom-left-radius: 7px; border-bottom-right-radius: 7px; padding: .5rem 1rem; } .collapsible-content p { margin-bottom: 0; }");
 
            client.print("</style>");
            
            
            client.print("<script>");
            
            client.print("var coll = document.getElementsByClassName(\"collapsible\");var i;for (i = 0; i < coll.length; i++) {  coll[i].addEventListener(\"click\", function() { this.classList.toggle(\"active\");    var content = this.nextElementSibling;    if (content.style.maxHeight){      content.style.maxHeight = null;    } else {      content.style.maxHeight = content.scrollHeight + \"px\";    }  });}");
            
            client.print("</script>");
            
            
            client.print("</head>");
 
            client.print("<body>");
            
            // Print theweather info at the top of the screen
            printWeatherInfo();
            
            // Get list of nodes and create a collaspable section for each
            Node* nodes = readNodesFromFile();
            
            client.print("<h2>");
            client.print(numNodes);
            client.print(" Nodes Connected</h2>");
            
            for (int i = 0; i < numNodes; i++) {
              client.print("<div class=\"wrap-collabsible\"> <input id=\"collapsible\" class=\"toggle\" type=\"checkbox\"> <label for=\"collapsible\" class=\"lbl-toggle\">");
              //client.print(nodes[i].macAddress);
              client.print("TestMacAddress");
              client.print("</label><div class=\"collapsible-content\"><div class=\"content-inner\">");
              client.print("<div>");
              printNodeHTML(nodes[i]);
              client.print("</div></div></div></div>");
            }
            
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
 
        if (currentLine.endsWith("GET /H")) {
          carrier.Relay1.open();
          relayState = "ON";
        }
        if (currentLine.endsWith("GET /L")) {
          carrier.Relay1.close();
          relayState = "OFF";
        }
 
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}

#endif