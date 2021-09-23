#include <PubSubClient.h> //Knolleary
#include <ESP8266WiFi.h> //ESP8266WiFi.h
#include "./secret.h" //This is a personal file containing web credentials

char* topic = "channels/683687/publish/OGFS62IIGTXIF12T"; 
char* server = "mqtt.thingspeak.com";

WiFiClient wifiClient;
PubSubClient client(server, 1883, wifiClient);

void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT); 
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  Serial.print("Connecting");
  if ( ! WiFi.begin(MYSSID, MYPASSWD) ) {
    Serial.println("SSID unknown: stop");
    while ( true ) {};
  }
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected as ");
  Serial.println(WiFi.localIP());

  char clientName[]="ESP-Thingspeak";
  
  if (client.connect(clientName)) {
    Serial.println("Connected to MQTT broker");
    Serial.print("Topic");
    Serial.println(topic);
    
    if (client.publish(topic, "hello from ESP8266")) {
      Serial.println("Publish ok");
    }
    else {
      Serial.println("Publish failed");
    }
  }
  else {
    Serial.println("MQTT connect failed");
    Serial.println("Will reset and try again...");
    abort();
  }
}

void loop() {
  static int counter = 0;
  String payload="field1=";
  payload+=micros();
  payload+="&field2=";
  payload+=counter++;
  payload+="&status=MQTTPUBLISH";
  
  if (client.connected()){
    Serial.print("Sendingoad: ");
    Serial.println(payload);
    
    if (client.publish(topic, (char*) payload.c_str())) {
      Serial.println("Publish ok");
    }
    else {
      Serial.println("Publish failed");
    }
  }
  delay(20000);
}
