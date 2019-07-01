#ifndef DELAUNAYTRIANGULATION_H
#define DELAUNAYTRIANGULATION_H

#include <vector>
#include "vertex.h"
#include "triangle.h"

class DelaunayTriangulation
{
public:
    DelaunayTriangulation();

    Triangle createSuperTriangle(const std::vector<Vertex> &inputs); // create the super triangle.
    void addPoint(const Vertex &vec);
    std::vector<Triangle> getTriangulation();

    void setDelegate(void* d);
private:
    void *delegate;
    Triangle *super;

    std::vector<Triangle> triangulation;
};

#endif // DELAUNAYTRIANGULATION_H
