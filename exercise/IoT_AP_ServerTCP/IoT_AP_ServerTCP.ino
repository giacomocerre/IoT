/*
 * L'interfaccia WiFi si comporta ora da Access Point, e
 * mette a disposizione un server TCP. Il server riproduce
 * cio' che gli viene spedito sul monitor serial
 */
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#define LED 2
// Credenziali per l'Access Point (meglio cambiarle)
#define ESSID "Wemos_MIO"   // ESSID dell'AP
#define PASSWORD "qwerty123" // almeno 8 caratteri

// Creazione dell'oggetto server sulla porta 80 (HTTP)
WiFiServer TCPServer(80);

void setup() {
  delay(1000);
  Serial.begin(115200);        // Abilita il monitor
  pinMode(LED,OUTPUT);
// Configurazione dell'Access Point con essid e password
// Viene configurato anche un server DHCP nella rete 192.168.4.0/18
  if (! WiFi.softAP(ESSID,PASSWORD)) {
    Serial.println("Creazione dell'AP fallita");
    while (true) {}
  }
// Lettura e stampa sul monitor seriale dell'indirizzo IP
  Serial.print("\nIndirizzo IP dell'Access Point: ");
  Serial.println(WiFi.softAPIP());
// Attesa: connettersi all'AP ora
  digitalWrite(LED,LOW);
  delay(20000);
  digitalWrite(LED,HIGH);
//  Avvio del server TCP
  TCPServer.begin();
}

/*
 * Il loop gira a vuoto se non ci sono richieste in attesa. Quando un cliente
 * chiede la connessione il server riceve e visualizza (senza analizzarla)
 * la stringa inviata, e spedisce in risposta una semplice pagina HTML
 * utilizzando il protocollo HTTP
 */
void loop() {
  char line[80];
  WiFiClient client;  // Creazione oggetto cliente
  // Se non ci sono richieste chiude il loop 
  if ( ! ( client = TCPServer.available() ) ) return;
  digitalWrite(LED,LOW);              // Accende il LED durante l'attivita'
  Serial.print("Cliente connesso: "); // Visualizza i dati del socket lato cliente
  Serial.print(client.remoteIP());
  Serial.print(":");
  Serial.println(client.remotePort());
  /*
   * Lettura e visualizzazione di tutti i caratteri inviati
   * dal cliente (una REQUEST HTTP)
   */
  while (client.available()) Serial.write(client.read());
  // Invio della RESPONSE 
  client.print("HTTP/1.1 200 OK\r\n");          // startline
  client.print("\r\n");                         // riga vuota
  client.print("<html>\r\n");                   // TAG di apertura
  client.print("Hallo from you lazy AP!\r\n");  // Contenuto
  client.print("</html>\r\n");                  // TAG di chiusura
  client.stop();            // Chiusura della connessione
  digitalWrite(LED,HIGH);   // Spegnimento del LED
}
