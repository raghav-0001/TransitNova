#include "../include/bus_manager.h"

#include <iostream>
#include <mutex>

using namespace std;

bool BusManager::addBus(const Bus &bus)
{
    if (busTree.search(bus.getID()) != nullptr)
    {
        cout << "Bus already exists\n";
        return false;
    }

    activeBuses.insert(bus);
    busTree.insert(bus);

    cout << "Bus added successfully\n";

    return true;
}

bool BusManager::removeBus(int busID)
{
    Bus *bus = busTree.search(busID);

    if (bus == nullptr)
    {
        cout << "Bus not found\n";
        return false;
    }

    // Remove the bus from every possible location.
    activeBuses.remove(busID);
    parkQueue1.remove(busID);
    parkQueue2.remove(busID);
    maintenanceBuses.remove(busID);
    busTree.remove(busID);

    cout << "Bus removed\n";

    return true;
}

Bus *BusManager::searchBus(int busID)
{
    return busTree.search(busID);
}

Bus *BusManager::searchBusByNumber(const std::string &busNumber)
{
    // Running buses
    Bus *bus = activeBuses.searchByNumber(busNumber);

    if (bus != nullptr)
        return bus;

    // Ratnapark queue
    bus = parkQueue1.searchByNumber(busNumber);

    if (bus != nullptr)
        return bus;

    // Bode queue
    bus = parkQueue2.searchByNumber(busNumber);

    if (bus != nullptr)
        return bus;

    // Maintenance
    return maintenanceBuses.searchByNumber(busNumber);
}

Bus *BusManager::searchMaintenanceBusByNumber(
    const std::string &busNumber)
{
    return maintenanceBuses.searchByNumber(busNumber);
}

void BusManager::displayActiveBuses()
{
    activeBuses.display();
}

void BusManager::loadDemoBuses()
{
    parkQueue1.clear();
    parkQueue2.clear();
    activeBuses.clear();
    maintenanceBuses.clear();

    const Bus park1Buses[] = {
        Bus(1, "5543", "Kathmandu ↔ Bode", "Ratnapark Buspark",
            "east", 40, 1, false, false),
        Bus(2, "4817", "Kathmandu ↔ Bode", "Ratnapark Buspark",
            "east", 40, 1, false, false),
        Bus(3, "4112", "Kathmandu ↔ Bode", "Ratnapark Buspark",
            "east", 40, 1, false, false),
        Bus(4, "4114", "Kathmandu ↔ Bode", "Ratnapark Buspark",
            "east", 40, 1, false, false),
        Bus(5, "232", "Kathmandu ↔ Bode", "Ratnapark Buspark",
            "east", 40, 1, false, false),
        Bus(6, "4730", "Kathmandu ↔ Bode", "Ratnapark Buspark",
            "east", 40, 1, false, false),
        Bus(7, "4816", "Kathmandu ↔ Bode", "Ratnapark Buspark",
            "east", 40, 1, false, false),
        Bus(8, "4115", "Kathmandu ↔ Bode", "Ratnapark Buspark",
            "east", 40, 1, false, false),
        Bus(9, "36", "Kathmandu ↔ Bode", "Ratnapark Buspark",
            "east", 40, 1, false, false),
        Bus(10, "1979", "Kathmandu ↔ Bode", "Ratnapark Buspark",
            "east", 40, 1, false, false),
        Bus(11, "523", "Kathmandu ↔ Bode", "Ratnapark Buspark",
            "east", 40, 1, false, false)};

    const Bus park2Buses[] = {
        Bus(12, "7401", "Kathmandu ↔ Bode", "Bode Buspark",
            "west", 40, 1, false, false),
        Bus(13, "4111", "Kathmandu ↔ Bode", "Bode Buspark",
            "west", 40, 1, false, false),
        Bus(14, "38", "Kathmandu ↔ Bode", "Bode Buspark",
            "west", 40, 1, false, false),
        Bus(15, "699", "Kathmandu ↔ Bode", "Bode Buspark",
            "west", 40, 1, false, false),
        Bus(16, "5832", "Kathmandu ↔ Bode", "Bode Buspark",
            "west", 40, 1, false, false),
        Bus(17, "5829", "Kathmandu ↔ Bode", "Bode Buspark",
            "west", 40, 1, false, false),
        Bus(18, "4732", "Kathmandu ↔ Bode", "Bode Buspark",
            "west", 40, 1, false, false),
        Bus(19, "39", "Kathmandu ↔ Bode", "Bode Buspark",
            "west", 40, 1, false, false),
        Bus(20, "4731", "Kathmandu ↔ Bode", "Bode Buspark",
            "west", 40, 1, false, false),
        Bus(21, "4119", "Kathmandu ↔ Bode Buspark",
            "Bode Buspark", "west", 40, 1, false, false),
        Bus(22, "4118", "Kathmandu ↔ Bode", "Bode Buspark",
            "west", 40, 1, false, false)};

    // Park 1 buses exist only in Park 1 + BST.
    for (const Bus &bus : park1Buses)
    {
        parkQueue1.insert(bus);
        busTree.insert(bus);
    }

    // Park 2 buses exist only in Park 2 + BST.
    for (const Bus &bus : park2Buses)
    {
        parkQueue2.insert(bus);
        busTree.insert(bus);
    }

    park1DispatchTimer = 0.0f;
    park2DispatchTimer = 0.0f;

    cout << "Exact 22-bus demo fleet loaded.\n";
}

