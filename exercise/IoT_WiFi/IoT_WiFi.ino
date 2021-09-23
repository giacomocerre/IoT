/*
 * Ora usiamo l'interfaccia WiFi del modulo per visualizzare alcuni 
 * dati dagli AP WiFi raggiungibili.
 * Per fare cio' utilizziamo una funzione di libreria specifica per
 * l'ESP8266 che costruisce e rende accessibili un array contenente
 * tali dati. Ogni elemento dell'array viene poi letto e stampato. 
 */
#include "ESP8266WiFi.h"
#define LED 2 // Il LED sulla scheda e' collegato al pin 2

void setup() {
  Serial.begin(115200);   // Questo consente di visualizzare sul PC
  pinMode(LED,OUTPUT);    // Questo LED e' acceso durante la ricerca
  digitalWrite(LED,HIGH); // Inizialmente spento (HIGH -> spento)
  WiFi.mode(WIFI_STA);    // L'interfaccia e' configurata come stazione
}

void loop() {
  char line[80];                      // buffer delle righe di stampa
  digitalWrite(LED,LOW);              // LED acceso
  int n = WiFi.scanNetworks();        // Scansione
  while (! WiFi.scanComplete() ) { }; // Attendo che abbia terminato
  digitalWrite(LED,HIGH);             // LED spento
  if (n == 0) {                       // n = numero di AP trovati
    Serial.println("no networks found");
  } else {
// Stampa dei dati raccolti, 
    sprintf(line,"%d networks found\n", n);
    Serial.print(line);
// Ciclo, per ogni AP trovato (0..n-1)  
    for (int i = 0; i < n; ++i) {
      // Preparazione della riga di buffer
      sprintf(line,"%02d: %30s | sig=%02dDb | ch=%02d\n",
        i+1,                  // posizione (1..n) dell'AP
        WiFi.SSID(i).c_str(), // SSID dell'i-esimo AP trovato
        WiFi.RSSI(i),         // potenza dell'i-esimo AP trovato
        WiFi.channel(i));     // canale dell'i-esimo AP trovato
      // Stampa della riga di buffer
      Serial.print(line);
    }
  }
  Serial.println();
  delay(5000);  // Attendo 5 secondi prima di ripetere
}
