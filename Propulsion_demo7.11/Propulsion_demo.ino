#include <Servo.h>  
  
Servo myServo; // 创建一个Servo对象  
  
String comStr = ""; // 定义一个空字符串用于接收串口数据  
int comInt = 0; // 定义一个整型变量用于存储转换后的整数  
  
void setup() {  
  // 在这里编写设置代码，仅运行一次：  
  Serial.begin(9600); // 启动串口通信，设置波特率为9600  
  while(Serial.read() >= 0) {} // 清除串口缓冲区中的任何残留数据  
    
  myServo.attach(9); // 将myServo连接到数字引脚9（GPIO_09）  
  myServo.writeMicroseconds(1500);   
}  
  
void loop() {  
：  
  if (Serial.available() > 0){  
   
    delay(10); // 短暂延时，确保数据接收完整  
    comStr = Serial.readString(); 
    comInt = comStr.toInt(); // 将字符串转换为整数  
    comInt = constrain(comInt, 1000, 2000); // 限制整数范围在1000到2000之间  
  
    Serial.println(comInt);  
    myServo.writeMicroseconds(comInt); 
      
  }  
    
}





