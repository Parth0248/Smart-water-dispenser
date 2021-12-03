#include "HTTPClient.h"
#include "time.h"
#include <ArduinoJson.h>
#include "WiFi.h"

const int trigPin = 5;
const int echoPin = 19;
const int trigPin2 = 26;
const int echoPin2 = 25;
const int a=23;
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

char *ssid = "JIO248_4G";
char *pwd = "18150110";
String cse_ip = "192.168.29.107";
String cse_port = "8080";
String server = "http://" + cse_ip + ":" + cse_port + "/~/in-cse/in-name/";
String ae = "Smart-water-despenser";
String cnt = "water-level";
String cnt1 = "when-used";
String cnt2 = "when-refilled";

float duration;
float duration2;
float distanceCm;
float distanceCm2;
float distanceInch;

String SendHTML(void)
{
  // Code to send html
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0,user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; textalign: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color:#444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color:white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #1abc9c;}\n";
  ptr +=".button-on:active {background-color: #16a085;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>ESP32 with ThingSpeak Server</h1>\n";
  ptr +="<h3>Using Station(STA) Mode</h3>\n";
  ptr +="<h4>Control status For D1</h4>\n";
  ptr +="<a class=\"button button-on\"href=\"https://api.thingspeak.com/update?api_key=FB2AFYOY1BQTV43U&field1=1\">ON</a>\n";
  ptr +="<a class=\"button button-off\"href=\"https://api.thingspeak.com/update?api_key=FB2AFYOY1BQTV43U&field1=0\">OFF</a>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}

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
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
  digitalWrite(trigPin,LOW);
  pinMode(trigPin2, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin2, INPUT); // Sets the echoPin as an Input
  digitalWrite(trigPin2,LOW);
  pinMode(a,OUTPUT);
  WiFi.begin(ssid, pwd);
  while (WiFi.status() != WL_CONNECTED) 
  {
      delay(1000);
      Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void loop() {
  // Sets the trigPin on HIGH state for 10 micro seconds
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
  Serial.print(distanceCm);
  Serial.print(" ");
  Serial.println(distanceCm2);
  // ultrasonic
  delay(3000);
}