void BusManager::loadDefaultRoute()
{
    liveRoute.clear();

    liveRoute.insert(Route(0, "Ratnapark Buspark"));
    liveRoute.insert(Route(1, "Singha Durbar"));
    liveRoute.insert(Route(2, "Maitighar"));
    liveRoute.insert(Route(3, "Bijulibazar"));
    liveRoute.insert(Route(4, "Baneshwor"));
    liveRoute.insert(Route(5, "Civil"));
    liveRoute.insert(Route(6, "Shantinagar"));
    liveRoute.insert(Route(7, "Tinkune"));
    liveRoute.insert(Route(8, "Koteshwor"));
    liveRoute.insert(Route(9, "Jadibuti"));
    liveRoute.insert(Route(10, "Lokanthali"));
    liveRoute.insert(Route(11, "Kaushaltar"));
    liveRoute.insert(Route(12, "Gatthaghar"));
    liveRoute.insert(Route(13, "Thimi"));
    liveRoute.insert(Route(14, "Bode Chardobato"));
    liveRoute.insert(Route(15, "Bode Planning"));
    liveRoute.insert(Route(16, "Jaya School"));
    liveRoute.insert(Route(17, "Bode Buspark"));

    cout << "Default route loaded\n";
}

void BusManager::displayLiveRoute()
{
    liveRoute.displayForward();
}

bool BusManager::enqueuePark1(const Bus &bus)
{
    if (bus.getPriority() != 1)
        return false;

    if (parkQueue1.search(bus.getID()) != nullptr)
        return false;

    return parkQueue1.insert(bus);
}

Bus BusManager::dispatchPark1()
{
    if (parkQueue1.isEmpty())
        return Bus();

    Bus bus = parkQueue1.removeFront();

    // Priority 0 = maintenance. Do not dispatch.
    if (bus.getPriority() == 0 || bus.isMaintenance())
    {
        bus.setPriority(0);
        bus.setMaintenance(true);
        bus.setAvailable(false);

        if (maintenanceBuses.search(bus.getID()) == nullptr)
            maintenanceBuses.insert(bus);

        busTree.remove(bus.getID());
        busTree.insert(bus);

        cout << "Bus " << bus.getBusNumber()
             << " is under maintenance and cannot dispatch.\n";

        return Bus();
    }

    // Priority 1 -> 2 means PARKED -> RUNNING.
    bus.setPriority(2);
    bus.setAvailable(true);
    bus.setMaintenance(false);

    bus.setCurrentStop("Ratnapark Buspark");
    bus.setDirection("east");
    bus.resetSimulation();

    // Ensure the bus exists only once in the active list.
    activeBuses.remove(bus.getID());
    activeBuses.insert(bus);

    // Update searchable copy.
    busTree.remove(bus.getID());
    busTree.insert(bus);

    cout << "Bus " << bus.getBusNumber()
         << " dispatched from Ratnapark Buspark.\n";

    return bus;
}

void BusManager::displayPark1()
{
    parkQueue1.display();
}

bool BusManager::enqueuePark2(const Bus &bus)
{
    if (bus.getPriority() != 1)
        return false;

    if (parkQueue2.search(bus.getID()) != nullptr)
        return false;

    return parkQueue2.insert(bus);
}

Bus BusManager::dispatchPark2()
{
    if (parkQueue2.isEmpty())
        return Bus();

    Bus bus = parkQueue2.removeFront();

    // Priority 0 = maintenance. Do not dispatch.
    if (bus.getPriority() == 0 || bus.isMaintenance())
    {
        bus.setPriority(0);
        bus.setMaintenance(true);
        bus.setAvailable(false);

        if (maintenanceBuses.search(bus.getID()) == nullptr)
            maintenanceBuses.insert(bus);

        busTree.remove(bus.getID());
        busTree.insert(bus);

        cout << "Bus " << bus.getBusNumber()
             << " is under maintenance and cannot dispatch.\n";

        return Bus();
    }

    // Priority 1 -> 2 means PARKED -> RUNNING.
    bus.setPriority(2);
    bus.setAvailable(true);
    bus.setMaintenance(false);

    bus.setCurrentStop("Bode Buspark");
    bus.setDirection("west");
    bus.resetSimulation();

    // Ensure the bus exists only once in the active list.
    activeBuses.remove(bus.getID());
    activeBuses.insert(bus);

    // Update searchable copy.
    busTree.remove(bus.getID());
    busTree.insert(bus);

    cout << "Bus " << bus.getBusNumber()
         << " dispatched from Bode Buspark.\n";

    return bus;
}

