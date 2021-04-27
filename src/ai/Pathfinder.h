#pragma once

class Pathfinder
{
private:
    struct Node;
    int dist(Node, Node);
public:
    int compare(Node, Node);
    bool operator==(const Node& rhs);
    void compute(Node, Node, Terrain);
};