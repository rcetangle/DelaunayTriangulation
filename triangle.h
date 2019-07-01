#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vertex.h"
#include "circle.h"
/*
 * 一个三角形
 */
class Triangle
{
public:
    Triangle();
    Triangle(const Vertex a, const Vertex b, const Vertex c);
    Triangle(const Triangle& copy);
    virtual ~Triangle();

    bool operator==(const Triangle &right) const;
    Triangle &operator=(const Triangle &right);

    bool isVertexInCircumcircle(const Vertex &ver); // 判断一个点是否在三角形的外接圆中
    bool isContainEdge(const Vertex &a, const Vertex &b) const; // 判断该边在不在三角形里
    bool isContainVertex(const Vertex &a) const; // 判断三角形拥有该顶点
    void setVertex(const Vertex a, const Vertex b, const Vertex c);

    Vertex getA() const {return a;}
    Vertex getB() const {return b;}
    Vertex getC() const {return c;}
    Circle getCircumcircle() {return circumcircle;}

    bool isBad;
private:
    void calculateCircumCircle();
    Vertex a;
    Vertex b;
    Vertex c;

    Circle circumcircle;
};

#endif // TRIANGLE_H
