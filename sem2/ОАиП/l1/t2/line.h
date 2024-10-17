#ifndef LINE_H
#define LINE_H

#include <QObject>
#include "polygon.h"
#include <QPointF>
#include <QVector>

class Line : public AP::Polygon
{
    Q_OBJECT
protected:
    qreal getSquare() override;
    //qreal getPerimeter() override;

    void draw(QPainter* painter) const override;

public:
    explicit Line(QObject *parent = nullptr);
    Line(qreal x, qreal y);
    void addPoint(const QPointF& pnt);
};

#endif // LINE_H
