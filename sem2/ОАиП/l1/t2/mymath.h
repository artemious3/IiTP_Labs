#ifndef MYMATH_H
#define MYMATH_H

#include <QVector2D>
#include <QPointF>
#include <QtMath>


QVector2D rotateVector2D(const QVector2D& vect, qreal angle);
qreal dist(const QPointF* a, const QPointF* b);


#endif // MYMATH_H
