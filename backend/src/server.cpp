#include "../include/server.h"
#include "../include/bus_manager.h"
#include "../httplib.h"
#include <nlohmann/json.hpp>

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using json = nlohmann::json;

// ============================================================
// Read frontend files
// ============================================================

string readFile(const string &path)
{
    ifstream file(path);

    if (!file)
        return "File not found.";

    stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

// ============================================================
// CORS
// ============================================================

void addCorsHeaders(httplib::Response &res)
{
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Methods",
                   "GET, POST, OPTIONS");
    res.set_header("Access-Control-Allow-Headers",
                   "Content-Type");
}

// ============================================================
// SERVER
// ============================================================

void Server::start(BusManager &manager)
{
    httplib::Server server;

    // ========================================================
    // FRONTEND PAGES
    // ========================================================

    server.Get("/", [](const httplib::Request &, httplib::Response &res)
               {
        addCorsHeaders(res);

        res.set_content(
            readFile("/app/frontend/index.html"),
            "text/html"); });

    server.Get("/index.html",
               [](const httplib::Request &, httplib::Response &res)
               {
                   addCorsHeaders(res);

                   res.set_content(
                       readFile("/app/frontend/index.html"),
                       "text/html");
               });

    server.Get("/search.html",
               [](const httplib::Request &, httplib::Response &res)
               {
                   addCorsHeaders(res);

                   res.set_content(
                       readFile("/app/frontend/search.html"),
                       "text/html");
               });

    server.Get("/journey.html",
               [](const httplib::Request &, httplib::Response &res)
               {
                   addCorsHeaders(res);

                   res.set_content(
                       readFile("/app/frontend/journey.html"),
                       "text/html");
               });

    // ========================================================
    // CSS
    // ========================================================

    server.Get("/style.css",
               [](const httplib::Request &, httplib::Response &res)
               {
                   addCorsHeaders(res);

                   res.set_content(
                       readFile("/app/frontend/style.css"),
                       "text/css");
               });

    // ========================================================
    // JAVASCRIPT
    // ========================================================

    server.Get("/script.js",
               [](const httplib::Request &, httplib::Response &res)
               {
                   addCorsHeaders(res);

                   res.set_content(
                       readFile("/app/frontend/script.js"),
                       "application/javascript");
               });

    server.Get("/journey.js",
               [](const httplib::Request &, httplib::Response &res)
               {
                   addCorsHeaders(res);

                   res.set_content(
                       readFile("/app/frontend/journey.js"),
                       "application/javascript");
               });

    server.Get("/search.js",
               [](const httplib::Request &, httplib::Response &res)
               {
                   addCorsHeaders(res);

                   res.set_content(
                       readFile("/app/frontend/search.js"),
                       "application/javascript");
               });

    // ========================================================
    // FRONTEND IMAGES
    // ========================================================

    server.set_mount_point(
        "/images",
        "/app/frontend/images");

    // ========================================================
    // BACKEND STATUS
    // ========================================================

    server.Get("/api/status",
               [](const httplib::Request &, httplib::Response &res)
               {
                   addCorsHeaders(res);

                   json response = {
                       {"status", "ONLINE"},
                       {"message", "TransitNova Backend Connected"}};

                   res.set_content(
                       response.dump(),
                       "application/json");
               });

    // ========================================================
    // TEST API
    // ========================================================

    server.Get("/api/hello",
               [](const httplib::Request &, httplib::Response &res)
               {
                   addCorsHeaders(res);

                   json response = {
                       {"message", "TransitNova Backend Connected"}};

                   res.set_content(
                       response.dump(),
                       "application/json");
               });

    // ========================================================
    // GET ALL ACTIVE / PARKED / MAINTENANCE BUSES
    // ========================================================

    server.Get("/api/buses",
               [&manager](const httplib::Request &,
                          httplib::Response &res)
               {
                   addCorsHeaders(res);

                   json response;

                   // ====================================================
                   // ACTIVE
                   // ====================================================

                   response["active"] = json::array();

                   for (const Bus &bus :
                        manager.getActiveBusData())
                   {
                       response["active"].push_back({{"id", bus.getID()},
                                                     {"busNumber", bus.getBusNumber()},
                                                     {"route", bus.getRoute()},
                                                     {"currentStop", bus.getCurrentStop()},
                                                     {"direction", bus.getDirection()},
                                                     {"lastStop", bus.getLastStop()},
                                                     {"lastPark", bus.getLastPark()},
                                                     {"capacity", bus.getCapacity()},
                                                     {"priority", bus.getPriority()},
                                                     {"available", bus.isAvailable()},
                                                     {"maintenance", bus.isMaintenance()},
                                                     {"currentStopIndex",
                                                      bus.getCurrentStopIndex()},
                                                     {"elapsedTime",
                                                      bus.getElapsedTime()},
                                                     {"progress",
                                                      bus.getProgress()}});
                   }

                   // ====================================================
                   // PARK 1
                   // ====================================================

                   response["park1"] = json::array();

                   for (const Bus &bus :
                        manager.getPark1BusData())
                   {
                       response["park1"].push_back({{"id", bus.getID()},
                                                    {"busNumber", bus.getBusNumber()},
                                                    {"route", bus.getRoute()},
                                                    {"currentStop", bus.getCurrentStop()},
                                                    {"direction", bus.getDirection()},
                                                    {"lastStop", bus.getLastStop()},
                                                    {"lastPark", bus.getLastPark()},
                                                    {"capacity", bus.getCapacity()},
                                                    {"priority", bus.getPriority()},
                                                    {"available", bus.isAvailable()},
                                                    {"maintenance", bus.isMaintenance()},
                                                    {"currentStopIndex",
                                                     bus.getCurrentStopIndex()},
                                                    {"elapsedTime",
                                                     bus.getElapsedTime()},
                                                    {"progress",
                                                     bus.getProgress()}});
                   }

                   // ====================================================
                   // PARK 2
                   // ====================================================

                   response["park2"] = json::array();

                   for (const Bus &bus :
                        manager.getPark2BusData())
                   {
                       response["park2"].push_back({{"id", bus.getID()},
                                                    {"busNumber", bus.getBusNumber()},
                                                    {"route", bus.getRoute()},
                                                    {"currentStop", bus.getCurrentStop()},
                                                    {"direction", bus.getDirection()},
                                                    {"lastStop", bus.getLastStop()},
                                                    {"lastPark", bus.getLastPark()},
                                                    {"capacity", bus.getCapacity()},
                                                    {"priority", bus.getPriority()},
                                                    {"available", bus.isAvailable()},
                                                    {"maintenance", bus.isMaintenance()},
                                                    {"currentStopIndex",
                                                     bus.getCurrentStopIndex()},
                                                    {"elapsedTime",
                                                     bus.getElapsedTime()},
                                                    {"progress",
                                                     bus.getProgress()}});
                   }

                   // ====================================================
                   // MAINTENANCE
                   // ====================================================

                   response["maintenance"] = json::array();

                   for (const Bus &bus :
                        manager.getMaintenanceBusData())
                   {
                       response["maintenance"].push_back({{"id", bus.getID()},
                                                          {"busNumber", bus.getBusNumber()},
                                                          {"route", bus.getRoute()},
                                                          {"currentStop", bus.getCurrentStop()},
                                                          {"direction", bus.getDirection()},
                                                          {"lastStop", bus.getLastStop()},
                                                          {"lastPark", bus.getLastPark()},
                                                          {"capacity", bus.getCapacity()},
                                                          {"priority", bus.getPriority()},
                                                          {"available", bus.isAvailable()},
                                                          {"maintenance", bus.isMaintenance()},
                                                          {"currentStopIndex",
                                                           bus.getCurrentStopIndex()},
                                                          {"elapsedTime",
                                                           bus.getElapsedTime()},
                                                          {"progress",
                                                           bus.getProgress()}});
                   }

                   res.set_content(
                       response.dump(4),
                       "application/json");
               });

    // ========================================================
    // JOURNEY PLANNER
    // ========================================================

    server.Post(
        "/api/journey",
        [&manager](const httplib::Request &req,
                   httplib::Response &res)
        {
            addCorsHeaders(res);

            try
            {
                // ====================================================
                // READ REQUEST
                // ====================================================

                if (req.body.empty())
                {
                    res.status = 400;

                    res.set_content(
                        R"({"message":"Missing request body"})",
                        "application/json");

                    return;
                }

                json body = json::parse(req.body);

                if (!body.contains("from") ||
                    !body.contains("to"))
                {
                    res.status = 400;

                    res.set_content(
                        R"({"message":"Both pickup and drop-off stops are required"})",
                        "application/json");

                    return;
                }

                string from =
                    body["from"].get<string>();

                string to =
                    body["to"].get<string>();

                // ====================================================
                // ROUTE
                // ====================================================

                Route route(
                    0,
                    "Kathmandu - Bode Route");

                int pickupIndex =
                    route.getStopIndex(from);

                int destinationIndex =
                    route.getStopIndex(to);

                // ====================================================
                // VALIDATE STOPS
                // ====================================================

                if (pickupIndex == -1 ||
                    destinationIndex == -1)
                {
                    res.status = 400;

                    res.set_content(
                        R"({"message":"Invalid pickup or destination stop"})",
                        "application/json");

                    return;
                }

                if (pickupIndex == destinationIndex)
                {
                    res.status = 400;

                    res.set_content(
                        R"({"message":"Pickup and destination cannot be the same"})",
                        "application/json");

                    return;
                }

                // ====================================================
                // DETERMINE DIRECTION
                // ====================================================

                string requiredDirection;

                if (destinationIndex > pickupIndex)
                {
                    requiredDirection = "east";
                }
                else
                {
                    requiredDirection = "west";
                }

                // ====================================================
                // CALCULATE TRAVEL TIME
                // ====================================================

                float journeyTravelTime = 0.0f;

                if (requiredDirection == "east")
                {
                    for (int i = pickupIndex;
                         i < destinationIndex;
                         ++i)
                    {
                        journeyTravelTime +=
                            travelTime[i];
                    }
                }
                else
                {
                    for (int i = destinationIndex;
                         i < pickupIndex;
                         ++i)
                    {
                        journeyTravelTime +=
                            travelTime[i];
                    }
                }

                // ====================================================
                // BUS CANDIDATES
                // ====================================================

                struct Candidate
                {
                    Bus bus;
                    float arrivalTime;
                };

                vector<Candidate> candidates;

                vector<Bus> activeBuses =
                    manager.getActiveBusData();

                // ====================================================
                // CHECK ACTIVE BUSES
                // ====================================================

                for (const Bus &bus : activeBuses)
                {
                    if (!bus.isAvailable())
                        continue;

                    if (bus.isMaintenance())
                        continue;

                    if (bus.getDirection() !=
                        requiredDirection)
                    {
                        continue;
                    }

                    int busIndex =
                        bus.getCurrentStopIndex();

                    float progress =
                        bus.getProgress();

                    // =================================================
                    // HAS BUS PASSED PICKUP?
                    // =================================================

                    if (requiredDirection == "east")
                    {
                        if (busIndex > pickupIndex)
                            continue;
                    }
                    else
                    {
                        if (busIndex < pickupIndex)
                            continue;
                    }

                    // =================================================
                    // CALCULATE ETA
                    // =================================================

                    float arrivalTime = 0.0f;

                    // Bus currently at pickup.

                    if (busIndex == pickupIndex)
                    {
                        arrivalTime = 0.0f;
                    }

                    // =================================================
                    // EASTBOUND
                    // =================================================

                    else if (requiredDirection == "east")
                    {
                        arrivalTime +=
                            travelTime[busIndex] *
                            (1.0f - progress);

                        for (int i = busIndex + 1;
                             i < pickupIndex;
                             ++i)
                        {
                            arrivalTime +=
                                travelTime[i];
                        }
                    }

                    // =================================================
                    // WESTBOUND
                    // =================================================

                    else
                    {
                        arrivalTime +=
                            travelTime[busIndex - 1] *
                            (1.0f - progress);

                        for (int i = busIndex - 2;
                             i >= pickupIndex;
                             --i)
                        {
                            arrivalTime +=
                                travelTime[i];
                        }
                    }

                    candidates.push_back({bus,
                                          arrivalTime});
                }

                // ====================================================
                // SORT BY EARLIEST ARRIVAL
                // ====================================================

                sort(
                    candidates.begin(),
                    candidates.end(),
                    [](const Candidate &a,
                       const Candidate &b)
                    {
                        return a.arrivalTime <
                               b.arrivalTime;
                    });

                // ====================================================
                // RETURN TOP 3
                // ====================================================

                json buses = json::array();

                int count =
                    min(
                        3,
                        static_cast<int>(
                            candidates.size()));

                for (int i = 0;
                     i < count;
                     ++i)
                {
                    const Candidate &candidate =
                        candidates[i];

                    const Bus &bus =
                        candidate.bus;

                    int arrivalMinutes =
                        static_cast<int>(
                            ceil(
                                candidate.arrivalTime));

                    json busData = {
                        {"busNumber",
                         bus.getBusNumber()},

                        {"currentStop",
                         bus.getCurrentStop()},

                        {"currentStopIndex",
                         bus.getCurrentStopIndex()},

                        {"direction",
                         bus.getDirection()},

                        {"progress",
                         bus.getProgress()},

                        {"elapsedTime",
                         bus.getElapsedTime()},

                        {"priority",
                         bus.getPriority()},

                        {"arrivalTime",
                         candidate.arrivalTime},

                        {"arrivalMinutes",
                         arrivalMinutes}};

                    buses.push_back(busData);
                }

                // ====================================================
                // RESPONSE
                // ====================================================

                int travelMinutes =
                    static_cast<int>(
                        ceil(
                            journeyTravelTime));

                json response = {
                    {"found",
                     !candidates.empty()},

                    {"pickup",
                     from},

                    {"destination",
                     to},

                    {"pickupIndex",
                     pickupIndex},

                    {"destinationIndex",
                     destinationIndex},

                    {"direction",
                     requiredDirection},

                    {"stopCount",
                     abs(
                         destinationIndex -
                         pickupIndex)},

                    {"travelTime",
                     journeyTravelTime},

                    {"travelMinutes",
                     travelMinutes},

                    {"buses",
                     buses}};

                res.set_content(
                    response.dump(4),
                    "application/json");
            }

            catch (const exception &e)
            {
                res.status = 400;

                json response = {
                    {"message",
                     "Failed to calculate journey"},

                    {"error",
                     e.what()}};

                res.set_content(
                    response.dump(),
                    "application/json");
            }
        });

    // ========================================================
    // SEARCH BUS
    // ========================================================

    server.Post(
        "/api/search",
        [&manager](const httplib::Request &req,
                   httplib::Response &res)
        {
            addCorsHeaders(res);

            try
            {
                if (req.body.empty())
                {
                    res.status = 400;

                    res.set_content(
                        R"({"message":"Missing request body"})",
                        "application/json");

                    return;
                }

                json body =
                    json::parse(req.body);

                if (!body.contains("busNumber"))
                {
                    res.status = 400;

                    res.set_content(
                        R"({"message":"Bus number is required"})",
                        "application/json");

                    return;
                }

                string busNumber =
                    body["busNumber"].get<string>();

                Bus *bus =
                    manager.searchBusByNumber(
                        busNumber);

                if (bus == nullptr)
                {
                    res.status = 404;

                    res.set_content(
                        R"({"message":"Bus not found"})",
                        "application/json");

                    return;
                }

                json response = {
                    {"id",
                     bus->getID()},

                    {"busNumber",
                     bus->getBusNumber()},

                    {"route",
                     bus->getRoute()},

                    {"currentStop",
                     bus->getCurrentStop()},

                    {"direction",
                     bus->getDirection()},

                    {"lastStop",
                     bus->getLastStop()},

                    {"lastPark",
                     bus->getLastPark()},

                    {"capacity",
                     bus->getCapacity()},

                    {"priority",
                     bus->getPriority()},

                    {"available",
                     bus->isAvailable()},

                    {"maintenance",
                     bus->isMaintenance()},

                    {"visibleOnMap",
                     bus->isVisibleOnMap()},

                    {"currentStopIndex",
                     bus->getCurrentStopIndex()},

                    {"elapsedTime",
                     bus->getElapsedTime()},

                    {"progress",
                     bus->getProgress()}};

                res.set_content(
                    response.dump(),
                    "application/json");
            }

            catch (const exception &e)
            {
                res.status = 400;

                json response = {
                    {"message",
                     "Invalid JSON request"},

                    {"error",
                     e.what()}};

                res.set_content(
                    response.dump(),
                    "application/json");
            }
        });

    // ========================================================
    // MOVE BUS TO MAINTENANCE
    // ========================================================

    server.Post(
        "/api/maintenance",
        [&manager](const httplib::Request &req,
                   httplib::Response &res)
        {
            addCorsHeaders(res);

            try
            {
                if (req.body.empty())
                {
                    res.status = 400;

                    res.set_content(
                        R"({"message":"Missing request body"})",
                        "application/json");

                    return;
                }

                json body =
                    json::parse(req.body);

                if (!body.contains("busNumber"))
                {
                    res.status = 400;

                    res.set_content(
                        R"({"message":"Bus number is required"})",
                        "application/json");

                    return;
                }

                string busNumber =
                    body["busNumber"].get<string>();

                Bus *bus =
                    manager.searchBusByNumber(
                        busNumber);

                if (bus == nullptr)
                {
                    res.status = 404;

                    res.set_content(
                        R"({"message":"Bus not found"})",
                        "application/json");

                    return;
                }

                int busID =
                    bus->getID();

                string currentStop =
                    bus->getCurrentStop();

                string lastStop =
                    bus->getLastStop();

                string lastPark =
                    bus->getLastPark();

                manager.moveToMaintenance(
                    busID);

                json response = {
                    {"message",
                     "Bus moved to maintenance successfully"},

                    {"busNumber",
                     busNumber},

                    {"busID",
                     busID},

                    {"currentStop",
                     currentStop},

                    {"lastStop",
                     lastStop},

                    {"lastPark",
                     lastPark},

                    {"priority",
                     0},

                    {"maintenance",
                     true}};

                res.set_content(
                    response.dump(),
                    "application/json");
            }

            catch (const exception &e)
            {
                res.status = 400;

                json response = {
                    {"message",
                     "Invalid maintenance request"},

                    {"error",
                     e.what()}};

                res.set_content(
                    response.dump(),
                    "application/json");
            }
        });

    // ========================================================
    // RESTORE BUS FROM MAINTENANCE
    // ========================================================

    server.Post(
        "/api/maintenance/restore",
        [&manager](const httplib::Request &req,
                   httplib::Response &res)
        {
            addCorsHeaders(res);

            try
            {
                if (req.body.empty())
                {
                    res.status = 400;

                    res.set_content(
                        R"({"message":"Missing request body"})",
                        "application/json");

                    return;
                }

                json body =
                    json::parse(req.body);

                if (!body.contains("busNumber"))
                {
                    res.status = 400;

                    res.set_content(
                        R"({"message":"Bus number is required"})",
                        "application/json");

                    return;
                }

                string busNumber =
                    body["busNumber"].get<string>();

                Bus *bus =
                    manager.searchMaintenanceBusByNumber(
                        busNumber);

                if (bus == nullptr)
                {
                    res.status = 404;

                    res.set_content(
                        R"({"message":"Bus not found in maintenance"})",
                        "application/json");

                    return;
                }

                int busID =
                    bus->getID();

                bool restored =
                    manager.restoreFromMaintenance(
                        busID);

                if (!restored)
                {
                    res.status = 500;

                    res.set_content(
                        R"({"message":"Failed to restore bus"})",
                        "application/json");

                    return;
                }

                json response = {
                    {"message",
                     "Bus restored successfully"},

                    {"busNumber",
                     busNumber},

                    {"busID",
                     busID},

                    {"currentStop",
                     "Ratnapark Buspark"},

                    {"direction",
                     "east"},

                    {"priority",
                     1},

                    {"maintenance",
                     false},

                    {"available",
                     true}};

                res.set_content(
                    response.dump(),
                    "application/json");
            }

            catch (const exception &e)
            {
                res.status = 400;

                json response = {
                    {"message",
                     "Invalid restore request"},

                    {"error",
                     e.what()}};

                res.set_content(
                    response.dump(),
                    "application/json");
            }
        });

    // ========================================================
    // SERVER START
    // ========================================================

    cout << "========================================\n";
    cout << "      TransitNova Backend Started\n";
    cout << "========================================\n";
    cout << "Server   : http://localhost:8080\n";
    cout << "Frontend : http://localhost:8080/\n";
    cout << "Status   : http://localhost:8080/api/status\n";
    cout << "========================================\n";

    // Render provides PORT through an environment variable.
    // Locally, fall back to port 8080.

    const char *port_env =
        std::getenv("PORT");

    int port =
        port_env
            ? std::stoi(port_env)
            : 8080;

    server.listen(
        "0.0.0.0",
        port);
}