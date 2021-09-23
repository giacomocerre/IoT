/*
 * Dweet.io e' un servizio di cloud orientato ad applicazioni IoT. E'
 * possibile inviare dati (numerici o stringhe) che vengono memorizzati
 * e sono successivamente disponibili tramite il servizio stesso.
 * Per questa funzionalita' di usa una API REST offerta dal servizio. Il
 * nostro dispositivo realizza un client HTTP che comunica al nostro
 * dweet (si chiama cosi' una istanza del servizio) gli interi positivi. Il nome del
 * dweet va inserito nel file secret.h: scegliete un nome che sia
 * unico. Il dweet e' aperto a tutti, in lettura e scrittura!
 */
#include "secret.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define LED 2        // Il LED sulla scheda e' collegato al pin 2

HTTPClient dweet;    // L'oggetto HTTPClient per comunicare con dweet.io 
int n=0;             // L'intero che sara' incrementato

void joinAP() {      // Funzione di join gia' vista nell'esercizio precedente
  WiFi.mode(WIFI_STA);
  Serial.print("Mi sto connettendo");
  if ( ! WiFi.begin(MYSSID, MYPASSWD) ) {
    Serial.println("SSID inesistente: addio!");
    while ( true ) {};
  }
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(WiFi.status());
    Serial.print("\n");
    Serial.print(WL_CONNECTED);
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connesso come ");
  Serial.println(WiFi.localIP());
}

void setup() {       // Setup gia' visto in precedenza
  pinMode(LED, OUTPUT); 
  Serial.begin(115200);
  joinAP();
}

/*
 * Nel loop viene inviata la REQUEST HTTP al server dweet.io. La parte
 * informativa della REQUEST, che e' di tipo GET, sta nella URL, nella 
 * parte detta di query. Dopo aver inviato la GET si attende la risposta
 * e poi si ripete il loop, incrementanto il valore inviato.
 */
void loop() {
  char url[100];  // Buffer per la URL di dweet.io
  char body[100]; // Buffer body HTTP delle REQUEST
  /* 
   *  Costruzione della URL
   *  L'ultimo elemento del path e' il nome del dweet di destinazione
   *  Dopo il ? segue la query che riporta l'identificatore di un campo 
   *  del dweet e il valore assegnato. Nel nostro caso abbiamo un unico
   *  campo "prova" ed il suo valore e' l'intero crescente
   */  
  sprintf(url,"http://dweet.io/dweet/for/%s?prova=%d",DWEET,n++);  
  Serial.print("URL: ");Serial.println(url);  // Visualizza la URL utilizzata
  digitalWrite(LED_BUILTIN,LOW);              // LED acceso durante l'attivita'
  // Apre la connessione HTTP con il server REST di dweet.io
  if (dweet.begin(url)) {                     // Apre la connessione
    int x=dweet.GET();                        // Esegue la GET, in x l'esito
    if(x == 200) 
      Serial.println("Channel update successful.");
    else 
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    dweet.end();                              // Chiude la connessione
  }
  digitalWrite(LED_BUILTIN,HIGH);             // Spegne il LED
  delay(2000);                                // Ritardo prima del prossimo feed
}
