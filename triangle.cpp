#include "triangle.h"
#include <QtDebug>

Triangle::Triangle()
    :isBad(false)
{
}

Triangle::Triangle(const Vertex a, const Vertex b, const Vertex c)
    :isBad(false), a(a), b(b), c(c)
{
    calculateCircumCircle();
}

Triangle::Triangle(const Triangle &copy)
    :isBad(false), a(copy.a), b(copy.b), c(copy.c)
{
    calculateCircumCircle();
}

Triangle::~Triangle()
{
}

bool Triangle::operator==(const Triangle &right) const
{
    return isContainVertex(right.a) && isContainVertex(right.b) && isContainVertex(right.c);
}

Triangle &Triangle::operator=(const Triangle &right)
{
    setVertex(right.a, right.b, right.c);
    return *this;
}

bool Triangle::isContainEdge(const Vertex &a, const Vertex &b) const
{
    return isContainVertex(a) && isContainVertex(b);
}

bool Triangle::isContainVertex(const Vertex &a) const
{
    return this->a == a || this->b == a || this->c == a;
}

/*
 * 求获取三角形的外接圆，设圆心是(x, y)
 * 因为三角形三点都在圆上，得到两等式:
 * (ax-x)(ax-x) + (ay-y)(ay-y) = (bx-x)(bx-x) + (by-y)(by-y)
 * (ax-x)(ax-x) + (ay-y)(ay-y) = (cx-x)(cx-x) + (cy-y)(cy-y)
 * 化简后：
 * (2ax-2bx)(x) + (2ay-2by)(y) = (ax)(ax) - (bx)(bx) + (ay)(ay) - (by)(by)
 * (2ax-2cx)(x) + (2ay-2cy)(y) = (ax)(ax) - (cx)(cx) + (ay)(ay) - (cy)(cy)
 * 令:
 * A1 = (2ax-2bx), B1 = (2ay-2by), C1 = (ax)(ax) - (bx)(bx) + (ay)(ay) - (by)(by)
 * A2 = (2ax-2cx), B2 = (2ay-2cy), C2 = (ax)(ax) - (cx)(cx) + (ay)(ay) - (cy)(cy)
 * 并组成二阶行列式：
 * |A1 B1| |x|   |C1|
 * |     |*| | = |  |
 * |A2 B2| |y|   |C2|
 * 令:
 *     |A1 B1|
 * D = |     |
 *     |A2 B2|
 * 克拉默法则： xi = |Di| / |D|，其中Di是D的第i列被C替换的行列式。
 *
 * 故最终圆心(x, y)为：
 * x = ((C1*B2)-(C2*B1))/((A1*B2)-(A2*B1))
 * y = ((A1*C2)-(A2*C1))/((A1*B2)-(A2*B1))
 */
void Triangle::calculateCircumCircle()
{
    double ax = a.getX(), ay = a.getY();
    double bx = b.getX(), by = b.getY();
    double cx = c.getX(), cy = c.getY();

    double A1 = 2*(ax-bx), B1 = 2*(ay-by), C1 = ax*ax - bx*bx + ay*ay - by*by;
    double A2 = 2*(ax-cx), B2 = 2*(ay-cy), C2 = ax*ax - cx*cx + ay*ay - cy*cy;

    double circleX = ((C1*B2)-(C2*B1))/((A1*B2)-(A2*B1));
    double circleY = ((A1*C2)-(A2*C1))/((A1*B2)-(A2*B1));
    Vertex center = Vertex(circleX, circleY);

//    qDebug() << "x=" << circleX << "y=" << circleY << "r=" << center.distance(a);
    circumcircle = Circle(center, center.distance(a));
}
bool Triangle::isVertexInCircumcircle(const Vertex &ver)
{
    return circumcircle.getRadis() >= circumcircle.getCenter().distance(ver);
}

void Triangle::setVertex(const Vertex a, const Vertex b, const Vertex c)
{
    this->a = a;
    this->b = b;
    this->c = c;
    calculateCircumCircle();
}
