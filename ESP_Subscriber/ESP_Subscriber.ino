/* Example of MQTT Subscriber on CloudMQTT through ESP    

 *  Message will publish through the Website of CloudMQTT
 *  on the Subscribed Topic. Publish '1' or '0'(without quotes)
  
 *  If the Subscribed topic will get message '1' then led 
 *  will glow otherwise on '0' it will off.
 
      ESP Connection

  o        o         o      o
  GND   led +ve             RX

  
  TX      3.3              3.3
  o        o         o      o


Connect ledPin +Ve Pin to ESP GPIO 2 pin With a 50 Ω of resistor
Otherwise it will blow and connect -ve to GND.
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const int ledPinPin = 2;
char state = 0;

// Replace this with your WiFi
// name and password 
const char* ssid = "YourWiFiSSID";
const char* password =  "YourWiFiPassword";

// MQTT Server address and Port
// Server and Port may differ
const char* mqttServer = "m14.cloudmqtt.com";
const int mqttPort = 17224;

// Repalce With your userID and Password
const char* mqttUser = "YourUserID";
const char* mqttPassword = "YourPassword";

WiFiClient espClient;
PubSubClient client(espClient);
 
void setup() {
 
  Serial.begin(115200);
  
  // Setting Pin 2 as output
  pinMode(ledPin, OUTPUT);

 // Connecting to WiFi Network
  WiFi.begin(ssid, password);

 // wait till WiFi is not Connected
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to the WiFi network");
}

 
void loop() {

  // if not connected to CloudMQTT Server
   while (!client.connected()) {
        reconnect();
    }

   // Comparing the parsed data 
   if ((char)state == '1'){
        digitalWrite(ledPin, true);
      }
      
   else if((char)state == '0'){
        digitalWrite(ledPin, false);
      }
      
    else {
        Serial.println("Wrong Input");
        Serial.println((char)state);
        }
        
  client.loop();
  
}

// getting the callback data from Server
void callback(char* topic, byte* payload, unsigned int length) {
    state = payload[0];
}

void reconnect(){
  // Setting Server and callback
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");


    // Connecting to MQTT Server
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failedPin with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
  // Subscribing Topic
  client.subscribe("esp/led");
 
  }
