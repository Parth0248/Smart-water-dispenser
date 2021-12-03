#include "HTTPClient.h"
#include "time.h"
#include <ArduinoJson.h>
#include "WiFi.h"
#include "ThingSpeak.h"
#include "PubSubClient.h"

#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

const int trigPin = 5;
const int echoPin = 19;
const int trigPin2 = 26;
const int echoPin2 = 25;

const int pump_pin = 23;

char *ssid = "JIO248_4G";
char *pwd = "18150110";
String cse_ip = "192.168.29.107";
String cse_port = "8080";
String server = "http://" + cse_ip + ":" + cse_port + "/~/in-cse/in-name/";
String ae = "Smart-water-despenser";
String cnt = "water-level";
String cnt1 = "when-used";
String cnt2 = "when-refilled";

const char* server_mqtt = "mqtt.thingspeak.com";
char mqttUserName[] = "mwa0000024808135";
char mqttPass[] = "ZCHQ2BFT4ZC6O7DO";
int writeChannelID = 1594104;
char writeAPIKey[] = "G2CTKVPJHQFJVFGC";
WiFiClient wifiClient;
PubSubClient mqttClient(server_mqtt, 1883, wifiClient);

float duration;
float duration2;
float distanceCm;
float distanceCm2;

void createCI(String& val)
{
    HTTPClient http;
    http.begin(server + ae + "/" + cnt + "/");
    http.addHeader("X-M2M-Origin", "admin:admin");
    http.addHeader("Content-Type", "application/json;ty=4");
    int code = http.POST("{\"m2m:cin\": {\"cnf\":\"application/json\",\"con\": " + String(val) + "}}");
    Serial.println(code);
    if (code == -1)
        Serial.println("Connection to server failed");
    http.end();
}
void createCI2(String& val)
{
    HTTPClient http;
    http.begin(server + ae + "/" + cnt1 + "/");
    http.addHeader("X-M2M-Origin", "admin:admin");
    http.addHeader("Content-Type", "application/json;ty=4");
    int code = http.POST("{\"m2m:cin\": {\"cnf\":\"application/json\",\"con\": " + String(val) + "}}");
    Serial.println(code);
    if (code == -1)
        Serial.println("Connection to server failed");
    http.end();
}

void setup() {
  pinMode(pump_pin, OUTPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
  digitalWrite(trigPin,LOW);
  pinMode(trigPin2, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin2, INPUT); // Sets the echoPin as an Input
  digitalWrite(trigPin2,LOW);
  
  WiFi.begin(ssid, pwd);
  while (WiFi.status() != WL_CONNECTED) 
  {
      delay(1000);
      Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  mqttClient.setServer(server_mqtt,1883);
}


void loop() {
  // put your main code here, to run repeatedly:
  int stat;

  const int water_threshold = 8;
  const int hand_detection = 15;
  
  while(!mqttClient.connected()){
      Serial.println("Connecting to mqtt");
      mqttClient.connect("", mqttUserName, mqttPass);
      
      if(!mqttClient.connected()){
        Serial.print("Error code :");
        Serial.println(mqttClient.state());
        Serial.println(WiFi.localIP());
        delay(5000);
      }
        
  }
  if(mqttClient.connected())
  {
    Serial.println("Connected to ThinkSpeak");
  }
  mqttClient.loop();

  
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  //Serial.println(duration);
  // Calculate the distance
  duration=duration/2;
  distanceCm = duration / 29.09;
  String val = String(distanceCm);
  createCI(val); // sending value to om2m
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2= pulseIn(echoPin2,HIGH);
  duration=duration2/2;
  distanceCm2= duration2*(0.0343);
  String val2= String(distanceCm2);
  createCI2(val2);
  int water_level = water_threshold - distanceCm;
  
  Serial.print("Water level : ");
  Serial.println(water_level);

  //Serial.print("");
  //Serial.println(distanceCm2);
  // ultrasonic

  if(distanceCm2 < 15)
  {
    Serial.println("Hand detected");
    
    if(water_level > 0){
      digitalWrite(pump_pin,1);
      Serial.println("PUMP ON");
      delay(1000);
      digitalWrite(pump_pin,0);
      delay(1000);
    }
    else 
      Serial.println("Insufficient water");  
  }
    
  String topicString = "channels/" + String(writeChannelID) + "/publish/" + String(writeAPIKey);
  String dataString = "field1=" + val;
  
  mqttClient.publish(topicString.c_str(),dataString.c_str());
  
  delay(3000);
  
}
