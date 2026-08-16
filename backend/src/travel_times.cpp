#include "../include/travel_times.h"

const float travelTime[NUM_TRANSITIONS] = {
    12.49f, // Stop 0  <-> Stop 1  (Ratnapark Buspark -> Singha Durbar)
    0.97f,  // Stop 1  <-> Stop 2  (Singha Durbar -> Maitighar)
    4.22f,  // Stop 2  <-> Stop 3  (Maitighar -> Bijulibazar)
    3.00f,  // Stop 3  <-> Stop 4  (Bijulibazar -> Baneshwor)
    10.91f, // Stop 4  <-> Stop 5  (Baneshwor -> Civil)
    1.39f,  // Stop 5  <-> Stop 6  (Civil -> Shantinagar)
    1.06f,  // Stop 6  <-> Stop 7  (Shantinagar -> Tinkune)
    3.48f,  // Stop 7  <-> Stop 8  (Tinkune -> Koteshwor)
    2.11f,  // Stop 8  <-> Stop 9  (Koteshwor -> Jadibuti)
    3.05f,  // Stop 9  <-> Stop 10 (Jadibuti -> Lokanthali)
    3.10f,  // Stop 10 <-> Stop 11 (Lokanthali -> Kaushaltar)
    1.96f,  // Stop 11 <-> Stop 12 (Kaushaltar -> Gatthaghar)
    9.09f,  // Stop 12 <-> Stop 13 (Gatthaghar -> Thimi)
    2.47f,  // Stop 13 <-> Stop 14 (Thimi -> Bode Chardobato)
    1.97f,  // Stop 14 <-> Stop 15 (Bode Chardobato -> Bode Planning)
    1.00f,  // Stop 15 <-> Stop 16 (Bode Planning -> Jaya School)
    2.49f   // Stop 16 <-> Stop 17 (Jaya School -> Bode Buspark)
};