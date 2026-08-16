#include "../include/bus.h"

Bus::Bus()
{
    id = 0;
    busNumber = "";
    routeName = "";

    currentStop = "";
    direction = "";
    lastStop = "";
    lastPark = "";

    capacity = 0;
    priority = 0;

    available = true;
    maintenance = false;

    currentStopIndex = 0;
    elapsedTime = 0.0f;
    progress = 0.0f;
}

Bus::Bus(
    int id,
    const std::string &busNumber,
    const std::string &route,
    const std::string &currentStop,
    const std::string &direction,
    int capacity,
    int priority,
    bool available,
    bool maintenance)
{
    this->id = id;
    this->busNumber = busNumber;
    this->routeName = route;

    this->route = Route(0, route);

    this->currentStop = currentStop;
    this->direction = direction;
    this->lastStop = currentStop;
    this->lastPark = currentStop;

    this->capacity = capacity;
    this->priority = priority;

    this->available = available;
    this->maintenance = maintenance;

    currentStopIndex = this->route.getStopIndex(currentStop);

    if (currentStopIndex < 0)
        currentStopIndex = 0;

    elapsedTime = 0.0f;
    progress = 0.0f;
}

// ==================== GETTERS ====================

int Bus::getID() const
{
    return id;
}

std::string Bus::getBusNumber() const
{
    return busNumber;
}

std::string Bus::getRoute() const
{
    return routeName;
}

std::string Bus::getCurrentStop() const
{
    return currentStop;
}

std::string Bus::getDirection() const
{
    return direction;
}

std::string Bus::getLastStop() const
{
    return lastStop.empty() ? currentStop : lastStop;
}

std::string Bus::getLastPark() const
{
    return lastPark;
}

int Bus::getCapacity() const
{
    return capacity;
}

int Bus::getPriority() const
{
    return priority;
}

bool Bus::isAvailable() const
{
    return available;
}

bool Bus::isMaintenance() const
{
    return maintenance;
}

bool Bus::isVisibleOnMap() const
{
    return available && !maintenance && priority > 0;
}

int Bus::getCurrentStopIndex() const
{
    return currentStopIndex;
}

float Bus::getElapsedTime() const
{
    return elapsedTime;
}

float Bus::getProgress() const
{
    return progress;
}

// ==================== SETTERS ====================

void Bus::setID(int id)
{
    this->id = id;
}

void Bus::setBusNumber(const std::string &busNumber)
{
    this->busNumber = busNumber;
}

void Bus::setRoute(const std::string &route)
{
    this->routeName = route;
}

void Bus::setCurrentStop(const std::string &currentStop)
{
    this->currentStop = currentStop;

    int index = route.getStopIndex(currentStop);

    if (index >= 0)
        currentStopIndex = index;
}

void Bus::setDirection(const std::string &direction)
{
    this->direction = direction;
}

void Bus::setLastStop(const std::string &lastStop)
{
    this->lastStop = lastStop;
}

void Bus::setLastPark(const std::string &lastPark)
{
    this->lastPark = lastPark;
}

void Bus::setCapacity(int capacity)
{
    this->capacity = capacity;
}

void Bus::setPriority(int priority)
{
    this->priority = priority;
}

void Bus::setAvailable(bool available)
{
    this->available = available;
}

void Bus::setMaintenance(bool maintenance)
{
    this->maintenance = maintenance;

    if (maintenance)
    {
        priority = 0;
        available = false;

        lastStop = currentStop;

        if (currentStop.find("Buspark") != std::string::npos)
            lastPark = currentStop;
    }
}

// ==================== SIMULATION ====================

void Bus::update(float deltaTime)
{
    // Maintenance, parked, or unavailable buses do not move.
    if (priority == 0 || maintenance || !available)
        return;

    // A bus must have a valid direction to move.
    if (direction != "east" && direction != "west")
        return;

    // Continue processing movement while this frame's
    // deltaTime still contains enough time to reach another stop.
    while (deltaTime > 0.0f)
    {
        // Already at the end of the route.
        if (direction == "east" && currentStopIndex >= 17)
            return;

        // Already at the beginning of the route.
        if (direction == "west" && currentStopIndex <= 0)
            return;

        int transitionIndex;

        if (direction == "east")
            transitionIndex = currentStopIndex;
        else
            transitionIndex = currentStopIndex - 1;

        if (transitionIndex < 0 || transitionIndex >= NUM_TRANSITIONS)
            return;

        float remainingTime =
            travelTime[transitionIndex] - elapsedTime;

        // The bus does not reach the next stop during this update.
        if (deltaTime < remainingTime)
        {
            elapsedTime += deltaTime;
            progress =
                elapsedTime / travelTime[transitionIndex];

            return;
        }

        // The bus reaches the next stop during this update.
        deltaTime -= remainingTime;

        lastStop = currentStop;

        if (direction == "east")
            currentStopIndex++;
        else
            currentStopIndex--;

        currentStop = route.getStop(currentStopIndex);

        // Start the next segment with the leftover time.
        elapsedTime = 0.0f;
        progress = 0.0f;

        // Reached a bus park.
        if (currentStopIndex == 0 || currentStopIndex == 17)
        {
            lastPark = currentStop;
            available = false;
            progress = 1.0f;
            return;
        }
    }
}

void Bus::resetSimulation()
{
    elapsedTime = 0.0f;
    progress = 0.0f;
}