void BusManager::displayPark2()
{
    parkQueue2.display();
}

void BusManager::moveToMaintenance(int busID)
{
    Bus *bus = busTree.search(busID);

    if (bus == nullptr)
    {
        cout << "Bus not found\n";
        return;
    }

    // Already in maintenance.
    if (maintenanceBuses.search(busID) != nullptr)
        return;

    Bus stoppedBus = *bus;

    // Preserve exact position.
    stoppedBus.setLastStop(stoppedBus.getCurrentStop());

    if (stoppedBus.getCurrentStop().find("Buspark") != string::npos)
    {
        stoppedBus.setLastPark(stoppedBus.getCurrentStop());
    }

    // ANY STATE -> MAINTENANCE.
    stoppedBus.setPriority(0);
    stoppedBus.setMaintenance(true);
    stoppedBus.setAvailable(false);

    // Remove from every operational structure.
    activeBuses.remove(busID);
    parkQueue1.remove(busID);
    parkQueue2.remove(busID);
    busTree.remove(busID);

    // Store only in maintenance.
    maintenanceBuses.insert(stoppedBus);

    // Keep the BST synchronized with the maintenance state.
    busTree.insert(stoppedBus);

    cout << "Bus " << stoppedBus.getBusNumber()
         << " stopped for maintenance at "
         << stoppedBus.getLastStop() << "\n";
}

bool BusManager::restoreFromMaintenance(int busID)
{
    Bus *maintenanceBus = maintenanceBuses.search(busID);

    if (maintenanceBus == nullptr)
        return false;

    Bus restoredBus = *maintenanceBus;

    // Every repaired bus returns to Ratnapark.
    restoredBus.setCurrentStop("Ratnapark Buspark");
    restoredBus.setDirection("east");

    // MAINTENANCE -> PARKED.
    restoredBus.setPriority(1);
    restoredBus.setMaintenance(false);
    restoredBus.setAvailable(false);

    restoredBus.resetSimulation();

    // Remove old maintenance state.
    maintenanceBuses.remove(busID);

    // Ensure no stale queue entry exists.
    parkQueue1.remove(busID);
    parkQueue2.remove(busID);
    activeBuses.remove(busID);

    // Put repaired bus into Ratnapark queue.
    parkQueue1.insert(restoredBus);

    // Update BST copy.
    busTree.remove(busID);
    busTree.insert(restoredBus);

    cout << "Bus " << restoredBus.getBusNumber()
         << " restored and placed at Ratnapark Buspark queue.\n";

    return true;
}

Bus *BusManager::searchMaintenanceBus(int busID)
{
    return maintenanceBuses.search(busID);
}

void BusManager::displayMaintenance()
{
    maintenanceBuses.display();
}

void BusManager::updateBuses(float deltaTime)
{
    std::lock_guard<std::mutex> lock(mutex);

    // --------------------------------------------------------
    // 1. Move currently running buses
    // --------------------------------------------------------

    Node *current = activeBuses.getHead();

    while (current != nullptr)
    {
        current->data.update(deltaTime);

        syncBusToTree(current->data);

        current = current->next;
    }

    // --------------------------------------------------------
    // 2. Move buses that reached a terminal into queues
    // --------------------------------------------------------

    handleBusArrivals();

    // --------------------------------------------------------
    // 3. Advance terminal timers
    // --------------------------------------------------------

    if (!parkQueue1.isEmpty())
    {
        park1DispatchTimer += deltaTime;
    }
    else
    {
        park1DispatchTimer = 0.0f;
    }

    if (!parkQueue2.isEmpty())
    {
        park2DispatchTimer += deltaTime;
    }
    else
    {
        park2DispatchTimer = 0.0f;
    }

    // --------------------------------------------------------
    // 4. Dispatch a bus from Ratnapark
    // --------------------------------------------------------

    if (park1DispatchTimer >= TERMINAL_DWELL_TIME &&
        !parkQueue1.isEmpty())
    {
        Bus dispatched = dispatchPark1();

        if (dispatched.getID() != 0)
        {
            // If this was the last bus in the park,
            // completely reset the timer.
            if (parkQueue1.isEmpty())
                park1DispatchTimer = 0.0f;
            else
                park1DispatchTimer -= TERMINAL_DWELL_TIME;
        }
    }

    // --------------------------------------------------------
    // 5. Dispatch a bus from Bode
    // --------------------------------------------------------

    if (park2DispatchTimer >= TERMINAL_DWELL_TIME &&
        !parkQueue2.isEmpty())
    {
        Bus dispatched = dispatchPark2();

        if (dispatched.getID() != 0)
        {
            // If this was the last bus in the park,
            // completely reset the timer.
            if (parkQueue2.isEmpty())
                park2DispatchTimer = 0.0f;
            else
                park2DispatchTimer -= TERMINAL_DWELL_TIME;
        }
    }
}

