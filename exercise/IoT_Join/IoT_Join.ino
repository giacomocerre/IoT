/*
 * La scheda WEMOS si associa ad un AP, quello di casa o quello realizzato dal vostro
 * smartphone come hotspot (tethering). Una volta connesso continua a lampeggiare.
 * E' possibile utilizzare il codice del server del programma precedente per
 * avere lo stesso server HTTP/TCP.
 */
#include "secret.h"
#include <ESP8266WiFi.h>
#define LED 2

/*
 * Riutilizzeremo questa nei prossimi esercizi: realizza il join
 * all'AP le cui credenziali sono registrate nel file secret.h.
 */
void joinAP() {
  WiFi.mode(WIFI_STA);                       // Configura come stazione il dispositivo
  Serial.print("Mi sto connettendo");
  if ( ! WiFi.begin(ESSID, PASSWORD) ) {     // Verifica l'esistenza dell'AP
    Serial.println("ESSID inesistente: addio!");
    while ( true ) {};
  }
  while (WiFi.status() != WL_CONNECTED)      // Attende il successo del JOIN 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nConnesso come ");         // Visualizza il proprio IP
  Serial.println(WiFi.localIP());
}

// Qui viene richiamata la funzione di join
void setup() {
  pinMode(LED, OUTPUT); 
  Serial.begin(115200);
  joinAP();
}

/*
 * Nessuna funzionalita' interessante nel loop, ma e' possibile sostituirlo
 * con quello dell'esercizio precedente, creando prima l'oggetto Server TCP
 * ed inizializzandolo nella funzione "setup"
 */
void loop() {
  digitalWrite(LED,!(digitalRead(LED)));
  delay(500);
}
