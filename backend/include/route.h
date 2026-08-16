#ifndef ROUTE_H
#define ROUTE_H

#include <string>

class Route
{
private:
    int id;
    std::string name;

    static const int NUM_STOPS = 18;

    std::string stops[NUM_STOPS];

public:
    Route();

    Route(
        int id,
        const std::string &name);

    int getID() const;
    std::string getName() const;

    void setID(int id);
    void setName(const std::string &name);

    // Stop information
    int getStopCount() const;
    std::string getStop(int index) const;
    int getStopIndex(const std::string &stopName) const;
};

#endif