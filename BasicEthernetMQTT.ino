#include <SPI.h>
#include <ESP8266WiFi.h>
#include <UIPEthernet.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress server(192, 168, 1, 134);

const char* topic = "";
String s = "";
String parsing = "";
String fix = "";

EthernetClient ethClient;
PubSubClient client(ethClient);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("EthernetClient")) {
      s = WiFi.macAddress();
      parsing = s.substring(9, 17);
      for (int i = 2; i < 7; i++) {
        parsing.remove(i, 1);
        i++;
      }
      fix = "r/" + parsing;
      Serial.println(s);
      topic = fix.c_str();
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", topic);
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void publishData() {
  if (client.connect("EthernetClient")) {
    s = WiFi.macAddress();
    parsing = s.substring(9, 17);

    for (int i = 2; i < 7; i++) {
      parsing.remove(i, 1);
      i++;
    }
    fix = "r/" + parsing;
    Serial.println(s);
    topic = fix.c_str();

    Serial.println("connected");
    // Once connected, publish an announcement...
    client.publish("outTopic", topic);
    // ... and resubscribe
    client.subscribe("inTopic");
  } else {
    Serial.print("failed, rc=");
    Serial.print(client.state());
    Serial.println(" try again in 5 seconds");
    // Wait 5 seconds before retrying
    delay(5000);
  }
}

void setup()
{
  Serial.begin(115200);
  client.setServer(server, 1883);
  client.setCallback(callback);
  Ethernet.begin(mac);
  // Allow the hardware to sort itself out
  delay(1500);
}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  publishData();
  client.loop();
}
