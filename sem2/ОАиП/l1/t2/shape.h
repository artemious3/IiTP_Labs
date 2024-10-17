#ifndef SHAPE_H
#define SHAPE_H

#include <QPainter>
#include <QVector2D>
#include <bitset>
#include <QObject>

namespace AP{

struct TransformData{
    qreal speedX = 0, speedY = 0;

    QPointF anchor;
    qreal speedAngle = 0;
    qreal speedScale = 0;

    int totalTime = 0;
    int currentTime = 0;
};

struct Transform{
    QPointF targetCenter;
    QPointF anchor;
    qreal targetAngle;
    qreal targetScale;
    int totalTime;
};

class Shape : public QObject
{
protected:
    Shape(QObject* parent);

    QPointF massCenter = {0,0};
    QPointF anchor = {0,0};
    QPointF coordinates = {0,0};

    qreal angle = 0;
    qreal scale = 1;

    qreal perimeter = -1, square = -1;

    std::bitset<3> isChanged {0b111};

    TransformData tData;
    Transform transform;
    bool isTransformActive = false;

    virtual void registerChange(unsigned long val);

public:
    Shape();
    static const unsigned char C = 0,
                                P = 1,
                                S = 2;

    //Nonconstant, because it changes internal perimeter, center and square values
    virtual qreal getSquare() = 0;
    virtual qreal getPerimeter() = 0;
    virtual QPointF getCenter() = 0;

    qreal getAngle() const;
    qreal getScale() const;

    virtual void move(qreal dx, qreal dy) = 0;
    virtual void rotate(qreal angle) = 0;
    virtual void sscale(qreal koef) = 0;

    virtual void draw(QPainter* painter) const = 0;

    void setTransformation(const Transform& data);
    virtual void setAnchor(const QPointF& point);
    void startTransform();
    void setFromShape(const AP::Shape* shape);
    QPointF getInternalCenter();
    void resetToCenter(const QSizeF& sz);

    void update(int dtime);

    Transform getTransform() const;
    const std::bitset<3>& getUpdateInfo();



    //virtual bool isInBounds(const QSizeF& sz);

signals:
    void transformFinished();
};

}
#endif // SHAPE_H
