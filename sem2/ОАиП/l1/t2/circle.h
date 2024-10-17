#ifndef CIRCLE_H
#define CIRCLE_H

#include <QObject>
#include "ellipse.h"

class Circle : public Ellipse
{
    Q_OBJECT
protected:
    explicit Circle(QObject *parent = nullptr);

public:
    Circle(qreal x, qreal y, qreal radius);

    virtual void move(qreal dx, qreal dy) override;
    virtual void rotate(qreal angle) override;

    virtual qreal getPerimeter() override;


};

#endif // CIRCLE_H
