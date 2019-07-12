#include "idw.h"
#include <algorithm>
#include <cmath>
/*
 * IDW是反距离权重插值法。
 * 给出n对控制点组{p1,...,pn}, {q1,...,qn} ，求出一个连续函数f(x)使，
 * f(x)=∑wi(x)fi(x)
 *
 * 其中wi(x)是未知点x与控制点的距离的权重函数。
 * fi(x)是 pi和qi的局部近似函数。即 fi(pi) = qi
 * 这里简单取fi(x) = qi + (x-pi)
 * 具体看相关论文。
 */
IDW::IDW()
    :u(2)
{

}

IDW::IDW(double u)
    :u(u)
{
}

/*
 * 根据给出的控制点对，输出变换结果
 */
Vertex IDW::warp(const std::vector<Vertex> &pointP,const std::vector<Vertex> &pointQ, Vertex &x)
{
    Vertex ret(0, 0);
    std::vector<Vertex>::size_type minSize = std::min(pointP.size(), pointQ.size());
    for (std::vector<Vertex>::size_type i = 0; i < minSize; i++)
    {
        double wi = wi_func(x, pointP, i);
        Vertex fi = fi_func(x, pointP, pointQ, i);
        ret = ret + fi*wi;
    }
    return ret;
}

Vertex IDW::fi_func(const Vertex &x, const std::vector<Vertex> &p, const std::vector<Vertex> &q, const std::vector<Vertex>::size_type i)
{
    Vertex ret(q[i].getX() + (x.getX()-p[i].getX()), q[i].getY() + (x.getY()-p[i].getY()));
//    Vertex ret(q[i]);
    return ret;
}

double IDW::wi_func(Vertex &x, const std::vector<Vertex> &p, const std::vector<Vertex>::size_type i)
{
    assert(i < p.size());
    double sum = 0.0;
    for (auto iter = p.begin(); iter != p.end(); iter++)
        sum += pow(x.distance(*iter), -this->u);
    return pow(x.distance(p[i]), -this->u)/sum;
}
