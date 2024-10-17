#include "ellipse.h"
#include <QtMath>

Ellipse::Ellipse(QObject *parent)
    : AP::Shape{parent}
{}

Ellipse::Ellipse(qreal x, qreal y, qreal aaxisX, qreal aaxisY)
    : axisX(aaxisX), axisY(aaxisY)
{
    anchor = {0,0};
    coordinates = {x,y};
    massCenter = {0,0};
}

qreal Ellipse::getSquare()
{
    if(isChanged[S]){
        square = M_PI * axisY * axisX;
        isChanged[S] = false;
    }

    return square;
}

void Ellipse::sscale(qreal koef){
    axisX *= koef;
    axisY *= koef;
    scale *= koef;
}

qreal Ellipse::getPerimeter()
{
    if(isChanged[P]){

        qreal p1 = M_PI * qSqrt(2 * (axisX*axisX + axisY*axisY));
        qreal p2 = M_PI * (1.5*(axisX + axisY) - sqrt(axisX*axisY));
        perimeter = 0.5*(p1 + p2);
        isChanged[P] = false;
    }

    return perimeter;
}

QPointF Ellipse::getCenter()
{
    return coordinates + massCenter;
}

void Ellipse::draw(QPainter* painter) const{
    painter->drawEllipse(coordinates + massCenter, axisX, axisY);
}






