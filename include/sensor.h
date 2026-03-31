#ifndef HEART_MODEL_H
#define HEART_MODEL_H

class HeartModel {
private:
    float preload;
    float afterload;
    float contractility;
    float heartRate;
    
public:
    HeartModel();
    void update(float dt);
    float getInletPressure();
    float getOutletPressure();
    float getFlow();
    void setPreload(float p);
    void setAfterload(float a);
    void setHeartRate(float hr);
    float getHeartRate();
};

#endif