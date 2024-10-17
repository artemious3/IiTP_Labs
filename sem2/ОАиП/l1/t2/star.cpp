#include "star.h"
#include <QVector2D>
#include <QtMath>
#include "mymath.h"


Star::Star(QObject *parent)
    : AP::Polygon{parent}
{}

Star::Star(qreal x, qreal y, int N, qreal length, qreal ratio) : AP::Polygon()
{
    anchor = {0,0};
    coordinates = {x,y};
    points.reserve(2*N);
    points.shrink_to_fit();

    qreal alpha = M_PI / N; // (2*Pi / N) / 2;
    QVector2D current(length, 0);
    for(int i = 0; i < N; ++i){
        points << current.toPointF();
        current = rotateVector2D(current, alpha);
        points << (current/ratio).toPointF();
        current = rotateVector2D(current, alpha);
    }
}
