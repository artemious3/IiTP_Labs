#ifndef REGHEXAGON_H
#define REGHEXAGON_H

#include <QObject>
#include "polygon.h"

class RegHexagon : public AP::Polygon
{
protected:
    explicit RegHexagon(QObject *parent = nullptr);

public:
    RegHexagon(qreal x, qreal y, qreal radius);
};

#endif // REGHEXAGON_H
