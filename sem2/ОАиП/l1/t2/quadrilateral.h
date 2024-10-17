#ifndef QUADRILATERAL_H
#define QUADRILATERAL_H

#include <QObject>
#include "polygon.h"

namespace AP{

class Quadrilateral : public AP::Polygon
{
protected:
    Quadrilateral();
    Quadrilateral(QObject* parent);

public:
    Quadrilateral(const QPolygonF& points);
};

}

#endif // QUADRILATERAL_H
