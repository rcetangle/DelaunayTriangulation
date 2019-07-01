#ifndef EDGE_H
#define EDGE_H

#include "vertex.h"

class Edge
{
public:
    Edge();
    Edge(Vertex a, Vertex b);

    bool operator==(const Edge &right) const;

    Vertex getA() const {return a;}
    Vertex getB() const {return b;}

    bool isBad;
private:
    Vertex a;
    Vertex b;
};

#endif // EDGE_H
