// PIDController.h
#ifndef PIDCONTROLLER_H
#define PIDCONTROLLER_H

class PIDController {
public:
    PIDController(float Kp, float Ki, float Kd) : Kp(Kp), Ki(Ki), Kd(Kd), integral(0), lastError(0), lastTime(0) {}

    float update(float setPoint, float feedbackValue);

private:
    float Kp;
    float Ki;
    float Kd;
    float integral;
    float lastError;
    unsigned long lastTime;
};

#endif // PIDCONTROLLER_H