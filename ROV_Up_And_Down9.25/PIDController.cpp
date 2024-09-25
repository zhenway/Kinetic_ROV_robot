// PIDController.cpp
#include <Arduino.h> 
#include "PIDController.h"

float PIDController::update(float setPoint, float feedbackValue) {
    unsigned long currentTime = millis();
    float error = setPoint - feedbackValue;
    float deltaTime = (currentTime - lastTime) / 1000.0;
    if (deltaTime > 0) {  // 防止除以0
        integral += error * deltaTime;
        float derivative = (error - lastError) / deltaTime;
        float output = Kp * error + Ki * integral + Kd * derivative;
        lastError = error;
        lastTime = currentTime;
        return output;
    }
    return 0;  // 如果没有时间差，返回0
}