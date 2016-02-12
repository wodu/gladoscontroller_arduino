/*************************************************** 
 Glados lamp servo controller with XBOX360 wireless
 controller receiver.

 Based on example sketch for the Xbox Wireless Reciver library
 developed by Kristian Lauszus
 
 GNU General Public License, Wojciech Dubowik <klute99@gmail.com>
 ****************************************************/

#include <XBOXRECV.h>
#include <Servo.h>

#define DBG_SER
#define MIN_MOVEMENT 7500

USB Usb;
XBOXRECV Xbox(&Usb);
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

#define SERVO1_MIN 0
#define SERVO1_INIT 45
#define SERVO1_MAX 90

#define SERVO2_MIN 0
#define SERVO2_INIT 45
#define SERVO2_MAX 90

#define SERVO3_MIN 0
#define SERVO3_INIT 45
#define SERVO3_MAX 90

#define SERVO4_MIN 0
#define SERVO4_INIT 45
#define SERVO4_MAX 90


int i = 0;
void setup() {
  #ifdef DBG_SER
  Serial.begin(115200);
  while (!Serial);
  #endif

  if (Usb.Init() == -1) {
    #ifdef DBG_SER
    Serial.print(F("\r\nOSC did not start"));
    #endif
    while (1); //halt
  }
  #ifdef DBG_SER
  Serial.print(F("\r\nXbox Wireless Receiver Library Started"));
  #endif
  servo1.attach(2); //HEAD
  
  servo2.attach(3); //NECK
  
  servo3.attach(4); //PIVOT
  
  servo4.attach(5); //BODY
#if 0 //TODO: Specify limits first
  servo1.write(SERVO1_INIT);
  servo2.write(SERVO2_INIT);
  servo3.write(SERVO3_INIT);
  servo4.write(SERVO4_INIT);
#endif
}


void loop() {
  int leftx;
  int lefty;
  int rightx;
  int righty;
  int servo_pos1;
  int servo_pos2;
  int servo_pos3;
  int servo_pos4;
  
  Usb.Task();
  if (Xbox.XboxReceiverConnected) {
      if (Xbox.Xbox360Connected[i]) {
        leftx = Xbox.getAnalogHat(LeftHatX, i);
        lefty = Xbox.getAnalogHat(LeftHatY, i);
        rightx = Xbox.getAnalogHat(RightHatX, i);
        righty = Xbox.getAnalogHat(RightHatY, i);

        if (Xbox.getButtonClick(START, i)) {
          servo1.write(SERVO1_INIT);
          servo2.write(SERVO2_INIT);
          servo3.write(SERVO3_INIT);
          servo4.write(SERVO4_INIT);
        }

        if (abs(leftx) > MIN_MOVEMENT || abs(lefty) > MIN_MOVEMENT || abs(rightx) > MIN_MOVEMENT || abs(righty) > MIN_MOVEMENT) {
          if (abs(leftx) > MIN_MOVEMENT) {
            servo_pos1 = map(leftx,-32768, 32767, SERVO1_MIN, SERVO1_MAX);
            servo1.write(servo_pos1);
            #ifdef DBG_SER
            Serial.print(F("LeftHatX: "));
            Serial.print(Xbox.getAnalogHat(LeftHatX, i));
            Serial.print("\t");
            Serial.print(F("Servo1: "));
            Serial.print(servo_pos1);
            Serial.print("\t");
            #endif
          }
          if (abs(lefty) > MIN_MOVEMENT) {
            servo_pos2 = map(leftx,-32768, 32767, SERVO2_MIN, SERVO2_MAX);
            servo2.write(servo_pos2);
            #ifdef DBG_SER
            Serial.print(F("LeftHatY: "));
            Serial.print(Xbox.getAnalogHat(LeftHatY, i));
            Serial.print("\t");
            Serial.print(F("Servo2: "));
            Serial.print(servo_pos2);
            Serial.print("\t");
            #endif
          }
          if (abs(rightx) > MIN_MOVEMENT) {
            servo_pos3 = map(leftx,-32768, 32767, SERVO3_MIN, SERVO3_MAX);
            servo3.write(servo_pos3);
            #ifdef DBG_SER
            Serial.print(F("RightHatX: "));
            Serial.print(Xbox.getAnalogHat(RightHatX, i));
            Serial.print("\t");
            Serial.print(F("Servo3: "));
            Serial.print(servo_pos3);
            Serial.print("\t");
            #endif
          }
          if (abs(righty) > MIN_MOVEMENT) {
            servo_pos4 = map(leftx,-32768, 32767, SERVO4_MIN, SERVO4_MAX);
            servo4.write(servo_pos4);
            #ifdef DBG_SER
            Serial.print(F("RightHatY: "));
            Serial.print(Xbox.getAnalogHat(RightHatY, i));
            Serial.print(F("Servo4: "));
            Serial.print(servo_pos4);
            Serial.print("\t");
            #endif
          }
          #ifdef DBG_SER
          Serial.println();
          #endif
      }
    }
  }
}
