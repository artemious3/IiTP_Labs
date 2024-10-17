#ifndef RHOMBUS_H
#define RHOMBUS_H

#include <QObject>
#include "polygon.h"

class Rhombus : public AP::Polygon
{
    Q_OBJECT
protected:
    explicit Rhombus(QObject *parent = nullptr);

public:
    Rhombus(qreal x, qreal y, qreal side, qreal angle);
};

#endif // RHOMBUS_H
