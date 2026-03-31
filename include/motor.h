#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

class MotorController {
private:
    float targetPressure;
    float currentPressure;
    float currentRPM;
    float maxRPM;
    
public:
    MotorController();
    void setTargetPressure(float p);
    void update(float dt);
    float getCurrentPressure();
    float getCurrentRPM();
    float getCurrentDraw();
};

#endif