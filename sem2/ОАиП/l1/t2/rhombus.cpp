#include "rhombus.h"
#include "mymath.h"

Rhombus::Rhombus(QObject *parent)
    : AP::Polygon{parent}
{}


Rhombus::Rhombus(qreal x, qreal y, qreal side, qreal angle) : AP::Polygon() {
    qreal width = side * qCos(angle/2);
    qreal height = side * qSin(angle/2);

    this->anchor = {0,0};
    this->coordinates = {x,y};

    points << QPointF(width, 0)
           << QPointF(0, height)
           << QPointF(-width, 0)
           << QPointF(0, -height);

}
