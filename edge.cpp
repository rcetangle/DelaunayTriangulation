#include "edge.h"

Edge::Edge()
    :isBad(false)
{

}

Edge::Edge(Vertex a, Vertex b)
    :isBad(false)
{
    this->a = a;
    this->b = b;
}

bool Edge::operator==(const Edge &right) const
{
    return (this->a == right.a && this->b == right.b) || (this->b == right.a && this->a == right.b);
}
