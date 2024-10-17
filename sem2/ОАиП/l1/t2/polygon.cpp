#include "polygon.h"
#include <QtMath>
#include <QDebug>
#include <QVector2D>
#include "mymath.h"

using AP::Polygon;


//---Constructors---

AP::Polygon::Polygon(QObject* parent) : AP::Shape{parent}
{}

AP::Polygon::Polygon() : AP::Shape()
{}

AP::Polygon::Polygon(const QPolygonF& apoints) : AP::Shape()
{
    if(apoints.size() < 3){
        qFatal("Wrong initialization of polygon with less than 3 points");
    }

    points = apoints;
    massCenter = std::accumulate(points.begin(), points.end(), QPointF(0,0)) / points.size();
}

//---Getters---

QPointF AP::Polygon::getCenter() {
    if(points.size() == 0){
        return {0,0};
    }

    if(isChanged[C]){
        massCenter = std::accumulate(points.begin(), points.end(), QPointF(0,0)) / points.size();
        isChanged[C] = false;
    }

    return coordinates + massCenter;
}




qreal AP::Polygon::getPerimeter() {
    if(points.empty()){
        return 0;
    }
    if(isChanged[P]){

        QPolygonF::const_iterator p1= points.cbegin();
        QPolygonF::const_iterator p2 = std::next(p1);
        qreal perimeter = 0;

        for(; p2 != points.cend(); ++p1, ++p2){
            QPointF diffVector = *p2 - *p1;
            perimeter += QVector2D(diffVector).length();
        }

        QPointF lastDiffVector = *p1 - *points.begin();
        perimeter += QVector2D(lastDiffVector).length();

        this->perimeter = perimeter;
        isChanged[P] = false;
    }

    return this->perimeter;
}



qreal AP::Polygon::getSquare() {

    if(isChanged[S]){
        QPolygonF::const_iterator p[2] = {points.cbegin(),
                               std::next(points.cbegin())};

        qreal resSquare = 0;

        for(; p[1] != points.cend(); p[0] = std::next(p[0]),
                                      p[1] = std::next(p[1])){

            resSquare += p[0]->x()*p[1]->y() -
                         p[1]->x()*p[0]->y();
        }

        p[0] = std::prev(points.cend());
        p[1] = points.cbegin();
        resSquare += p[0]->x()*p[1]->y() -
                     p[1]->x()*p[0]->y();



        resSquare /= 2;

        square = qAbs(resSquare);
        isChanged[S] = false;
    }

    return this->square;

}


void AP::Polygon::move(qreal dx, qreal dy)
{
    QPointF dPoint = QPointF(dx,dy);
    coordinates += dPoint;
}

void AP::Polygon::sscale(qreal koef){

    for(auto& point : points){
        QVector2D scaleVect(point - getInternalCenter());
        scaleVect *= koef;
        point = getInternalCenter() + scaleVect.toPointF();
    }
    scale *= koef;
}



void AP::Polygon::rotate(qreal angle){
    for(auto& point : points)
    {
        QVector2D vect (point - anchor);
        vect = rotateVector2D(vect, angle);
        point = anchor + vect.toPointF();
    }
    this->angle += angle;
}

//---Main methods---

void AP::Polygon::draw(QPainter* painter) const{
    painter->drawPolygon(points.translated(coordinates));
}

