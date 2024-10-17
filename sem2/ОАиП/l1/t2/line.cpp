#include "line.h"

Line::Line(QObject *parent)
    : AP::Polygon{parent}
{}

Line::Line(qreal x, qreal y){
    coordinates = {x,y};
}

qreal Line::getSquare(){
    return 0;
}

/*qreal Line::getPerimeter(){
    return 0;
}*/


void Line::draw(QPainter* painter) const{
    painter->drawPolyline(points.translated(coordinates));
}

void Line::addPoint(const QPointF& point){
    points.push_back(point - coordinates);
    registerChange(0b011);
}

