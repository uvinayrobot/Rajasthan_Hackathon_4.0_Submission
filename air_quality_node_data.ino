
// Air Quality Node ... Resposible for Sending data to Cloud 
// Parameters :  Carbon Monooxide ,Dust Concentration ,Temperature and Humidity 
#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
#include "DHT.h"
#include<string.h>
#define DHTPIN 2
#define DHTTYPE DHT11

//DHT dht(DHTPIN,DHTTYPE);
char ssid[] = "iPhone"; // your network SSID (name)
char pass[] = "varma1223"; // your network password
 int status = WL_IDLE_STATUS;
 WiFiClient client;
unsigned long myChannelNumber = 454108; // Channel Number
const char * myWriteAPIKey = "O7AULPKRHIH2RY23"; // Channel Write API Key

DHT dht_temp(DHTPIN,DHTTYPE);
byte buff[2];
int pin = 4;//DSM501A input D8
unsigned long duration;
unsigned long starttime;
unsigned long endtime;
unsigned long sampletime_ms = 30000;
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;
 
int i=0;




void setup() {
WiFi.begin(ssid, pass);
Serial.begin(9600);
ThingSpeak.begin(client);
dht_temp.begin();
pinMode(4,INPUT);
starttime = millis();
Serial.println("Here in setup loop");
}




void loop() {

  //Code to read data from Temp and Humidity Sensor and send it to cloud 
   
   float humid = dht_temp.readHumidity();
   float temp = dht_temp.readTemperature();
   //delay(16000);
   ThingSpeak.writeField(myChannelNumber, 2, humid, myWriteAPIKey);
   delay(16000);
   ThingSpeak.writeField(myChannelNumber, 4,temp , myWriteAPIKey); 
   delay(16000);

   float co_raw=analogRead(A0); // Connect Carbon Monoxide Sensor and get raw data ---> later convert to PPM 
   ThingSpeak.writeField(myChannelNumber, 3,co_raw , myWriteAPIKey); 
   delay(16000);
  // End of code to send data to Cloud 
   
  // Code to read data from Dust Sensor and Update it in Cloud 
  
  duration = pulseIn(pin, LOW);
  lowpulseoccupancy += duration;
  endtime = millis();
  if ((endtime-starttime) > sampletime_ms)
  {
    ratio = (lowpulseoccupancy-endtime+starttime + sampletime_ms)/(sampletime_ms*10.0);  // Integer percentage 0=>100
    concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve
    Serial.print("lowpulseoccupancy:");
    Serial.print(lowpulseoccupancy);
    Serial.print("    ratio:");
    Serial.print(ratio);
    Serial.print("    DSM501A:");
    Serial.println(concentration);
    ThingSpeak.writeField(myChannelNumber, 1, concentration, myWriteAPIKey);   // Need a formula to convert concentration into PM 2.5 and PM 10
    lowpulseoccupancy = 0;
    starttime = millis();
  } 
delay(16000);
  // End of Code to read data from Dust Sensor and Put it in Cloud 


  // Read data from Carbon Monoxide sensor 
 
 
 
}
