#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cstdlib>
#include <QDebug>
#include <QPainter>
#include <QFileDialog>
#include "delaunaytriangulation.h"
#include "vertex.h"
#include "idw.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    bgImg(nullptr),
    oriImg(nullptr)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// create random points & using Delaunay triangulation
static DelaunayTriangulation _delaunay;
void MainWindow::on_actionRandomPoints_triggered()
{
    clear();
    unsigned pointsCnt = 10;
    const int minX = imgRect.x();
    const int minY = imgRect.y();
    const int maxX = max(100, imgRect.width());
    const int maxY = max(100, imgRect.height());
    qDebug() << "Create " << pointsCnt << " points, range from " << minX << " to " << maxX+minX;
    std::vector<Vertex> inputs;
    for (unsigned i = 0; i < pointsCnt; i++)
    {
        inputs.push_back(Vertex(rand()%maxX+minX, rand()%maxY+minY)); // create points
    }
//    _delaunay.setDelegate(this);
    _delaunay.createSuperTriangle(inputs);
    for (auto iter = inputs.begin(); iter != inputs.end(); iter++)
    {
        _delaunay.addPoint(*iter);
    }

    std::vector<Triangle> ret = _delaunay.getTriangulation();
    for (auto iter = ret.begin(); iter != ret.end(); iter++)
    {
        triangulation.push_back(iter->getA().convertToQPoint());
        triangulation.push_back(iter->getB().convertToQPoint());
        triangulation.push_back(iter->getC().convertToQPoint());
    }
    update(); // 通知绘制
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

    if (!bgImg && oriImg)
    {
        painter.drawImage(imgRect, *oriImg);
        painter.drawRect(imgRect.x(), imgRect.y(), imgRect.width(), imgRect.height());
    }
    else if (bgImg)
    {
        painter.drawImage(imgRect, *bgImg);
        painter.drawRect(imgRect.x(), imgRect.y(), imgRect.width(), imgRect.height());
    }

    QPen pen; //画笔
    // 绘制过程外接圆
    std::vector<double>::size_type tmp2 = 0;
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
    for (auto iter = triangulation.begin(); iter != triangulation.end(); iter++)
    {
        auto a = *iter, b = *(iter+1), c = *(iter+2);
        painter.drawLine(a, b);
        painter.drawLine(a, c);
        painter.drawLine(b, c);
        iter += 2;
    }

    // 画pointP pointQ的连线
    pen.setColor(QColor(0, 255, 0));
    painter.setPen(pen);
    for (auto iter = pointP.begin(), iter2 = pointQ.begin(); iter != pointP.end(), iter2 != pointQ.end(); iter++, iter2++)
    {
        painter.drawLine(*iter, *iter2);
    }
    for (auto iter = pointP.begin(); iter != pointP.end(); iter++)
    {
        painter.drawRect(iter->x()-1, iter->y()-1, 2, 2);
    }
    pen.setColor(QColor(255, 0, 0));
    painter.setPen(pen);
    for (auto iter = pointQ.begin(); iter != pointQ.end(); iter++)
    {
        painter.drawRect(iter->x()-1, iter->y()-1, 2, 2);
    }
}

void MainWindow::clear()
{
    triangulation.clear();
    process.clear();
    circle.clear();
    radis.clear();
    pointP.clear();
    pointQ.clear();
    if (oriImg)
    {
        delete oriImg;
        oriImg = nullptr;
    }
    if (bgImg)
    {
        delete bgImg;
        bgImg = nullptr;
    }

}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton) //鼠标左键按下
    {
        QPoint pos = getPointInImgRect(event->pos());
        pointP.push_back(pos);
        pointQ.push_back(pos);
        update();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton) //鼠标左键按下
    {
        pointQ[pointQ.size()-1] = getPointInImgRect(event->pos());
        update();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton) //鼠标左键按下
    {
        pointQ[pointQ.size()-1] = getPointInImgRect(event->pos());
        update();
    }
}

QPoint MainWindow::getPointInImgRect(QPoint pos)
{
    if (pos.x() < imgRect.left())
    {
        pos.setX(imgRect.left());
    }
    else if (pos.x() > imgRect.right())
    {
        pos.setX(imgRect.right());
    }
    if (pos.y() < imgRect.top())
    {
        pos.setY(imgRect.top());
    }
    else if (pos.y() > imgRect.bottom())
    {
        pos.setY(imgRect.bottom());
    }
    return pos;
}

void MainWindow::on_actionBFS_triggered()
{

}

