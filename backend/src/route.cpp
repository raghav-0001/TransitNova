#include "../include/route.h"

Route::Route()
{
    id = 0;
    name = "";

    stops[0] = "Ratnapark Buspark";
    stops[1] = "Singha Durbar";
    stops[2] = "Maitighar";
    stops[3] = "Bijulibazar";
    stops[4] = "Baneshwor";
    stops[5] = "Civil";
    stops[6] = "Shantinagar";
    stops[7] = "Tinkune";
    stops[8] = "Koteshwor";
    stops[9] = "Jadibuti";
    stops[10] = "Lokanthali";
    stops[11] = "Kaushaltar";
    stops[12] = "Gatthaghar";
    stops[13] = "Thimi";
    stops[14] = "Bode Chardobato";
    stops[15] = "Bode Planning";
    stops[16] = "Jaya School";
    stops[17] = "Bode Buspark";
}

Route::Route(
    int id,
    const std::string &name)
{
    this->id = id;
    this->name = name;

    stops[0] = "Ratnapark Buspark";
    stops[1] = "Singha Durbar";
    stops[2] = "Maitighar";
    stops[3] = "Bijulibazar";
    stops[4] = "Baneshwor";
    stops[5] = "Civil";
    stops[6] = "Shantinagar";
    stops[7] = "Tinkune";
    stops[8] = "Koteshwor";
    stops[9] = "Jadibuti";
    stops[10] = "Lokanthali";
    stops[11] = "Kaushaltar";
    stops[12] = "Gatthaghar";
    stops[13] = "Thimi";
    stops[14] = "Bode Chardobato";
    stops[15] = "Bode Planning";
    stops[16] = "Jaya School";
    stops[17] = "Bode Buspark";
}

int Route::getID() const
{
    return id;
}

std::string Route::getName() const
{
    return name;
}

void Route::setID(int id)
{
    this->id = id;
}

void Route::setName(const std::string &name)
{
    this->name = name;
}

int Route::getStopCount() const
{
    return NUM_STOPS;
}

std::string Route::getStop(int index) const
{
    if (index < 0 || index >= NUM_STOPS)
        return "";

    return stops[index];
}

int Route::getStopIndex(const std::string &stopName) const
{
    for (int i = 0; i < NUM_STOPS; i++)
    {
        if (stops[i] == stopName)
            return i;
    }

    return -1;
}