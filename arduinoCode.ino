#include <TimeLib.h>
#include <Dictionary.h>
#include <SoftwareSerial.h>
#include <AFMotor.h>
#include <Servo.h> 
#define _DICT_CRC_16

#include <AFMotor.h>
AF_DCMotor leftMotorOBJ(4, MOTOR12_64KHZ);
AF_DCMotor rightMotorOBJ(3, MOTOR12_64KHZ);

// globals
SoftwareSerial BTSerial(11, 10); // RX | TX
//String JSON = "{\"108\":\"left\",\"114\": \"right\",\"117\": \"up\",\"100\": \"down\",\"85\": \"servoUp\",\"68\": \"servoDown\"}"
Dictionary& controlMap = *(new Dictionary());


void writeEngine(int input);
void writeServo(int input);


class Engine {
  public:
    int spd;
    const static int staticTurningSpeed = 200;
    int turnSpeed;
    String dir;
    AF_DCMotor* motorPointer;
    Engine(AF_DCMotor* motorIn) {
      motorPointer = motorIn;
      spd = 0;
      
      turnSpeed = 0;
      dir = "FOWARD";
    }

    _update() {
      int tempSpd = spd + turnSpeed;
      if (tempSpd > 0) {
        motorPointer->run(FORWARD);
      } else if (tempSpd == 0) {
        motorPointer->run(RELEASE);
      } else if (tempSpd < 0) {
        motorPointer->run(BACKWARD);
      }
      motorPointer->setSpeed(abs(tempSpd));
      //    Serial.println(spd);
      Serial.println(abs(spd));
    }
};

Engine leftMotor = Engine(&leftMotorOBJ);

Engine rightMotor = Engine(&rightMotorOBJ);

class ServoClass {
  public:
    int angle;
};

void left();
void right();
void up();
void down();
void _stop();
void servoUp();
void servoDown();
Servo myservo;
void makeCommand(String input) {
  String dir = controlMap[input];
  int inputInt = input.toInt();
  if (dir == "left") {
    left();
  } else if (dir == "right") {
    right();
  } else if (dir == "up") {
    up();
  } else if (dir == "down") {
    down();
  } else if (dir == "stop") {
    _stop();
  } else if (dir == "servoUp") {
   myservo.write(150);
  } else if (dir == "servoDown") {
   myservo.write(60);
  }
}

void servoUp() {

}

void left() {
  //  Serial.println("left()");
  leftMotor.spd = 0;
  rightMotor.spd = 0;
  leftMotor.turnSpeed = (-leftMotor.staticTurningSpeed);
  rightMotor.turnSpeed = rightMotor.staticTurningSpeed;
}

void right() {
  //Serial.println("right()");
  leftMotor.spd = 0;
  rightMotor.spd = 0;
  leftMotor.turnSpeed = leftMotor.staticTurningSpeed;
  rightMotor.turnSpeed = (-rightMotor.staticTurningSpeed);
}

void up() {
  leftMotor.turnSpeed = 0;
  rightMotor.turnSpeed = 0;
  leftMotor.spd += 70;
  rightMotor.spd += 70;
  if (leftMotor.spd > 255) leftMotor.spd = 255;
  if (rightMotor.spd > 255) rightMotor.spd = 255;
}

void down() {
  // Serial.println("down()");
    leftMotor.turnSpeed = 0;
  rightMotor.turnSpeed = 0;
  leftMotor.spd -= 70;
  rightMotor.spd -= 70;
  if (leftMotor.spd < -255) leftMotor.spd = -255;
  if (rightMotor.spd < -255) rightMotor.spd = -255;
}

void _stop() {
  leftMotor.spd = 0;
  rightMotor.spd = 0;
  leftMotor.turnSpeed = 0;
  rightMotor.turnSpeed = 0;
};

void _update_() {
  leftMotor._update();
  rightMotor._update();
};

float phSensor() {
  int analogIn = analogRead(0);
  float voltage = ((float)analogIn / 1023) * 5;
  float PH = (-3 / 0.52) * voltage + 22.47;
  return PH;
};



void setup() {
  
  Serial.begin(9600);
  BTSerial.begin(9600);
  controlMap.jload("{\"108\":\"left\",\"114\": \"right\",\"117\": \"up\",\"100\": \"down\",\"85\": \"servoUp\",\"68\": \"servoDown\"}");
  //  Serial.println(controlMap.json());
  myservo.attach(10); 
  myservo.write(60);
};

void loop() {
  _update_();
  BTSerial.println((float)phSensor());
  if (BTSerial.available()) { // read from HC-05 and send to Arduino Serial Monitor
    String BTInput;
    BTInput += BTSerial.read();
    makeCommand(BTInput);
  }

  if (Serial.available())     // Keep reading from Arduino Serial Monitor and send to HC-05
    BTSerial.write(Serial.read());;
};

void writeEngine(int input) {
  //TODO: Write code to handle changes to engines
};

void writeServo(int input) {
  //TODO: Write code to handle changes to servos
};
