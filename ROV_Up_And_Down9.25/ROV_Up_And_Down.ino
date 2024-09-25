
/*
#include <Servo.h>
#include <Wire.h>
#include "MS5837.h"
#include "PIDController.h"

MS5837 sensor;
Servo myServo1;
Servo myServo2;

const int servoPins[] = {9, 10}; 
const int numServos = sizeof(servoPins) / sizeof(servoPins[0]);

int newPosition1 = 1500; // 1号电机的初始位置
int newPosition2 = 1500; // 2号电机的初始位置

float setPointDepth = 0.0; // 初始目标深度
bool isMachineOn = false;  // 机器初始状态为关闭
PIDController depthController(1.0, 0.0, 0.05); // PID参数

void setup() {
  Serial.begin(9600);
  while (!Serial) {}
  Wire.begin();

  // 初始化压力传感器
  while (!sensor.init()) {
    Serial.println("Failed to initialize sensor!");
    delay(5000);
  }
  sensor.setModel(MS5837::MS5837_02BA);
  sensor.setFluidDensity(997);

  // 初始化舵机
  for (int i = 0; i < numServos; i++) {
    myServo1.attach(servoPins[i]);
    myServo2.attach(servoPins[i+1]); 
  }
  myServo1.writeMicroseconds(1500);
  myServo2.writeMicroseconds(1500);
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');

    if (command == "on") {
      isMachineOn = true;
      newPosition1 = newPosition2 = 1500; // 设置两个电机的位置为1500微秒
    } else if (command == "off") {
      isMachineOn = false;
      newPosition1 = newPosition2 = 1500; // 设置两个电机的位置为1500微秒
    } else {
      setPointDepth = command.toFloat();
    }
    while (Serial.available() > 0) {
      Serial.read();
    }
  }

  if (isMachineOn) {
    sensor.read();
    float currentDepth = sensor.depth();

    float adjustment = depthController.update(setPointDepth, currentDepth);
    newPosition1 = newPosition2 = 1500 + adjustment*200; // 电机最终输出

    newPosition1 = constrain(newPosition1, 1000, 2000);
    newPosition2 = constrain(newPosition2, 1000, 2000);

    Serial.print("newPosition1: ");
    Serial.print(newPosition1);
    Serial.print(", newPosition2: ");
    Serial.println(newPosition2);

    myServo1.writeMicroseconds(newPosition1);
    myServo2.writeMicroseconds(newPosition2);

    Serial.print("Machine State: ");
    Serial.println(isMachineOn ? "On" : "Off");
    Serial.print("Target Depth: ");
    Serial.print(setPointDepth, 2);
    Serial.print(" m, Current Depth: ");
    Serial.print(currentDepth, 2);
    Serial.print(", newPosition1: ");
    Serial.print(newPosition1);
    Serial.print(", newPosition2: ");
    Serial.println(newPosition2);
  }

  delay(100);
}

*/




#include <Servo.h>
#include <Wire.h>
#include "MS5837.h"
#include "PIDController.h"

MS5837 sensor;

// 定义舵机对象
Servo myServo1; // 1号电机
Servo myServo2; // 2号电机
Servo myServo3; // 3号电机
Servo myServo4; // 4号电机
Servo upServo;  // 上电机
Servo downServo; // 下电机

// 定义PID控制器参数
float Kp = 1.0, Ki = 0.0, Kd = 0.05;
PIDController depthController(Kp, Ki, Kd);

// 定义舵机位置变量
int servoPosition1 = 1500; // 1号电机位置
int servoPosition2 = 1500; // 2号电机位置
int servoPosition3 = 1500; // 3号电机位置
int servoPosition4 = 1500; // 4号电机位置
int upPosition = 1500;
int downPosition = 1500;

// 定义目标深度和当前深度
float setPointDepth = 0.0;
float currentDepth = 0.0;

// 初始化标志
bool isMachineOn = false;

void setup() {
  Serial.begin(9600);
  while (!Serial) {}
  Wire.begin();

  // 初始化压力传感器
  sensor.init();
  sensor.setModel(MS5837::MS5837_02BA);
  sensor.setFluidDensity(997);

  // 为舵机分配引脚
  myServo1.attach(9);
  myServo2.attach(10);
  myServo3.attach(11);
  myServo4.attach(6);
  upServo.attach(3);
  downServo.attach(5);

  // 初始化舵机位置
  myServo1.writeMicroseconds(1500);
  myServo2.writeMicroseconds(1500);
  myServo3.writeMicroseconds(1500);
  myServo4.writeMicroseconds(1500);
  upServo.writeMicroseconds(1500);
  downServo.writeMicroseconds(1500);
}

