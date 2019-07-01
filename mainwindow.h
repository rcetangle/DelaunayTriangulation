#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>

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

private slots:
    void on_actionRandomPoints_triggered();

    void on_actionAddPoint_triggered();

private:
    Ui::MainWindow *ui;

    std::vector<QPoint> points; // points waiting for operations.

    std::vector<QPointF> super;
    std::vector<QPointF> process;
    std::vector<QPointF> circle;
    std::vector<double> radis;
    int processIndex;
};

#endif // MAINWINDOW_H
