#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QObject>
#include "polygon.h"

class Triangle : public AP::Polygon
{
    Q_OBJECT
protected:
    explicit Triangle(QObject *parent = nullptr);

public:
    Triangle(qreal sideA, qreal sideB, qreal sideC);
};

#endif // TRIANGLE_H
