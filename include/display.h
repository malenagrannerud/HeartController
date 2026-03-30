cat > /workspaces/HeartController/include/display.h << 'EOF'
#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>

class Display {
public:
    void update(float preload_mmHg, float target_mmHg, float actual_mmHg,
                float motor_rpm, float motor_current_mA,
                float heart_rate_bpm, float simulation_time_s);
    
    void printStats(float avg_preload_mmHg, float avg_afterload_mmHg, float avg_error_mmHg);
    
    void showControls();
};

#endif
EOF