void BusManager::syncBusToTree(const Bus &bus)
{
    busTree.remove(bus.getID());
    busTree.insert(bus);
}

void BusManager::handleBusArrivals()
{
    Node *current = activeBuses.getHead();

    while (current != nullptr)
    {
        Node *nextNode = current->next;

        Bus arrivedBus = current->data;

        // Only running buses can arrive.
        if (arrivedBus.getPriority() != 2)
        {
            current = nextNode;
            continue;
        }

        // ==========================
        // ARRIVED AT RATNAPARK
        // ==========================

        if (arrivedBus.getCurrentStopIndex() == 0 &&
            arrivedBus.getProgress() >= 1.0f)
        {
            arrivedBus.setCurrentStop("Ratnapark Buspark");
            arrivedBus.setDirection("east");

            arrivedBus.setPriority(1);
            arrivedBus.setAvailable(false);
            arrivedBus.setMaintenance(false);

            activeBuses.remove(arrivedBus.getID());
            parkQueue1.remove(arrivedBus.getID());
            parkQueue2.remove(arrivedBus.getID());
            busTree.remove(arrivedBus.getID());

            parkQueue1.insert(arrivedBus);
            busTree.insert(arrivedBus);

            cout << "Bus " << arrivedBus.getBusNumber()
                 << " arrived at Ratnapark Buspark.\n";
        }

        // ==========================
        // ARRIVED AT BODE
        // ==========================

        else if (arrivedBus.getCurrentStopIndex() == 17 &&
                 arrivedBus.getProgress() >= 1.0f)
        {
            arrivedBus.setCurrentStop("Bode Buspark");
            arrivedBus.setDirection("west");

            arrivedBus.setPriority(1);
            arrivedBus.setAvailable(false);
            arrivedBus.setMaintenance(false);

            activeBuses.remove(arrivedBus.getID());
            parkQueue1.remove(arrivedBus.getID());
            parkQueue2.remove(arrivedBus.getID());
            busTree.remove(arrivedBus.getID());

            parkQueue2.insert(arrivedBus);
            busTree.insert(arrivedBus);

            cout << "Bus " << arrivedBus.getBusNumber()
                 << " arrived at Bode Buspark.\n";
        }

        current = nextNode;
    }
}

LinkedList &BusManager::getActiveBuses()
{
    return activeBuses;
}

LinkedList &BusManager::getPark1()
{
    return parkQueue1;
}

LinkedList &BusManager::getPark2()
{
    return parkQueue2;
}

LinkedList &BusManager::getMaintenanceBuses()
{
    return maintenanceBuses;
}

// ============================================================
// READ-ONLY SNAPSHOTS FOR SERVER / FRONTEND API
// ============================================================

std::vector<Bus> BusManager::getActiveBusData() const
{
    std::lock_guard<std::mutex> lock(mutex);

    std::vector<Bus> buses;

    const Node *current = activeBuses.getHead();

    while (current != nullptr)
    {
        buses.push_back(current->data);
        current = current->next;
    }

    return buses;
}

std::vector<Bus> BusManager::getPark1BusData() const
{
    std::lock_guard<std::mutex> lock(mutex);

    std::vector<Bus> buses;

    const Node *current = parkQueue1.getHead();

    while (current != nullptr)
    {
        buses.push_back(current->data);
        current = current->next;
    }

    return buses;
}

std::vector<Bus> BusManager::getPark2BusData() const
{
    std::lock_guard<std::mutex> lock(mutex);

    std::vector<Bus> buses;

    const Node *current = parkQueue2.getHead();

    while (current != nullptr)
    {
        buses.push_back(current->data);
        current = current->next;
    }

    return buses;
}

std::vector<Bus> BusManager::getMaintenanceBusData() const
{
    std::lock_guard<std::mutex> lock(mutex);

    std::vector<Bus> buses;

    const Node *current = maintenanceBuses.getHead();

    while (current != nullptr)
    {
        buses.push_back(current->data);
        current = current->next;
    }

    return buses;
}