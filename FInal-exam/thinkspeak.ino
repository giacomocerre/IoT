#include <DHT.h>
#include <ESP8266WiFi.h>
#include "secret.h"
const char* server = "api.thingspeak.com";
//Pin D2 per data 
#define DHTPIN D2
#define LED 2
//definisco il tipo di DHT (DHT11 o DHT22)
#define DHTTYPE DHT22 
//inizializzi il sensore DHT
DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;
 
void setup() 
{
Serial.begin(115200);
pinMode(LED,OUTPUT);
delay(10);
dht.begin();
 
WiFi.begin(ssid, password);
 
Serial.println();
Serial.println();
Serial.print("Mi connetto a: ");
Serial.println(ssid);
 
WiFi.begin(ssid, password);
 
while (WiFi.status() != WL_CONNECTED) 
{
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("Collegamento WiFi effettuato!");
 
}
 
void loop() 
{
//accendo il LED a inizio lettura
 digitalWrite(LED,LOW); 
//leggo umidità 
float h = dht.readHumidity();
//leggo temperatura (se la voglio in Fahrenheit basta mettere come parametro true)
float t = dht.readTemperature();
//check di controllo sulle letture
if (isnan(h) || isnan(t)) 
{
Serial.println("Impossibile leggere dal sensore DHT22!!");
return;
}
 
if (client.connect(server,80)) {
//creo la stringa url da passare a thinkspeak per inviare i dati 
String postStr = apiKey;
postStr +="&field1=";
postStr += String(t);
postStr +="&field2=";
postStr += String(h);
postStr += "\r\n\r\n";
//esegue una POST per HTTP 1.1
client.print("POST /update HTTP/1.1\n");
//sull'host api.thingspeak.com
client.print("Host: api.thingspeak.com\n");
client.print("Connection: close\n");
//connessione a thingspeak con l'API
client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
client.print("Content-Type: application/x-www-form-urlencoded\n");
client.print("Content-Length: ");
//invio la stringa genereta prima a thinkspeak
client.print(postStr.length());
client.print("\n\n");
client.print(postStr);
 
Serial.print("Temperature: ");
Serial.print(t);
Serial.print(" °C \nHumidity   : ");
Serial.print(h);
Serial.print(" %");
Serial.println("\n *** Invio dati a Thingspeak ***");
}
client.stop();
Serial.print("Prossima lettura tra 20sec");
//spengo il LED a fine lettura
digitalWrite(LED,HIGH);
//aspetto 20 secondi che thinkspeak riceva, poi ricomincio!
delay(20000);
}
