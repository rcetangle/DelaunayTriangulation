#include "vertex.h"
#include "stdlib.h"
#include "math.h"
#include "float.h"

Vertex::Vertex()
{
}

Vertex::Vertex(double x, double y)
    :x(x), y(y)
{
}

Vertex::Vertex(const Vertex &copy)
    :x(copy.x), y(copy.y)
{
}

Vertex::~Vertex()
{
}

double Vertex::distance(const Vertex &right)
{
    return sqrt(distance2(right));
}

double Vertex::distance2(const Vertex &right)
{
    return pow(x-right.x, 2) + pow(y-right.y, 2);
}

Vertex Vertex::operator+(const Vertex &right) const
{
    return Vertex(x+right.x, y+right.y);
}

Vertex Vertex::operator-(const Vertex &right) const
{
    return Vertex(x-right.x, y-right.y);
}

Vertex Vertex::operator*(const int c) const
{
    return Vertex(x*c, y*c);
}

Vertex Vertex::operator*(const double c) const
{
    return Vertex(x*c, y*c);
}

bool Vertex::operator==(const Vertex &right) const
{
    return abs(x-right.x) <= DBL_EPSILON && abs(y-right.y) <= DBL_EPSILON;
}

Vertex &Vertex::operator=(const Vertex &right)
{
    this->x = right.x;
    this->y = right.y;
    return *this;
}

void Vertex::setX(const double x)
{
    this->x = x;
}

void Vertex::setY(const double y)
{
    this->y = y;
}

double Vertex::getX() const
{
    return x;
}

double Vertex::getY() const
{
    return y;
}

void Vertex::set(const double x, const double y)
{
    setX(x);
    setY(y);
}
