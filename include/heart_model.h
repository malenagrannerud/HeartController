#ifndef HEART_MODEL_H
#define HEART_MODEL_H

class HeartModel {
private:
    float preload;
    float phase;
    float heartRate;
    
public:
    HeartModel();
    void update(float dt);
    float getInletPressure();
    void setPreload(float p);
    float getHeartRate();
};

#endif