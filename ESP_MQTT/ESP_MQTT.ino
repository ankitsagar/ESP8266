/* Example of MQTT Publish on ThingsBoard API through ESP    
      
      ESP Connection

  o        o         o      o
  GND  DHT_Data_Pin         RX

  
  TX      3.3              3.3
  o        o         o      o


   DHT Wiring
Pin         Connection

Data        ESP GPIO 2
NC          Not used 
GND         GND
VCC         5V

  
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// Using DHT11 Sensor
#define DHTTYPE DHT11
#define DHTPIN  2

// Repalce With your Device Token
#define TOKEN "YourToken"

WiFiClient espClient;
PubSubClient client(espClient);

DHT dht(DHTPIN, DHTTYPE);

// Replace this with your WiFi
// name and password 
const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";

// ThingsBoard Server Address
const char* mqtt_server = "demo.thingsboard.io";

// Defining Global Variables for Temperature and Humidity
float temperature;
float humidity;

void setup() {
  Serial.begin(115200);

  Serial.print("Connecting to:  ");
  Serial.println(ssid);
  
  // Connecting to WiFi Network
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
   }

   Serial.println("\nWifi Connected Successfully");

  // Setting Mqtt Server
  client.setServer(mqtt_server, 1883);

}

void loop() {
  // if not connected to ThingsBoard Server 
  if(!client.connected()){
    connectToThingsboard();
    }

    // Publising Data to ThingsBoard
    // with delay of 2 seconds
    publishToThingsBoard();
    delay(2000);
    
    // Looping all the client Procedures
   client.loop();
 
}

void connectToThingsboard(){
   Serial.println("Connecting to ThingsBord......");

   // Connecting to Thingsbord Server
   if (client.connect("ESP8266Client", TOKEN, NULL)) {
      Serial.println( "Done" );
    } 
    else 
    {
      Serial.print( "Failed with status: " );
      Serial.print( client.state());
      Serial.println( " : retrying in 5 seconds" );
      // Wait 5 seconds before retrying
      delay( 5000 );
    }
}

void publishToThingsBoard()
{

  humidity = dht.readHumidity();          // Read humidity (percent)
  temperature = dht.readtemperature();           // Read temperature(celsius)

  
  // Prepare a JSON payload string
  String payload = "{";
  payload += "\"temperature\":"; 
  payload += String(temperature); 
  payload += ",";
  payload += "\"humidity\":"; 
  payload += String(humidity);
  payload += "}";
  
  // Send payload
  char attributes[200];
  payload.toCharArray( attributes, 200 );
  client.publish( "v1/devices/me/telemetry", attributes );
  Serial.println( attributes );
}

