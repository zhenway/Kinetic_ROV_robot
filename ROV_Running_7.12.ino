//发1，整体电机初始化（开机和停机）
//发2，四个电机赋值1200，ROV前进
//发3，四个电机赋值1800，ROV后退


//左上为1号电机
//右上为2号电机
//左下为3号电机
//右下为4号电机



#include <Servo.h>

Servo myServo1; // 创建四个舵机对象
Servo myServo2;
Servo myServo3;
Servo myServo4;

int servoPosition = 0; // 定义一个整型变量用于存储舵机的位置

void setup() {
  Serial.begin(9600); // 启动串口通信，设置波特率为9600
  while (!Serial) { // 等待串口准备就绪
    delay(1);        // 等待串口初始化
  }

  // 将舵机分别连接到Arduino的数字引脚
  myServo1.attach(9);
  myServo2.attach(10);
  myServo3.attach(11);
  myServo4.attach(6);
}

void loop() {
  if (Serial.available() > 0) {
    // 读取串口数据
    char command = Serial.read();

    // 根据接收到的命令设置servoPosition的值
    switch (command) {
      case '1':
        servoPosition = 1500;
        break;
      case '2':
        servoPosition = 1200;
        break;
      case '3':
        servoPosition = 1800;
        break;
      default:
        // 如果接收到的不是1、2、3，不改变servoPosition的值
        break;
    }

    // 将servoPosition的值应用到四个舵机上
    myServo1.writeMicroseconds(servoPosition);
    myServo2.writeMicroseconds(servoPosition);
    myServo3.writeMicroseconds(servoPosition);
    myServo4.writeMicroseconds(servoPosition);

    // 清空串口缓冲区，为下一次读取做准备
    while (Serial.available() > 0) {
      Serial.read();
    }
  }
}


/*
#include <Servo.h>

Servo myServo1; // 创建四个电机对象
Servo myServo2;
Servo myServo3;
Servo myServo4;

int servoPosition1 = 1500; // 1号电机位置
int servoPosition2 = 1500; // 2号电机位置
int servoPosition3 = 1500; // 3号电机位置
int servoPosition4 = 1500; // 4号电机机位置

void setup() {
  Serial.begin(9600); // 启动串口通信，设置波特率为9600
  while (!Serial) { // 等待串口准备就绪
    delay(1);        // 等待串口初始化
  }

  // 将电机分别连接到Arduino的数字引脚
  myServo1.attach(9);
  myServo2.attach(10);
  myServo3.attach(11);
  myServo4.attach(6);
}

void loop() {
  if (Serial.available() > 0) {
    // 读取串口数据
    char command = Serial.read();

    switch (command) {
      case '1':
        // 整体电机初始化
        servoPosition1 = 1500;
        servoPosition2 = 1500;
        servoPosition3 = 1500;
        servoPosition4 = 1500;
        break;
      case '2':
        // ROV前进
        servoPosition1 = 1200;
        servoPosition2 = 1200;
        servoPosition3 = 1200;
        servoPosition4 = 1200;
        break;
      case '3':
        // ROV后退
        servoPosition1 = 1800;
        servoPosition2 = 1800;
        servoPosition3 = 1800;
        servoPosition4 = 1800;
        break;
      case '4':
        // 左移
        servoPosition1 = 1800;
        servoPosition4 = 1200;
        servoPosition2 = 1200;
        servoPosition3 = 1800;
        break;
      case '5':
        // 右移
        servoPosition1 = 1200;
        servoPosition4 = 1800;
        servoPosition2 = 1800;
        servoPosition3 = 1200;
        break;
      case '6':
        // 顺时针旋转
        servoPosition1 = 1200;
        servoPosition3 = 1800;
        servoPosition2 = 1200;
        servoPosition4 = 1800;
        break;
      case '7':
        // 逆时针旋转
        servoPosition1 = 1800;
        servoPosition3 = 1200;
        servoPosition2 = 1800;
        servoPosition4 = 1200;
        break;
      default:
        // 如果接收到的命令不是1-7，不改变电机转速
        break;
    }

    // 将电机位置应用到各个舵机上
    myServo1.writeMicroseconds(servoPosition1);
    myServo2.writeMicroseconds(servoPosition2);
    myServo3.writeMicroseconds(servoPosition3);
    myServo4.writeMicroseconds(servoPosition4);

    // 清空串口缓冲区，为下一次读取做准备
    while (Serial.available() > 0) {
      Serial.read();
    }
  }
}
*/