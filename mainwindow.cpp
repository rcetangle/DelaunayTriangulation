#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cstdlib>
#include <QDebug>
#include <QPainter>
#include "delaunaytriangulation.h"
#include "vertex.h"
#include "delaunay.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    processIndex(0)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


// create random points.
static DelaunayTriangulation _delaunay;
static int pointIndex = 0;

std::vector<delaunay::Point<float>> dePoints;

void MainWindow::on_actionRandomPoints_triggered()
{
    clear();
    unsigned pointsCnt = 10;
    const int minimum = 50;
    const int maximum = min(500, 500) - 2*minimum;
    qDebug() << "Create " << pointsCnt << " points, range from " << minimum << " to " << maximum;
    for (unsigned i = 0; i < pointsCnt; i++)
    {
        points.push_back(QPoint(rand()%maximum+minimum, rand()%maximum+minimum)); // create points
    }
//    _delaunay.setDelegate(this);

    std::vector<Vertex> inputs;
    for (auto iter = points.begin(); iter != points.end(); iter++)
    {
        inputs.push_back(Vertex(iter->x(), iter->y()));
    }
    _delaunay.createSuperTriangle(inputs);

    for (auto iter = inputs.begin(); iter != inputs.end(); iter++)
    {
        _delaunay.addPoint(*iter);
    }

    std::vector<Triangle> ret = _delaunay.getTriangulation();
    for (auto iter = ret.begin(); iter != ret.end(); iter++)
    {
        super.push_back(iter->getA().convertToQPoint());
        super.push_back(iter->getB().convertToQPoint());
        super.push_back(iter->getC().convertToQPoint());
    }

    update(); // 通知绘制
}

void MainWindow::on_actionAddPoint_triggered()
{
//    if (pointIndex >= points.size())
//    {
//        std::vector<Triangle> ret = _delaunay.getTriangulation();
//        for (auto iter = ret.begin(); iter != ret.end(); iter++)
//        {
//            super.push_back(iter->getA().convertToQPoint());
//            super.push_back(iter->getB().convertToQPoint());
//            super.push_back(iter->getC().convertToQPoint());
//        }
//        update(); // 通知绘制
//        return;
//    }
//    process.clear();
//    circle.clear();
//    radis.clear();
//    _delaunay.addPoint(Vertex(points[pointIndex].x(), points[pointIndex].y()));
//    pointIndex++;
//    update(); // 通知绘制
}

void MainWindow::addLine(QPointF a, QPointF b)
{
    process.push_back(a);
    process.push_back(b);
}

void MainWindow::addCircle(QPointF a, double radis)
{
    circle.push_back(a);
    this->radis.push_back(radis);
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPen pen; //画笔


    // 绘制过程外接圆
    int tmp2 = 0;
    for (auto iter = circle.begin();  iter != circle.end(); iter++, tmp2++)
    {
        painter.drawEllipse(*iter, radis[tmp2], radis[tmp2]);
    }

    // 绘制过程线段
    int tmp = 0;
    for (auto iter = process.begin(); iter != process.end(); iter++)
    {
        if (tmp%3 == 0)
        {
            pen.setColor(QColor(rand()%50+200, rand()%50+200, rand()%50+200));
            painter.setPen(pen);
        }
        painter.drawLine(*iter, *(iter+1));
        iter++;
        tmp++;
    }

    pen.setColor(QColor(255, 0, 0));
    painter.setPen(pen); //添加画笔
    // 画个结果三角形
    for (auto iter = super.begin(); iter != super.end(); iter++)
    {
        auto a = *iter, b = *(iter+1), c = *(iter+2);
        painter.drawLine(a, b);
        painter.drawLine(a, c);
        painter.drawLine(b, c);
        iter += 2;
    }

//    // 画出对照代码的三角形
//    if (super.size() > 0)
//    {
//        pen.setColor(QColor(0, 255, 0));
//        painter.setPen(pen); //添加画笔
//        dePoints.clear();
//        for (auto iter = points.begin(); iter != points.end(); iter++)
//        {
//            dePoints.push_back(delaunay::Point<float>(iter->x(), iter->y()));
//        }
//        delaunay::Delaunay<float> ret = delaunay::triangulate(dePoints);
//        for (auto iter = ret.triangles.begin(); iter != ret.triangles.end(); iter++)
//        {
//            painter.drawLine(QPointF(iter->p0.x, iter->p0.y), QPointF(iter->p1.x, iter->p1.y));
//            painter.drawLine(QPointF(iter->p0.x, iter->p0.y), QPointF(iter->p2.x, iter->p2.y));
//            painter.drawLine(QPointF(iter->p1.x, iter->p1.y), QPointF(iter->p2.x, iter->p2.y));
//        }
//    }

    // 绘制目标插入点
    pen.setColor(QColor(0, 255, 0));
    painter.setPen(pen); //添加画笔
    for (int i = 0; i < points.size(); i++)
    {
        painter.drawRoundRect(points[i].x()-3, points[i].y()-3, 6, 6);
    }
}

void MainWindow::clear()
{
    pointIndex = 0;
    points.clear();
    process.clear();
    circle.clear();
    radis.clear();
    super.clear();
}
