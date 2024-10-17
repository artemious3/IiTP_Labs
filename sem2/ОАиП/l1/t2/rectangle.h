#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "quadrilateral.h"

class Rectangle : public AP::Quadrilateral
{
    Q_OBJECT
protected:
    Rectangle(QObject* parent);
    Rectangle();
    Rectangle(const QPolygonF& pol);

    void initializeBySides(qreal x, qreal y,qreal a, qreal b);
    qreal sideA, sideB;

public:
    Rectangle(qreal x, qreal y, qreal sA, qreal sB);

};

#endif // RECTANGLE_H
