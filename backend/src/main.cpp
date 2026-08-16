#include "../include/server.h"
#include "../include/bus_manager.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>

int main()
{
    BusManager manager;

    // --------------------------------------------------------
    // Load route
    // --------------------------------------------------------

    manager.loadDefaultRoute();

    // --------------------------------------------------------
    // Load 22-bus demo fleet
    // --------------------------------------------------------

    manager.loadDemoBuses();

    // --------------------------------------------------------
    // Initial deployment
    // --------------------------------------------------------
    //
    // 3 buses from Ratnapark
    // 3 buses from Bode
    //
    // Remaining buses stay in queues.
    //

    // --------------------------------------------------------
    // Initial deployment
    // --------------------------------------------------------
    //
    // Start ONE bus from each terminal immediately.
    // Remaining buses are dispatched every 5 seconds.
    // This prevents buses from spawning on top of each other.
    //

    manager.dispatchPark1();
    manager.dispatchPark2();

    // --------------------------------------------------------
    // Start backend simulation
    // --------------------------------------------------------

    std::atomic<bool> simulationRunning(true);

    std::thread simulationThread(
        [&manager, &simulationRunning]()
        {
            using clock =
                std::chrono::steady_clock;

            auto previous =
                clock::now();

            while (simulationRunning)
            {
                auto now =
                    clock::now();

                std::chrono::duration<float> elapsed =
                    now - previous;

                previous = now;

                float deltaTime =
                    elapsed.count();

                /*
                 * Prevent massive simulation jumps
                 * after pauses/scheduling delays.
                 */
                if (deltaTime > 0.5f)
                {
                    deltaTime = 0.5f;
                }

                manager.updateBuses(deltaTime);

                std::this_thread::sleep_for(
                    std::chrono::milliseconds(100));
            }
        });

    // --------------------------------------------------------
    // Start HTTP server
    // --------------------------------------------------------

    Server server;

    server.start(manager);

    // --------------------------------------------------------
    // Stop simulation when server exits
    // --------------------------------------------------------

    simulationRunning = false;

    if (simulationThread.joinable())
    {
        simulationThread.join();
    }

    return 0;
}