#pragma once

#include "../game/Terrain.h"

class Pathfinder
{
private:
    struct Node;
    int dist(Node, Node);
public:
    int compare(Node, Node);
    void compute(Node, Node, Terrain);
};