#include "mymath.h"

QVector2D rotateVector2D(const QVector2D& vect, qreal angle){
    return QVector2D( qCos(angle)*vect.x() - qSin(angle)*vect.y(),
                     qSin(angle) * vect.x() + qCos(angle)*vect.y());
}

qreal dist(const QPointF* a, const QPointF* b){
    QPointF diff = *b - *a;
    return QVector2D(diff).length();
}
