#ifndef STAR_H
#define STAR_H

#include <QObject>
#include "polygon.h"

class Star : public AP::Polygon
{
    Q_OBJECT
protected:
    explicit Star(QObject *parent = nullptr);

public:
    Star(qreal x, qreal y, int N, qreal length, qreal ratio);


};

#endif // STAR_H
