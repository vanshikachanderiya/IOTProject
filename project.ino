#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
// You should get Auth Token in the Blynk App.
// BLYNK
char auth[] = "yMxUdSQprm6Yt2txtLXAOWgtFFfUBnW3";

// WIFI
// Set password to "" for open networks.
char ssid[] = "vinci";
char pass[] = "vanshika07";
BlynkTimer timer;

/* DHT11*/
//#include "DHT.h"

#define DHTTYPE DHT11 
#define DHTPIN D1 
#define soilMoisterPin D7
#define ptr A0
#define LED D4

DHT dht(DHTPIN, DHTTYPE);
//Define variables
float hum = 0;
float temp = 0;
int soilMoister = 0;
int lum =0;
//BlynkTimer timer;
//define treshold for variables

int seuil_lum = 200;
bool led_ON = false;
//declare the widgets 

//WidgetLED led1(V4);
WidgetLED led(V4);
led.off();
led.on();

void setup()
{
  //Soil moisture
  pinMode(soilMoisterPin,OUTPUT);
  //DHT
  pinMode(DHTPIN,INPUT);
  //LDR
  pinMode(ptr,OUTPUT);
  pinMode(LED, OUTPUT);
  Serial.begin(115200); // See the connection status in Serial Monitor
  delay(10);
  //timer.setInterval(1000L, myTimerEvent);
   //BLYNK
  Blynk.begin(auth, ssid, pass); // Connexion to Blynk 
  dht.begin();
  timer.setInterval(1000L,getData);
}
void loop()
{
  
  Blynk.run(); // Initiates Blynk
  timer.run(); // Initiates SimpleTimer
}

void getData()
{
  Serial.println("analysis started");
  getDhtData();
  getSoilMoisterData();
  ProcessingLights();
}
void getDhtData()
{
    // Reading temperature and humidity
  float hum = dht.readHumidity();
  // Read temperature as Celsius
  float temp = dht.readTemperature();
  
  // Display data
  Serial.print("Humidity: ");
  Serial.print(hum);
  Blynk.virtualWrite(V2, hum);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" *C ");
  //Blynk.virtualWrite(V2, hum);
  Blynk.virtualWrite(V0, temp);
  Serial.print("\t");
  
  // Wait a few seconds between measurements.
  delay(2000);
}

void getSoilMoisterData(void)
{ 
  soilMoister = digitalRead(soilMoisterPin);
  Serial.println("Soil moisture measured:");
  Serial.println(soilMoister);
  soilMoister = (soilMoister *100) /1024;
  Blynk.virtualWrite(V3, soilMoister); 
  lum = analogRead(A0);
  //lum = (lum*100)/1024;
  Serial.println("Light measured:");
  Serial.println(lum);
  Blynk.virtualWrite(V1,lum);  
}
void ProcessingLights()
{
  if(lum < seuil_lum && (led_ON == false ))
  {
    Serial.println("LEDs ON");
    digitalWrite(LED, HIGH);
    // turn the LED on (HIGH is the voltage level)
    delay(1000);                    
    //Blynk.setProperty(V4,"color");
  }
  else if(lum < seuil_lum && (led_ON == true))
  {
    Serial.println("LEDs already ON");
  }
  else
  {
    Serial.println("LEDs OFF");
    digitalWrite(LED, LOW); 
    //Blynk.setProperty(V4,"color");
    //digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);                       // wait for a second

  }
  delay(200);
}
