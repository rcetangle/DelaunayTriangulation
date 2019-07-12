#ifndef IDW_H
#define IDW_H

#include <vector>
#include "vertex.h"

class IDW
{
public:
    IDW();
    IDW(double u);
    Vertex warp(const std::vector<Vertex> &pointP, const std::vector<Vertex> &pointQ, Vertex &x);
private:
    Vertex fi_func(const Vertex &x, const std::vector<Vertex> &p, const std::vector<Vertex> &q, const std::vector<Vertex>::size_type i);
    double wi_func(Vertex &x, const std::vector<Vertex> &p, const std::vector<Vertex>::size_type i);

    double u;
};

#endif // IDW_H
