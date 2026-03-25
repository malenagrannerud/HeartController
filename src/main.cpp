#include <iostream>
#include <thread>
#include <chrono>
#include "../include/heart_model.h"
#include "../include/pressure_sensor.h"
#include "../include/motor_controller.h"
#include "../include/frank_starling.h"
#include "../include/display.h"

using namespace std;

int main() {
    HeartModel heart;
    PressureSensor inletSensor;
    PressureSensor outletSensor;
    MotorController motor;
    FrankStarling regulator;
    Display display;
    
    float dt = 0.05;
    float time = 0;
    
    float sumInlet = 0, sumOutlet = 0, sumError = 0;
    int samples = 0;
    
    for (int i = 0; i < 200; i++) {
        heart.update(dt);
        float measuredInlet = inletSensor.measure(heart.getInletPressure());
        float targetOutlet = regulator.calculateTargetPressure(measuredInlet);
        
        motor.setTargetPressure(targetOutlet);
        motor.update(dt);
        
        float actualOutlet = outletSensor.measure(motor.getCurrentPressure());
        
        display.update(measuredInlet, targetOutlet, actualOutlet, 
                      motor.getCurrentRPM(), motor.getCurrentDraw(),
                      heart.getHeartRate(), time);
        
        sumInlet += measuredInlet;
        sumOutlet += actualOutlet;
        sumError += abs(targetOutlet - actualOutlet);
        samples++;
        
        time += dt;
        this_thread::sleep_for(chrono::milliseconds(50));
        
        if (i == 100) {
            heart.setPreload(12.0);
        }
    }
    
    display.printStats(sumInlet/samples, sumOutlet/samples, sumError/samples);
    return 0;
}
