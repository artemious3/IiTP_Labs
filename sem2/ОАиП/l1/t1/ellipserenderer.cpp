#include "ellipserenderer.h"
#include <QPainter>
#include <QBrush>

FaceRenderer::FaceRenderer(QWidget *parent)
    : QWidget{parent}, renderer(this)
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(animationLoop()));
    timer.start(update_time);
    elapsed_timer.start();
}

void FaceRenderer::addRenderFace(int parentWidth, int parentHeight){
    QWidget* widgetParernt = qobject_cast<QWidget*>(parent());
    new MyFace(this, parentWidth, parentHeight);
}

MyFace* FaceRenderer::getFace() const{
    return findChild<MyFace*>("MainFace");
}


void FaceRenderer::animationLoop(){
    int elapsed_time = elapsed_timer.elapsed();
    elapsed_timer.restart();

    MyFace* face = findChild<MyFace*>("MainFace");
    if(face){
        face->updateFace(elapsed_time);
    }

    this->update();
}

void FaceRenderer::paintEvent(QPaintEvent* e){
    Q_UNUSED(e);

    QPainter painter(this);
    painter.setRenderHint(QPainter::RenderHint::Antialiasing);

    painter.fillRect( QRect(0,0, width(), height()), QBrush(Qt::black) );
    MyFace* face = findChild<MyFace*>("MainFace");
    if(face){
        face->render(&painter);
    }
}

