#ifndef SQUARE_H
#define SQUARE_H

#include <QObject>
#include "rectangle.h"

class Square : public Rectangle
{
    Q_OBJECT
protected:
    explicit Square(QObject *parent = nullptr);

public:
    Square(qreal x, qreal y, qreal side);
};

#endif // SQUARE_H
