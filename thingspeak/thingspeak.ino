```#include "PubSubClient.h"
#include "WiFi.h"
#include "ThingSpeak.h"

const int trigPin = 5;
const int echoPin = 19;
const int a=23;
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

char *ssid = "JIO248_4G";
char *pwd = "18150110";
const char* server = "mqtt.thingspeak.com";
char mqttUserName[] = "mwa0000024808135";
char mqttPass[] = "ZCHQ2BFT4ZC6O7DO";
int writeChannelID = 1594104;
char writeAPIKey[] = "G2CTKVPJHQFJVFGC";
float duration;
float distanceCm;
float distanceInch;
WiFiClient wifiClient;
PubSubClient mqttClient(server, 1883, wifiClient);

void setup() {
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
  digitalWrite(trigPin,LOW);

  // put your setup code here, to run once:
  WiFi.begin(ssid, pwd);
  while (WiFi.status() != WL_CONNECTED){
    Serial.println("Connecting");
    delay(2000);
  }
  Serial.println("WiFi connected");
  
  mqttClient.setServer(server,1883);

  //while(!mqttClient.connect(server, mqttUserName, mqttPass)) delay(2000);
}

void loop() {
  int water_level;
  // put your main code here, to run repeatedly:
  int stat;
  
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
    Serial.println("Connected");
  }
  mqttClient.loop();
   // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  //Serial.println(duration);
  // Calculate the distance
  duration=duration/2;
  Serial.print("duration :");
  Serial.println(duration);
  distanceCm = duration / 29.09;
  Serial.print("distance cm");
  Serial.println(distanceCm);
  String val = String(distanceCm);
  Serial.print("val is:");
  Serial.println(val);
  //createCI(val); // sending value to om2m
  // ultrasonic
  //String val2 = String(rand());
  //createCI2(val2);

  
  String topicString = "channels/" + String(writeChannelID) + "/publish/" + String(writeAPIKey);
  String dataString = "field1=" + val;
  //String(water_level);
  
  mqttClient.publish(topicString.c_str(),dataString.c_str());
  delay(15000);
}```