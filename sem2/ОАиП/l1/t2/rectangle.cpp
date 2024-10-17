#include "rectangle.h"
#include <QWidget>

Rectangle::Rectangle(QObject* parent) : Quadrilateral(parent)
{}

Rectangle::Rectangle() : AP::Quadrilateral()
{}

void Rectangle::initializeBySides(qreal x, qreal y, qreal a, qreal b){
    qreal distA = a / 2;
    qreal distB = b / 2;

    anchor = {0,0};
    coordinates = {x,y};

    points << QPointF(-distA, -distB) <<
        QPointF(distA, -distB) <<
        QPointF(distA, distB) <<
        QPointF(-distA, distB);
}

Rectangle::Rectangle(qreal x, qreal y, qreal sA, qreal sB) : Quadrilateral()
{
    initializeBySides(x, y, sA, sB);
}


