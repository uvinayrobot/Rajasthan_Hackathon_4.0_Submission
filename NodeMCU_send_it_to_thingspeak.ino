/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on NodeMCU.

  Note: This requires ESP8266 support package:
    https://github.com/esp8266/Arduino

  Please be sure to select the right NodeMCU module
  in the Tools -> Board menu!

  For advanced settings please follow ESP examples :
   - ESP8266_Standalone_Manual_IP.ino
   - ESP8266_Standalone_SmartConfig.ino
   - ESP8266_Standalone_SSL.ino

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include "DHT.h"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Ticker.h>
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "ace07b36d4204d01937e65040fce3bec";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "iPhone";
char pass[] = "varma1223";
#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN,DHTTYPE);
BlynkTimer timer; 

float latitude;
float longitude;

// Declare Variables below 

// The Ticker/flipper routine
Ticker flipper;

//  VARIABLES
int blinkPin = 15;                // pin to blink led at each beat
int fadePin = 12;                 // pin to do fancy classy fading blink at each beat
int fadeRate = 0;                 // used to fade LED on with PWM on fadePin


// these variables are volatile because they are used during the interrupt service routine!
volatile int BPM;                   // used to hold the pulse rate
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // holds the time between beats, must be seeded! 
volatile boolean Pulse = false;     // true when pulse wave is high, false when it's low
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.

int i=0;



BLYNK_WRITE(V7) {
  GpsParam gps(param);

  // Print 6 decimal places for Lat, Lon
  Serial.print("Lat: ");
  Serial.println(gps.getLat(), 7);
  
  //latitude=gps.getLat();
 // longitude=gps.getLon();
  Serial.print("Lon: ");
  Serial.println(gps.getLon(), 7);

  Blynk.virtualWrite(V2, gps.getLat());
  delay(16000);
  Blynk.virtualWrite(V3, gps.getLon());

  i=10;

  // Print 2 decimal places for Alt, Speed
  Serial.print("Altitute: ");
  Serial.println(gps.getAltitude(), 2);

  Serial.print("Speed: ");
  Serial.println(gps.getSpeed(), 2);

  Serial.println();
 // Blynk.virtualWrite(V3, gps.getLat());
 // Blynk.virtualWrite(V4, gps.getLon());
}



void sendSensor()
{
  //float h = 100;//dht.readHumidity();
  //float t = 150;//dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
  //int hr_data=analogRead(A0);
  if(i==10)
  {
  interruptSetup();  // Call this every time after 2 min  
  }
  i=0;
  Serial.print("BPM Value is ");
  Serial.println(BPM);
  Serial.print("IBI Value is ");
  Serial.println(IBI);
  // Call another function read data from PM 2.5and PM 10 Sensors 
  // Call another function to read data from Carbon Monooxide Sensor 
 // if (isnan(h) || isnan(t)) {
 //   Serial.println("Failed to read from DHT sensor!");
 //   return;
 // }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
 // Blynk.virtualWrite(V1, h);
 // Blynk.virtualWrite(V0, t);
  
  //delay(3000);
 // Blynk.virtualWrite(V4, longitude);
  Blynk.virtualWrite(V0,BPM);
  Blynk.virtualWrite(V1,IBI);
  Blynk.virtualWrite(V15,1,28,74);
}


void setup()
{
  // Debug console
  Serial.begin(9600);
  
  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 8442);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8442);
  dht.begin();
  Serial.println("Waiting for connections...");
  timer.setInterval(1000L, sendSensor);
 // timer.setInterval(1000L, sendSensor1);
// interruptSetup(); 
}



void loop()
{
  Blynk.run();
 //Serial.println("In main loop"); 
  timer.run();
 // Serial.println("Here i am also")
}


