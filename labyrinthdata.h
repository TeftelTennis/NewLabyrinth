#include "treasure.h"
#include <vector>

#ifndef LABYRINTHDATA_H
#define LABYRINTHDATA_H

using namespace std;

class LabyrinthData {
public:
    double wallProb;
    vector<Treasure> treasures;
    bool useRandomTreasure;
    int treasureCount;
    double loveToilets; // Если хочет ставить в сартир
    double staticTreasureProb;
    bool canPutTreasureTogether;
};

#endif // LABYRINTHDATA_H
