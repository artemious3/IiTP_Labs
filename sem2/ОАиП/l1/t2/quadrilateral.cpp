#include "quadrilateral.h"

AP::Quadrilateral::Quadrilateral(QObject* parent) : AP::Polygon{parent}
{}

AP::Quadrilateral::Quadrilateral() : AP::Polygon()
{}

AP::Quadrilateral::Quadrilateral(const QPolygonF& apoints) : AP::Polygon()
{
    if(apoints.size() != 4){
        qFatal("Wrong initialization of a quadrilateral with polygon of not 4 points");
    }

    points = apoints;
    points.shrink_to_fit();
}
