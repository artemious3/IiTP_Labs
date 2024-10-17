#include "myellipse.h"
#include <QPainter>
#include <QBrush>
#include <QTimer>

MyEllipse::MyEllipse(QWidget* parent) : QWidget(parent)
{}

MyEllipse::MyEllipse(QWidget* parent, const QRectF& arect) : QWidget(parent), rect(arect)
{}

MyEllipse::MyEllipse(QWidget* parent, float x, float y, float width, float height) : QWidget(parent){
    QRect temp_rect(x, y, width, height);
    this->rect = temp_rect;
}

void MyEllipse::setColor(QColor acolor){
    this->color = acolor;
}

QColor MyEllipse::getColor() const{
    return this->color;
}

bool MyEllipse::getIsMoving() const{
    return isMoving;
}

void MyEllipse::startMoving(float offset_x, float offset_y, int time){
    if(isMoving)
        return;
    totalAnimationTime = time;
    speedX = offset_x / time;
    speedY = offset_y / time;
    isMoving = true;
}

//btw, offset can be computed here - on each update call
void MyEllipse::update(int dTime){
    currentAnimationTime += dTime;

    if(currentAnimationTime >= totalAnimationTime){
        isMoving = false;
        speedX = 0.f;
        speedY = 0.f;
        totalAnimationTime = 0;
        currentAnimationTime = 0;
    }

    if(isMoving){
        rect.translate(dTime * speedX, dTime * speedY);
    }
}

void MyEllipse::draw(QPainter* painter){
    painter->setPen(color);
    painter->setBrush(QBrush(color));
    painter->drawEllipse(rect);
}
