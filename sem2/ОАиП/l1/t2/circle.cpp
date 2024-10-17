#include "circle.h"
#include <QVector2D>
#include <QtMath>
#include "mymath.h"

Circle::Circle(QObject *parent)
    : Ellipse{parent}
{}


Circle::Circle(qreal x, qreal y, qreal radius) : Ellipse(x,y,radius,radius)
{}

void Circle::move(qreal dx, qreal dy){
    coordinates += QPointF(dx, dy);
}

void Circle::rotate(qreal angle){
    QVector2D vect (massCenter - anchor);
    massCenter = anchor + rotateVector2D(vect, angle).toPointF();
    this->angle += angle;
}

qreal Circle::getPerimeter(){
    if (isChanged[P]){
        perimeter = 2*M_PI*axisX;
    }
    return perimeter;
}



