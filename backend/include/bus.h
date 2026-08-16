#pragma once

#include <string>
#include "route.h"
#include "travel_times.h"

class Bus
{
private:
    int id;
    std::string busNumber;
    std::string routeName;

    Route route;

    std::string currentStop;
    std::string direction;
    std::string lastStop;
    std::string lastPark;

    int capacity;
    int priority;

    bool available;
    bool maintenance;

    // Simulation state
    int currentStopIndex;
    float elapsedTime;
    float progress;

public:
    Bus();

    Bus(
        int id,
        const std::string &busNumber,
        const std::string &route,
        const std::string &currentStop,
        const std::string &direction,
        int capacity,
        int priority,
        bool available,
        bool maintenance);

    // Getters
    int getID() const;
    std::string getBusNumber() const;
    std::string getRoute() const;
    std::string getCurrentStop() const;
    std::string getDirection() const;
    std::string getLastStop() const;
    std::string getLastPark() const;
    int getCapacity() const;
    int getPriority() const;
    bool isAvailable() const;
    bool isMaintenance() const;
    bool isVisibleOnMap() const;

    // Simulation getters
    int getCurrentStopIndex() const;
    float getElapsedTime() const;
    float getProgress() const;

    // Setters
    void setID(int id);
    void setBusNumber(const std::string &busNumber);
    void setRoute(const std::string &route);
    void setCurrentStop(const std::string &currentStop);
    void setDirection(const std::string &direction);
    void setLastStop(const std::string &lastStop);
    void setLastPark(const std::string &lastPark);
    void setCapacity(int capacity);
    void setPriority(int priority);
    void setAvailable(bool available);
    void setMaintenance(bool maintenance);

    // Simulation
    void update(float deltaTime);
    void resetSimulation();
};