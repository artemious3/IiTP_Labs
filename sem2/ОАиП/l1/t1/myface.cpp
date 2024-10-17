#include "myface.h"
#include <QTimer>

//_______!!!________________
MyFace::MyFace(QWidget *parent, int aparentWidth, int aparentHeight):
    MyEllipse(parent, aparentWidth*0.1, aparentHeight*0.1, aparentWidth*0.8, aparentHeight*0.8),
    parentWidth{aparentWidth},
    parentHeight{aparentHeight},
    eyeRect1(parentWidth*0.25, parentHeight * 0.3, 80, 80),
    eyeRect2(parentWidth*0.65, parentHeight * 0.3, 80, 80),
    pupilRect1(parentWidth*0.25 + 40, parentHeight * 0.3 + 25, 19, 19),
    pupilRect2(parentWidth*0.65 + 40, parentHeight * 0.3 + 25, 19, 19),
    lipRect1(parentWidth*0.5 - 50, parentHeight*0.72, 100, 30),
    lipRect2(parentWidth*0.5 - 50, parentHeight*0.74, 100, 30)
{
    this->setColor(Qt::yellow);
    setObjectName("MainFace");
    base = new MyEllipse(this, baseRect);
    base->setColor(Qt::yellow);

    eye1 = new MyEllipse(this, eyeRect1);
    eye1->setColor(Qt::white);
    eye2 = new MyEllipse(this, eyeRect2);
    eye2->setColor(Qt::white);

    pupil1 = new MyEllipse(this, pupilRect1);
    pupil1->setColor(Qt::black);
    pupil2 = new MyEllipse(this, pupilRect2);
    pupil2->setColor(Qt::black);

    lip1 = new MyEllipse(this, lipRect1);
    lip1->setColor(Qt::red);
    lip2 = new MyEllipse(this, lipRect2);
    lip2->setColor(Qt::red);
}


void MyFace::render(QPainter* painter){
    this->draw(painter);
    for(MyEllipse* el : findChildren<MyEllipse*>()){
        el->draw(painter);
    }

}

void MyFace::updateFace(int dtime){
    for(MyEllipse* el : findChildren<MyEllipse*>()){
        el->update(dtime);
    }
}

void MyFace::moveEyes(){
    if(areEyesMoving == true){
        return;
    }
    areEyesMoving = true;

    pupil1->startMoving(-20, 0, 250);
    pupil2->startMoving(-20, 0, 250);

    QTimer::singleShot(310, [=]{
        pupil1->startMoving(20, 0, 250);
        pupil2->startMoving(20, 0, 250);
    });

    QTimer::singleShot(580, [=]{
        areEyesMoving = false;
    });
}


void MyFace::openMouth(){
    if(isMouthMoving == true){
        return;
    }
    isMouthMoving = true;

    lip1->startMoving(0, -16, 250);
    lip2->startMoving(0, 16, 250);

    QTimer::singleShot(310, [=]{
        lip1->startMoving(0, 16, 250);
        lip2->startMoving(0, -16, 250);
    });

    QTimer::singleShot(580, [=]{
        isMouthMoving = false;
    });

}