void MainWindow::on_actionIDW_triggered()
{
    if (!oriImg) return;

    // 置零
    repairedPoints.assign(imgRect.height()*imgRect.width(), 0);

    IDW idw(2.0);
    std::vector<Vertex> p;
    // pointP和pointQ都在imgRect的范围里
    for (auto iter = pointP.begin(); iter != pointP.end(); iter++)
    {
        p.push_back(Vertex(iter->x()-imgRect.left(), iter->y()-imgRect.top()));
    }
    std::vector<Vertex> q;
    for (auto iter = pointQ.begin(); iter != pointQ.end(); iter++)
    {
        q.push_back(Vertex(iter->x()-imgRect.left(), iter->y()-imgRect.top()));
    }

    unsigned char *bits = new unsigned char[oriImg->sizeInBytes()]();
    unsigned char *oriBits = oriImg->bits();
    int bytesPerLine = oriImg->bytesPerLine();
    int pixelLen = oriImg->depth() / 8;
    for (int i = 0; i < imgRect.width(); i++)
    {
        for (int j = 0; j < imgRect.height(); j++)
        {
            Vertex x(i, j);
            Vertex ret=idw.warp(p, q, x);
            if (ret.getX() <= imgRect.width() && ret.getX() >= 0 && ret.getY() <= imgRect.height() && ret.getY() >= 0)
            {
                int retX = ret.getX();
                int retY = ret.getY();
                for (int rgbIdx = 0; rgbIdx < pixelLen; rgbIdx++)
                {
                    bits[retX*pixelLen+retY*bytesPerLine+rgbIdx] = oriBits[i*pixelLen+j*bytesPerLine+rgbIdx];
                }

                repairedPoints[retX+retY*imgRect.width()] = 1;
            }
        }
    }
    if (bgImg)
    {
        delete bgImg;
        bgImg = nullptr;
    }
    bgImg = new QImage(bits, imgRect.width(), imgRect.height(), oriImg->bytesPerLine(), QImage::Format_RGB32);
    qDebug() << "bgImg" << bgImg->format();

    delete []bits;
    update();
}

void MainWindow::on_actionaddImg_triggered()
{
    clear();
    // Open file
    QString fileName = "/Users/ios/Desktop/test.jpg";//QFileDialog::getOpenFileName(this, tr("Read Image"), ".", tr("Images(*.bmp *.png *.jpg)"));

    // Load file
    if (!fileName.isEmpty())
    {
        QImage tmp(fileName);
        oriImg = new QImage(tmp.convertToFormat(QImage::Format_RGB32)); // make sure the format of oriImg is RGB32
        int cx = this->size().width()/2 - oriImg->width()/2;
        int cy = this->size().height()/2 - oriImg->height()/2;
        imgRect = QRect(cx, cy, oriImg->width(), oriImg->height());
        qDebug() << "original" << oriImg->format() << oriImg->bytesPerLine() << oriImg->depth() << oriImg->width() << imgRect.height();
        update();
    }
}

// 计算临近x点像素
unsigned char averageChar(unsigned char* bits,
                          const int row, const int col,
                          const int width, const int height,
                          const int pixelLen, const int bytes,
                          const int rgbIdx)
{

    unsigned cnt = 4;
    unsigned char b[]{
        bits[row*pixelLen+(col==0?col:col-1)*bytes+rgbIdx], // top
        bits[(row==0?row:row-1)*pixelLen+col*bytes+rgbIdx], // left
        bits[(row==width-1?row:row+1)*pixelLen+col*bytes+rgbIdx], // right
        bits[row*pixelLen+(col==height-1?col:col+1)*bytes+rgbIdx], // bottom
    };
    // unsigned char [0-255]
    unsigned sum = 0;
    for (unsigned i = 0; i < cnt; i++)
    {
        sum += b[i];
    }
    return sum/cnt;
}

void MainWindow::on_actionFixBlank_triggered()
{
    if (!bgImg) return;
    unsigned char *bits = bgImg->bits();
    int bytesPerLine = bgImg->bytesPerLine();
    int pixelLen = bgImg->depth() / 8;
    int width = bgImg->width(), height = bgImg->height();
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            if (repairedPoints[i+j*width] == 0)
            {
                // 使用邻近像素插值填补空白区域
                for (int rgbIdx = 0; rgbIdx < pixelLen; rgbIdx++)
                {
                    bits[i*pixelLen+j*bytesPerLine+rgbIdx] = averageChar(bits, i, j, width, height, pixelLen, bytesPerLine, rgbIdx);
                }
            }
        }
    }
    update();
}
