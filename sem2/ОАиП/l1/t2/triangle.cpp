#include "triangle.h"
#include "mymath.h"

Triangle::Triangle(QObject *parent)
    : AP::Polygon{parent}
{}


Triangle::Triangle(qreal a, qreal b, qreal c){
    qreal c_x = (c*c + a*a - b*b)/(2*a);
    qreal c_y = -qSqrt(c*c - c_x*c_x);

    points << QPointF(c_y, c_x)
           << QPointF(0,0)
           << QPointF(0,a);

    points.translate( -1 * std::accumulate(points.begin(), points.end(), QPointF(0,0)) / 3.0 );
}
