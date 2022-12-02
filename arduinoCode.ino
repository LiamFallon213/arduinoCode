#include <TimeLib.h>
#include <Dictionary.h>
#include <SoftwareSerial.h>
#define _DICT_CRC_16

// globals
SoftwareSerial BTSerial(10, 11); // RX | TX
//String JSON = "{\"108\":\"left\",\"114\": \"right\",\"117\": \"up\",\"100\": \"down\"}"
Dictionary& controlMap = *(new Dictionary());



void writeEngine(int input);
void writeServo(int input);

class Command {
  private:
    int portNumber;
    int value;
    int type;
    time_t timeStamp;

  public:

    Command(int portNumberIn, int valueIn, String commandTypeIn) {
      portNumberIn = portNumber;
      valueIn = value;
      commandTypeIn = type;
      timeStamp = now();
    }

    int getPN() {
      return portNumber;
    };

    void setPN(int input) {
      portNumber = input;
    };

    int getValue() {
      return value;
    };

    void setValue(int input) {
      value = input;
    };

    time_t getTimeStamp() {
      return timeStamp;
    };

    int getType() {
      return type;
    };

    void setType(int input) {
      type = input;
    };
};

class Component {
  private:
    int value;
    int pin;
    bool active;

  public:
    int getValue() {
      return value;
    };
    int setValue(int val) {
      value = val;
    };
    int getPin() {
      return pin;
    };

    void setPin(int val) {
      pin = val;
    };

    // Takes in command, which is a pointer to a Command object
    void execute(Command* command) {
      switch (command->getType()) {

        case 1: writeEngine(command->getValue());//engine speed command
          break;

        case 2: writeServo(command->getType()); // Servo angle control
          break;

      }
    };
};

class Engine: public Component {
  public:
    int angle;
    int enginePosition;
};

class ServoClass: public Component {
  public:
    int angle;
};

void handleEngines();

Command makeCommand(String input) {
  String dir = controlMap[input];
  int inputInt = input.toInt();
  if (dir == "left") {
    handleEngines();
  } else if (dir == "right") {
    handleEngines();
  } else if (dir == "up") {
    handleEngines();
  } else if (dir == "down") {
    handleEngines();
  }
  Serial.println(dir);
}

void handleEngines(){
  Serial.println("Handle Engines");
}

float phSensor(){
 int analogIn = analogRead(0);
 float voltage = ((float)analogIn / 1023)*5;
 float PH = (-3/0.52)*voltage+21.47;
 return voltage;
}



void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);
  controlMap.jload("{\"108\":\"left\",\"114\": \"right\",\"117\": \"up\",\"100\": \"down\"}");
  //  Serial.println(controlMap.json());
};

void loop() {
  Serial.println(phSensor());
  if (BTSerial.available()) { // read from HC-05 and send to Arduino Serial Monitor
    String BTInput;
    BTInput += BTSerial.read();
  //  Serial.println(BTInput);
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
