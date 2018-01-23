/* Example on http POST request on ThingSpeak API through ESP    
      
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
#include <DHT.h>

// Using DHT11 Sensor
#define DHTTYPE DHT11
#define DHTPIN  2

// Replace this with your WiFi
// name and password 
const char* ssid =  "YourWiFiSSID";
const char* password =  "YourWiFiPassword";

// ThingSpeak Api Server Address
const char* server = "api.thingspeak.com";

// Replace with your ThingSpeak Api Key
String ApiKey = "YourAPIKey";
String uri="/update?api_key=" + ApiKey;

// Defining Global variables
float temperature;
float humidity;

String data="";
String request_header_1="";
String request_header_2=""; 

// Creating WiFiClient object
WiFiClient client;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);

  // Connecting to WiFi Network
  WiFi.begin(ssid, password);

  Serial.print("Connecting to");
  Serial.println(ssid);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
    }
  Serial.print("\r\n");
  Serial.println("Wifi Connected \n");
}
void loop() {

  // Posting to ThingSpeak sever again and again
  // with 5 seconds of delay
  ThingSpeak();
  delay(5000);
}

void ThingSpeak(){
  // connecting to ThingSpeak Server
  if(client.connect(server, 80)){
      Serial.println("Connected to thingsSpeak \n");

      // Preparing data
      getValues();

      Serial.print("Current Temperature is:-  ");
      Serial.print((int)temperature);
      Serial.println("°C");

      Serial.print("Current Humidity is:-  ");
      Serial.print((int)humidity);
      Serial.print("%");
      
      Serial.println("\n");

      Serial.println("Uploading Data to thingsSpeak \n");

      // Creating Headers to post on Server
      request_header_1= "POST " + uri + " HTTP/1.1\r\n"+ "Host: " + server + ":" + "80" + "\r\n" 
                        + "Accept: *" + "/" + "*\r\n" + "Content-Length: " + data.length() + "\r\n" ;      
      request_header_2 = "Content-Type: application/json\r\n\r\n";

      
      client.print(request_header_1);
      client.print(request_header_2);
      client.print(data);
    }
    else{
      Serial.println("Failed to connect thingSpeak Server");  
    }
  
 }

void getValues(){
  
    humidity = dht.readHumidity();          // Read humidity (percent)
    temperature = dht.readTemperature();    // Read Temperature (Celsius)

    // Creating a Json data of Temperature and Humidity
    
    data = "";
    data += "{";
    data += "\"field1\":\"" + String(temperature) + "\",";      // Temperature 
    data += "\"field2\":\""+String(humidity)+"\"";      // Humidity
    data += "}";     
}
