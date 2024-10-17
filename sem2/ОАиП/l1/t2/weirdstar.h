#ifndef WEIRDSTAR_H
#define WEIRDSTAR_H

#include <QObject>
#include "polygon.h"

class WeirdStar : public AP::Polygon
{
    Q_OBJECT
protected:
    explicit WeirdStar(QObject *parent = nullptr);

public:
    WeirdStar(qreal x, qreal y, int N, qreal length);

};

#endif // WEIRDSTAR_H
