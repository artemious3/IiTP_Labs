#include "shape.h"
#include <QWidget>
#include <QDebug>
#include "mymath.h"
#include <QVector2D>

AP::Shape::Shape() : QObject{nullptr}
{}

AP::Shape::Shape(QObject* parent) : QObject{parent}
{}



void AP::Shape::registerChange(unsigned long val){
    isChanged = std::bitset<3>(val);
}

void AP::Shape::setTransformation(const AP::Transform& data){
    AP::TransformData td;
    setAnchor(data.anchor);

    td.totalTime = data.totalTime;
    td.speedX = (data.targetCenter.x() - coordinates.x()) / data.totalTime;
    td.speedY = (data.targetCenter.y() - coordinates.y() ) / data.totalTime;
    td.speedAngle = ( data.targetAngle - this->angle ) / data.totalTime;
    td.speedScale = ( data.targetScale - this->scale ) / data.totalTime;
    td.currentTime = 0;

    tData = td;
    transform = data;
}

void AP::Shape::startTransform(){
    isTransformActive = true;
}


void AP::Shape::update(int dtime){
    if(!isTransformActive){
        return;
    }

    if(tData.currentTime >= tData.totalTime){
        qDebug() << "target angle" << 180 / M_PI * transform.targetAngle;
        qDebug() << "current angle" << 180.0 / M_PI * angle;
        qDebug() << "difference" << 180.0 / M_PI * (transform.targetAngle - angle);

        move(transform.targetCenter.x() - coordinates.x(), transform.targetCenter.y() - coordinates.y());
        rotate(transform.targetAngle - angle);
        qDebug() << 180.0 / M_PI * angle;
        isTransformActive = false;
        //emit transformFinished();
        return;
    }

    //qDebug() << dtime << " " << tData.currentTime;
    unsigned long changes = 0b000;
    if(tData.speedX != 0 || tData.speedY != 0){
        this->move(tData.speedX * dtime, tData.speedY * dtime);
        changes |= 0b001;
    }

    if(tData.speedAngle != 0){
        this->rotate(tData.speedAngle * dtime);
        changes |= 0b001;
    }

    if(tData.speedScale != 0){
        qreal dscale = (scale + tData.speedScale * dtime)/scale;
        qDebug() << dscale;
        this->sscale(dscale);
        changes |= 0b111;
    }

    registerChange(changes);
    tData.currentTime += dtime;
}

qreal AP::Shape::getAngle() const{
    return angle;
}

qreal AP::Shape::getScale() const{
    return scale;
}

void AP::Shape::setAnchor(const QPointF& point){
    if(anchor != point){
        qDebug() << "set anchor occured";
        qreal tempAngle = angle;
        rotate(-tempAngle);
        anchor = point;
        rotate(tempAngle);
    }
}

void AP::Shape::setFromShape(const AP::Shape* shape){
    QPointF diff = shape->coordinates - coordinates;
    setAnchor(shape->anchor);
    move(diff.x(), diff.y());
    rotate(shape->angle - angle);
    sscale(shape->scale/scale);
    registerChange(0b111);
}

AP::Transform AP::Shape::getTransform() const{
    Transform t;
    t.anchor = anchor;
    t.targetCenter = coordinates;
    t.targetAngle = angle;
    t.targetScale = scale;
    t.totalTime = 0;
    return t;
}

QPointF AP::Shape::getInternalCenter(){
    return getCenter() - coordinates;
}

void AP::Shape::resetToCenter(const QSizeF& sz){
    isTransformActive = false;
    tData = TransformData{};

    setAnchor({0,0});
    registerChange(0b001);
    QPointF diff = QPointF(sz.width()/2, sz.height()/2) - coordinates;
    move(diff.x(), diff.y());
    rotate(-angle);
    sscale(1.0/scale);
    registerChange(0b111);
}

const std::bitset<3>&  AP::Shape::getUpdateInfo(){
    return isChanged;
}

