#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define CLIENT_ID "boiler"

#define MQTT_USER "your_user"
#define MQTT_PASSWORD "your_password"

#define RELAY_PIN D1

// the IP address for the shield:
IPAddress ip(192, 168, 1, 200); 
IPAddress gateway(192, 168, 1, 1); 
IPAddress subnet(255, 255, 255, 0); 

const char* ssid = "your_router_SSID";
const char* password = "wifi_password";
const char* mqtt_server = "mqtt_broker_ip";

WiFiClient espClient;
PubSubClient client(espClient);

bool value = false;

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid); //We don't want the ESP to act as an AP
  WiFi.config(ip, gateway, subnet);
  WiFi.mode(WIFI_STA); 
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void updateStatus(char* payload) {
  if (payload[0] == 'O' && payload[1] == 'N') {
    value = true;
  } else {
    value = false;
  }
  digitalWrite(RELAY_PIN, value);
}

void broadcastStatus() {
  char* payload = "OFF";
  if (value) {
    payload = "ON";
  }
  client.publish("stat/"CLIENT_ID"/POWER", payload);
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  // Allocate the correct amount of memory for the payload copy
  char* payload_data = (char*)malloc(length);
  // Copy the payload to the new buffer
  memcpy(payload_data, payload, length);
  Serial.print(payload_data);
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  updateStatus(payload_data);
  // Free the memory
  free(payload_data);
  broadcastStatus();

}

void reconnect() {
  // Loop until we're reconnected
  digitalWrite(LED_BUILTIN, LOW);
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(CLIENT_ID, MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("connected");
      client.subscribe("cmnd/"CLIENT_ID"/POWER");
      digitalWrite(LED_BUILTIN, HIGH);
      broadcastStatus();
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  Serial.begin(115200);
  setup_wifi(); 
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  digitalWrite(RELAY_PIN, false);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
