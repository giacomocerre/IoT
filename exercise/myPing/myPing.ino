#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "secret.h"
#include <ping.h>

#define TARGET "www.example.com" // destinazione del ping

int ritardo;
int flag;

void joinAP() {
  WiFi.mode(WIFI_STA);
  Serial.print("Mi sto connettendo");
  if ( ! WiFi.begin(MYSSID, MYPASSWD) ) {
    Serial.println("SSID inesistente: addio!");
    while ( true ) {};
  }
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connesso come ");
  Serial.println(WiFi.localIP());
}

void ping_recv_cb(void *opt, void *resp_ptr) {
  ping_resp* resp = (struct ping_resp*)resp_ptr;
  if (resp->ping_err == -1) ritardo=-1;
  else ritardo=resp->resp_time;
  flag=1;
}

void setup() { 
  IPAddress destination;
  Serial.begin(115200);
  joinAP();
}

void loop() {
  IPAddress destination;
  ping_option options;
  // Preparo la struttura per il PING
  memset(&options, 0, sizeof(struct ping_option));
  // Preparo il PING
  WiFi.hostByName(TARGET,destination);
  options.ip=destination;
  options.recv_function = ping_recv_cb;
  options.sent_function = NULL;
//  options.count=3;          // numero di tentativi
//  options.coarse_time=1;  // timeout (millisecondi)
  flag=0;
  ping_start(&options);
  while ( !flag ) { delay(10); }
  Serial.print("-> ");
  if (ritardo < 0) Serial.println("Ping perduto");
  else Serial.println(ritardo);
  delay(3000);
}
