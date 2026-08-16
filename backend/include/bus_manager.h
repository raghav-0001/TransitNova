#ifndef BUS_MANAGER_H
#define BUS_MANAGER_H

#include <vector>
#include <mutex>

#include "bus.h"
#include "bst.h"
#include "linked_list.h"
#include "doubly_linked_list.h"

class BusManager
{

private:
    mutable std::mutex mutex;

    // Active running buses
    LinkedList activeBuses;

    // Route map (stops)
    DoublyLinkedList liveRoute;

    // Bus searching
    BST busTree;

    // Priority departure queues
    LinkedList parkQueue1;
    LinkedList parkQueue2;

    // Maintenance buses
    LinkedList maintenanceBuses;

    // Terminal departure timers
    float park1DispatchTimer = 0.0f;
    float park2DispatchTimer = 0.0f;

    // How long a bus waits at a terminal before it can depart.
    static constexpr float TERMINAL_DWELL_TIME = 10.0f;

public:
    bool addBus(const Bus &bus);

    bool removeBus(int busID);

    Bus *searchBus(int busID);
    Bus *searchBusByNumber(const std::string &busNumber);
    Bus *searchMaintenanceBusByNumber(const std::string &busNumber);

    void displayActiveBuses();

    void loadDemoBuses();

    // Route

    void loadDefaultRoute();

    void displayLiveRoute();

    // Park 1

    bool enqueuePark1(const Bus &bus);

    Bus dispatchPark1();

    void displayPark1();

    // Park 2

    bool enqueuePark2(const Bus &bus);

    Bus dispatchPark2();

    void displayPark2();

    // Maintenance

    void moveToMaintenance(int busID);
    bool restoreFromMaintenance(int busID);
    Bus *searchMaintenanceBus(int busID);

    void displayMaintenance();

    void updateBuses(float deltaTime);

    void syncBusToTree(const Bus &bus);

    void handleBusArrivals();

    LinkedList &getActiveBuses();
    LinkedList &getPark1();
    LinkedList &getPark2();
    LinkedList &getMaintenanceBuses();

    std::vector<Bus> getActiveBusData() const;
    std::vector<Bus> getPark1BusData() const;
    std::vector<Bus> getPark2BusData() const;
    std::vector<Bus> getMaintenanceBusData() const;
};

#endif