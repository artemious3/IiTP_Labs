#ifndef MYFACE_H
#define MYFACE_H

#include <QObject>
#include <QPainter>
#include "myellipse.h"

class MyFace : public MyEllipse
{
    Q_OBJECT
private:
    const int parentWidth, parentHeight;
    QRectF baseRect;
    QRectF eyeRect1, eyeRect2;
    QRectF pupilRect1, pupilRect2;
    QRectF lipRect1, lipRect2;

    bool areEyesMoving = false, isMouthMoving = false;

public:
    explicit MyFace(QWidget *parent = nullptr, int aparentWidth = 100, int aparentHeight = 100);
    MyEllipse *base;
    MyEllipse *eye1, *eye2;
    MyEllipse *pupil1, *pupil2;
    MyEllipse *lip1, *lip2;

public:
    void render(QPainter* painter);
    void updateFace(int dTime);

public slots:
    void moveEyes();
    void openMouth();
};

#endif // MYFACE_H