void loop() {
  if (Serial.available() > 0) {

    char command = Serial.read();
    switch (command) {
      case '1':
        // 初始化位置
        servoPosition1 = 1500;
        servoPosition2 = 1500;
        servoPosition3 = 1500;
        servoPosition4 = 1500;
        upPosition = 1500;
        downPosition = 1500;
        break;
      case '2':
        // 前进
        servoPosition1 = 1200;
        servoPosition2 = 1200;
        servoPosition3 = 1200;
        servoPosition4 = 1200;
        break;
      case '3':
        // 后退

        servoPosition1 = 1800;
        servoPosition2 = 1800;
        servoPosition3 = 1800;
        servoPosition4 = 1800;
        break;
      case '4':
        // 左移
        servoPosition1 = 1800;
        servoPosition2 = 1200;
        servoPosition3 = 1200;
        servoPosition4 = 1800;
        break;
      case '5':
        // 右移
        servoPosition1 = 1200;
        servoPosition2 = 1800;
        servoPosition3 = 1800;
        servoPosition4 = 1200;
        break;
      case '6':
        // 顺时针旋转
        servoPosition1 = 1200;
        servoPosition2 = 1800;
        servoPosition3 = 1800;
        servoPosition4 = 1200;
        break;
      case '7':
        // 逆时针旋转
        servoPosition1 = 1800;
        servoPosition2 = 1200;
        servoPosition3 = 1200;
        servoPosition4 = 1800;
        break;
      case 'o':
        // 特殊命令，用于设置目标深度或开启/关闭机器
        if (Serial.available() > 0) {
          String command2 = Serial.readStringUntil('\n');
          if (command2 == "on") {
            isMachineOn = true;
          } else if (command2 == "off") {
            isMachineOn = false;
          } else {
            setPointDepth = command2.toFloat();
          }
        }
        while (Serial.available() > 0) {
          Serial.read();
        }
        break;
      default:
        // 其他情况不改变位置
        break;
    }
  }

  // 应用舵机位置
  myServo1.writeMicroseconds(servoPosition1);
  myServo2.writeMicroseconds(servoPosition2);
  myServo3.writeMicroseconds(servoPosition3);
  myServo4.writeMicroseconds(servoPosition4);

  if (isMachineOn) {
    sensor.read();
    currentDepth = sensor.depth();

    float adjustment = depthController.update(setPointDepth, currentDepth);
    upPosition = 1500 + adjustment*100;
    downPosition = 1500 + adjustment*100;

    upPosition = constrain(upPosition, 1000, 2000);
    downPosition = constrain(downPosition, 1000, 2000);

    upServo.writeMicroseconds(upPosition);
    downServo.writeMicroseconds(downPosition);

    // 调试输出
    Serial.print("Up Position: ");
    Serial.print(upPosition);
    Serial.print(", Down Position: ");
    Serial.println(downPosition);
    Serial.print("Machine State: ");
    Serial.println(isMachineOn ? "On" : "Off");
    Serial.print("Target Depth: ");
    Serial.print(setPointDepth, 2);
    Serial.print(" m, Current Depth: ");
    Serial.print(currentDepth, 2);
    Serial.println();
  }

  delay(200); // 循环延迟
}


