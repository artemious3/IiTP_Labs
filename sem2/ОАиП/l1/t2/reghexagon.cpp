#include "reghexagon.h"
#include "mymath.h"
RegHexagon::RegHexagon(QObject *parent)
    : AP::Polygon{parent}
{}

RegHexagon::RegHexagon(qreal x, qreal y, qreal radius)
{
    anchor = {0,0};
    coordinates = {x,y};

    points.reserve(6);
    points.shrink_to_fit();

    qreal alpha = 2.0* M_PI / 6;
    QVector2D current(radius, 0);
    for(int i = 0; i < 6; ++i){
        points << current.toPointF();
        current = rotateVector2D(current, alpha);
    }
}
