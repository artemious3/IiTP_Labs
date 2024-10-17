#ifndef POLYGON_H
#define POLYGON_H

#include <QObject>
#include <QVector>
#include <QPoint>
#include "shape.h"

namespace AP{

class Polygon : public AP::Shape
{
protected:
    QPolygonF points;
    Polygon(QObject* parent);
    Polygon();

public:
    Polygon(const QPolygonF& points);
    virtual void draw(QPainter* painter) const override;

protected:
    virtual qreal getSquare() override;
    virtual qreal getPerimeter() override;
    virtual QPointF getCenter() override;

    virtual void move(qreal dx, qreal dy) override;
    virtual void rotate( qreal angle) override;
    virtual void sscale(qreal koef) override;


};

}



#endif // POLYGON_H
