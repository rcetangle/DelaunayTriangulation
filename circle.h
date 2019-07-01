#ifndef CIRCLE_H
#define CIRCLE_H
#include "vertex.h"

class Circle
{
public:
    Circle();
    Circle(Vertex center, double radis);
    Circle &operator=(const Circle &right);

    Vertex getCenter() {return center;}
    double getRadis() {return radis;}
private:
    Vertex center;
    double radis;
};

#endif // CIRCLE_H
