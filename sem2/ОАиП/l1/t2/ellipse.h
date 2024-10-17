#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <QObject>
#include "shape.h"

class Ellipse : public AP::Shape
{
    Q_OBJECT

protected:
    qreal axisX, axisY;
    explicit Ellipse(QObject *parent = nullptr);

public:
    Ellipse(qreal x, qreal y, qreal aaxisX, qreal aaxisY);

    virtual qreal getSquare() override;
    virtual qreal getPerimeter() override;
    virtual QPointF getCenter() override;
    virtual void draw(QPainter* painter) const override;
    virtual void sscale(qreal koef) override;

};

#endif // ELLIPSE_H
