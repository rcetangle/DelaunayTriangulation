#include "delaunaytriangulation.h"
#include <QDebug>
#include "edge.h"
#include "triangle.h"
#include "mainwindow.h"

/*
 * using Bowyer-Watson algorithm
 * fake code is below:
 * function BowyerWatson (pointList)
      // pointList is a set of coordinates defining the points to be triangulated
      triangulation := empty triangle mesh data structure
      add super-triangle to triangulation // must be large enough to completely contain all the points in pointList
      for each point in pointList do // add all the points one at a time to the triangulation
         badTriangles := empty set
         for each triangle in triangulation do // first find all the triangles that are no longer valid due to the insertion
            if point is inside circumcircle of triangle
               add triangle to badTriangles
         polygon := empty set
         for each triangle in badTriangles do // find the boundary of the polygonal hole
            for each edge in triangle do
               if edge is not shared by any other triangles in badTriangles
                  add edge to polygon
         for each triangle in badTriangles do // remove them from the data structure
            remove triangle from triangulation
         for each edge in polygon do // re-triangulate the polygonal hole
            newTri := form a triangle from edge to point
            add newTri to triangulation
      for each triangle in triangulation // done inserting points, now clean up
         if triangle contains a vertex from original super-triangle
            remove triangle from triangulation
      return triangulation
 */
DelaunayTriangulation::DelaunayTriangulation()
{
}

void DelaunayTriangulation::addPoint(const Vertex &vec)
{
    qDebug() << "-------- add new point:";
    qDebug() << "waiting triangulation size " << triangulation.size();

    // get all the bad triangles.
    std::vector<Edge> polygon;
    int basTriagles = 0;
    for (auto triIter = triangulation.begin(); triIter != triangulation.end(); triIter++)
    {
        if (triIter->isVertexInCircumcircle(vec))
        {
            basTriagles++;
            triIter->isBad = true;
            polygon.push_back(Edge(triIter->getA(), triIter->getB()));
            polygon.push_back(Edge(triIter->getB(), triIter->getC()));
            polygon.push_back(Edge(triIter->getA(), triIter->getC()));
        }
        else
        {
            triIter->isBad = false;
        }
    }
    qDebug() << "bad triangles size: " << basTriagles;
    qDebug() << "waiting edges size: " << polygon.size();

    // remove bad triangles from triangulations.
    triangulation.erase(remove_if(triangulation.begin(), triangulation.end(), [](Triangle &tri){
                            return tri.isBad;
                        }), triangulation.end());

    qDebug() << "current triangulation size " << triangulation.size();
    int rhha = 0;
    for (auto triIter = triangulation.begin(); triIter != triangulation.end(); triIter++)
    {
        if (triIter->isBad)
        {
            qDebug() << "已经erase了，为啥还有isBad" << rhha;
        }
        rhha++;
    }
    // find remove bad edges in polygon
    for (auto egIter = polygon.begin(); egIter != polygon.end(); egIter++)
    {
        for (auto egIter2 = egIter+1; egIter2 != polygon.end(); egIter2++)
        {
            if ((*egIter) == (*egIter2))
            {
                egIter->isBad = true;
                egIter2->isBad = true;
            }
        }
    }
    polygon.erase(remove_if(polygon.begin(), polygon.end(), [](Edge &e){
                      return e.isBad;
                  }), polygon.end());
    qDebug() << "good egdes size: " << polygon.size();

    // add the new point in polygon, and form new triangles.
    for (auto egIter = polygon.begin(); egIter != polygon.end(); egIter++)
    {
        Triangle newTri(egIter->getA(), egIter->getB(), vec);
        triangulation.push_back(newTri);
    }
    qDebug() << "latest triangulation size " << triangulation.size()<< endl;

    for (auto triIter = triangulation.begin(); triIter != triangulation.end(); triIter++)
    {
        if (delegate)
        {
            MainWindow *main = static_cast<MainWindow*>(delegate);
            main->addLine(triIter->getA().convertToQPoint(), triIter->getB().convertToQPoint());
            main->addLine(triIter->getB().convertToQPoint(), triIter->getC().convertToQPoint());
            main->addLine(triIter->getA().convertToQPoint(), triIter->getC().convertToQPoint());
//            main->addCircle(triIter->getCircumcircle().getCenter().convertToQPoint(), triIter->getCircumcircle().getRadis());
        }
    }
}

std::vector<Triangle> DelaunayTriangulation::getTriangulation()
{
    // remove all the triangles contains vertexs in super triangle.
    triangulation.erase(std::remove_if(triangulation.begin(), triangulation.end(), [this](Triangle &tri){
                            return tri.isContainVertex(super->getA()) || tri.isContainVertex(super->getB()) || tri.isContainVertex(super->getC());
                        }), triangulation.end());
    qDebug() << "final triangulation size " << triangulation.size();
    return triangulation;
}

Triangle DelaunayTriangulation::createSuperTriangle(const std::vector<Vertex> &inputs)
{
    if (super)
        delete super;

    // get minX minY maxX maxY of input vertexs.
    double minX = inputs[0].getX();
    double minY = inputs[0].getY();
    double maxX = minX, maxY = minY;
    for (auto iter = inputs.begin(); iter != inputs.end(); iter++)
    {
        if (iter->getX() < minX) minX = iter->getX();
        if (iter->getY() < minY) minY = iter->getY();
        if (iter->getX() > maxX) maxX = iter->getX();
        if (iter->getY() > maxY) maxY = iter->getY();
    }

//    qDebug() << "super is " << minX << ", " << minY << " ==> " << maxX << ", " << maxY << endl;

    // create an isosceles triangle that is large enough.
    double delta = 50;
    double height = 2*delta + maxY-minY;
    double width = height * (maxX-minX) / delta;

    super = new Triangle(Vertex(minX-width/2, minY-delta),
                 Vertex(maxX+width/2, minY-delta),
                 Vertex((maxX+minX)/2, maxY+delta));

    triangulation.clear();
    triangulation.push_back(*super);

    return *super;
}

void DelaunayTriangulation::setDelegate(void *d)
{
    delegate = d;
}