/*
#include <Servo.h>
#include <Wire.h>
#include "MS5837.h"
#include "PIDController.h"
#include <JY901.h>
MS5837 sensor;

// 定义舵机对象
Servo myServo1; // 1号电机
Servo myServo2; // 2号电机
Servo myServo3; // 3号电机
Servo myServo4; // 4号电机
Servo upServo;  // 上电机
Servo downServo; // 下电机

// 定义PID控制器参数
float Kp = 1.0, Ki = 0.0, Kd = 0.05;
PIDController depthController(Kp, Ki, Kd);

// 定义舵机位置变量
int servoPosition1 = 1500; // 1号电机位置
int servoPosition2 = 1500; // 2号电机位置
int servoPosition3 = 1500; // 3号电机位置
int servoPosition4 = 1500; // 4号电机位置
int upPosition = 1500;
int downPosition = 1500;

// 定义目标深度和当前深度
float setPointDepth = 0.0;
float currentDepth = 0.0;

// 初始化标志
bool isMachineOn = false;

void setup() {
  Serial.begin(9600);
  while (!Serial) {}
  Wire.begin();

  // 初始化压力传感器
  sensor.init();
  sensor.setModel(MS5837::MS5837_02BA);
  sensor.setFluidDensity(997);

  // 为舵机分配引脚
  myServo1.attach(9);
  myServo2.attach(10);
  myServo3.attach(11);
  myServo4.attach(6);
  upServo.attach(3);
  downServo.attach(5);

  // 初始化舵机位置
  myServo1.writeMicroseconds(1500);
  myServo2.writeMicroseconds(1500);
  myServo3.writeMicroseconds(1500);
  myServo4.writeMicroseconds(1500);
  upServo.writeMicroseconds(1500);
  downServo.writeMicroseconds(1500);
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();
    switch (command) {
      case '1':
        // 初始化位置
        servoPosition1 = 1500;
        servoPosition2 = 1500;
        servoPosition3 = 1500;
        servoPosition4 = 1500;
        upPosition = 1500;
        downPosition = 1500;
        break;
      case '2':
        // 前进
        servoPosition1 = 1200;
        servoPosition2 = 1200;
        servoPosition3 = 1200;
        servoPosition4 = 1200;
        break;
      case '3':
        // 后退
        servoPosition1 = 1800;
        servoPosition2 = 1800;
        servoPosition3 = 1800;
        servoPosition4 = 1800;
        break;
      case '4':
        // 左移
        servoPosition1 = 1800;
        servoPosition2 = 1200;
        servoPosition3 = 1200;
        servoPosition4 = 1800;
        break;
      case '5':
        // 右移
        servoPosition1 = 1200;
        servoPosition2 = 1800;
        servoPosition3 = 1800;
        servoPosition4 = 1200;
        break;
      case '6':
        // 顺时针旋转
        servoPosition1 = 1200;
        servoPosition2 = 1800;
        servoPosition3 = 1800;
        servoPosition4 = 1200;
        break;
      case '7':
        // 逆时针旋转
        servoPosition1 = 1800;
        servoPosition2 = 1200;
        servoPosition3 = 1200;
        servoPosition4 = 1800;
        break;
      case 'o':
        // 特殊命令，用于设置目标深度或开启/关闭机器
        if (Serial.available() > 0) {
          String command2 = Serial.readStringUntil('\n');
          if (command2 == "on") {
            isMachineOn = true;
          } else if (command2 == "off") {
            isMachineOn = false;
          } else {
            setPointDepth = command2.toFloat();
          }
        }
        while (Serial.available() > 0) {
          Serial.read();
        }
        break;
      default:
        // 其他情况不改变位置
        break;
    }
  }

  // 应用舵机位置
  myServo1.writeMicroseconds(servoPosition1);
  myServo2.writeMicroseconds(servoPosition2);
  myServo3.writeMicroseconds(servoPosition3);
  myServo4.writeMicroseconds(servoPosition4);

  if (isMachineOn) {
    sensor.read();
    currentDepth = sensor.depth();

    float adjustment = depthController.update(setPointDepth, currentDepth);
    upPosition = 1500 + adjustment;
    downPosition = 1500 - adjustment;

    upPosition = constrain(upPosition, 1000, 2000);
    downPosition = constrain(downPosition, 1000, 2000);

    upServo.writeMicroseconds(upPosition);
    downServo.writeMicroseconds(downPosition);

    // 调试输出
        Serial.print("Up Position: ");
    Serial.print(upPosition);
    Serial.print(", Down Position: ");
    Serial.println(downPosition);
    Serial.print("Machine State: ");
    Serial.println(isMachineOn ? "On" : "Off");
    Serial.print("Target Depth: ");
    Serial.print(setPointDepth, 2);
    Serial.print(" m, Current Depth: ");
    Serial.print(currentDepth, 2);
    Serial.println();
  }

  Serial.print("Angle:");Serial.print((float)JY901.stcAngle.Angle[0]/32768*180);Serial.print(" ");Serial.print((float)JY901.stcAngle.Angle[1]/32768*180);Serial.print(" ");Serial.println((float)JY901.stcAngle.Angle[2]/32768*180);
  Serial.println("");
  delay(100); // 循环延迟
}

void serialEvent() 
{
  while (Serial.available()) 
  {
    JY901.CopeSerialData(Serial.read()); //Call JY901 data cope function
  }
}
//改最后这个函数，现在串口所有输入的东西都喂给这个函数了,会干扰其它串口

*/

