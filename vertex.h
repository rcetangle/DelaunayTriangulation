#ifndef VERTEX_H
#define VERTEX_H

#include <QPointF>
/*
 * 二维顶点
 */
class Vertex
{
public:
    Vertex();
    Vertex(double x, double y);
    Vertex(const Vertex &copy);
    virtual ~Vertex();

    double distance(const Vertex& right); // 计算两点的距离
    double distance2(const Vertex& right); // 距离的平方
    // operators
    Vertex operator+(const Vertex &right) const; // 任何不会修改数据成员的函数，都应该被声明为const函数。
    Vertex operator-(const Vertex &right) const; // 加减乘除等运算符重载函数，一般情况下都会被声明为const函数。
    Vertex operator*(const int c) const; // 数乘
    Vertex operator*(const double c) const; // 数乘
    bool operator==(const Vertex &right) const;
    Vertex &operator=(const Vertex &right);       // 赋值操作就不需要声明为const了

    void setX(const double x);
    void setY(const double y);
    double getX() const;
    double getY() const;
    void set(const double x, const double y);

    QPointF convertToQPoint() const {return QPointF(x, y);}
private:
    double x;
    double y;
};

#endif // VERTEX_H
