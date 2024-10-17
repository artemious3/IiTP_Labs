#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "shape.h"


enum class ComboBoxShape{
    Square = 0,
    Rectangle,
    Rhombus,
    Hexagon,
    Star,
    WeirdStar,
    Circle,
    Triangle,
    Drawing
};


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    static const int animationTime = 1000;
    Ui::MainWindow *ui;
    AP::Shape* constructShape(bool isGiven = false, QPointF point = {0,0});
    void setDefaultWidgetValues();
    void setMaximumSpinboxValues();

public slots:
    void refreshShape();
    void mouseRefreshShape(const QPointF& pos);
    void startTransform();
    void refreshShapeInfo();
    void refreshTriangleBounds();
    void resetTransform();

};
#endif // MAINWINDOW_H
