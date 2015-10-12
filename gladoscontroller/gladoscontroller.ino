/*************************************************** 
 Glados lamp servo controller. It gets gyro settings
 from Android phone with jason over UDP on port 2362
 and translates gyro degress to servo pulses.

 TODO: Control fourth servo with input from sliding bar
 or accelerometer.
 
 MIT License, Wojciech Dubowik <klute99@gmail.com>
 ****************************************************/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>

char ssid[] = "glados";  //  your network SSID (name)
char pass[] = "controller";       // your network password


unsigned int localPort = 2362;      // local port to listen for UDP packets

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERV_MIN  200
#define SERV_MAX  550

const float serv_scale = 1;
const float serv2deg = (SERV_MAX - SERV_MIN) / 360.0 * serv_scale;

#define UDP_PACKET_SIZE 1200
byte packetBuffer[UDP_PACKET_SIZE];
WiFiUDP udp;



void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println("GlaDos lamp controller");

  Wire.begin(0, 2);

  pwm.begin();
  pwm.setPWMFreq(60);
#if 0
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
#else
  WiFi.softAP(ssid, pass);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
#endif
  Serial.println("Starting UDP");
  udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(udp.localPort());

  
}

void loop() {
  StaticJsonBuffer<200> jsonBuffer;
  int packetLength = udp.parsePacket();

  if (packetLength) {

    udp.read(packetBuffer, UDP_PACKET_SIZE);
    packetBuffer[packetLength + 1] = '\0';
    
    JsonObject& root = jsonBuffer.parseObject((char*)packetBuffer);
    if (!root.success()) {
      Serial.println("parseObject() failed");
    }
    float azimuth = root["gyro"][0];
    float pitch = root["gyro"][1];
    float roll = root["gyro"][2];

    float servo1 = SERV_MIN + ( azimuth + 180.0) * serv2deg;
    float servo2 = SERV_MIN + ( pitch + 180.0) * serv2deg;
    float servo3 = SERV_MIN + ( roll + 180.0) * serv2deg;
#if 0
    Serial.println("Gyro");
    Serial.println(azimuth, 3);
    Serial.println(pitch, 3);
    Serial.println(roll, 3);
    Serial.println("Servo");
    Serial.println(servo1, 3);
    Serial.println(servo2, 3);
    Serial.println(servo3, 3);
#endif
    pwm.setPWM(0, 0, servo1);
    pwm.setPWM(1, 0, servo2);
    pwm.setPWM(2, 0, servo3);
  }
  delay(10);
}
