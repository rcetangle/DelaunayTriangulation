#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QMouseEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void addLine(QPointF a, QPointF b);
    void addCircle(QPointF a, double radis);

protected:
    virtual void paintEvent(QPaintEvent *event);
    void clear(); // clear all data.

    // mouse event
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void on_actionRandomPoints_triggered();

    void on_actionBFS_triggered();

    void on_actionIDW_triggered();

    void on_actionaddImg_triggered();

    QPoint getPointInImgRect(QPoint p);

    void on_actionFixBlank_triggered();

private:
    Ui::MainWindow *ui;

    std::vector<QPointF> triangulation;
    std::vector<QPointF> process;
    std::vector<QPointF> circle;
    std::vector<double> radis;

    std::vector<QPoint> pointP;
    std::vector<QPoint> pointQ;

    std::vector<int> repairedPoints;
    QImage *bgImg;
    QImage *oriImg;
    QRect imgRect;
};

#endif // MAINWINDOW_H
