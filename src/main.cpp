
/**
 * @file main.cpp
 * @brief Entry point for TAH simulation. It creates the Simulation controller and starts the
 * simulation loop.
 * 
 * bygg & kör: cd /workspaces/HeartController && rm -rf build && mkdir build && cd build && cmake -DBUILD_TESTS=OFF .. && make && ./bin/tah_simulator

 * kör:  cd /workspaces/HeartController/build && make tah_simulator && ./bin/tah_simulator
 */

#include "Simulation.h"

int main() {
    Simulation sim;
    sim.run();
    return 0;
}