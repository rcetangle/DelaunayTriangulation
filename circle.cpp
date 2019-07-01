#include "circle.h"

Circle::Circle()
{

}

Circle::Circle(Vertex center, double radis)
    :center(center), radis(radis)
{

}

Circle &Circle::operator=(const Circle &right)
{
    center = right.center;
    radis = right.radis;
    return *this;
}
