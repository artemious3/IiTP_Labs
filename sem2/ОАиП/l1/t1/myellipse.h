#ifndef MYELLIPSE_H
#define MYELLIPSE_H

#include <QObject>
#include <QRect>
#include <QColor>
#include <QWidget>

class MyEllipse : public QWidget
{
    Q_OBJECT

private:
    bool isMoving = false;
    float speedX = 0.f;
    float speedY = 0.f;
    int totalAnimationTime = 0;
    int currentAnimationTime = 0;

    const int updateTime = 1000 / 60;

    QRectF rect = {0.0f ,0.0f , 1.0f , 1.0};
    QColor color = Qt::white;


public:

    MyEllipse(QWidget* parent = nullptr);
    MyEllipse(QWidget* parent, const QRectF& rect);
    MyEllipse(QWidget* parent, float x, float y, float width, float height);

    void setColor(QColor color);
    QColor getColor() const;
    bool getIsMoving() const;

    void startMoving(float offset_x, float offset_y, int time);
    void update(int dTime);
    void draw(QPainter* painter);
};

#endif // MYELLIPSE_H
