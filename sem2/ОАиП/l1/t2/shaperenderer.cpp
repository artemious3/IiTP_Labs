#include "shaperenderer.h"
#include <QDebug>
//--MEMORY MANAGEMENT HERE--
ShapeRenderer::ShapeRenderer(QWidget *parent, AP::Shape* shape)
    : QWidget{parent}, renderShape(shape)
{
    connect(&renderTimer, SIGNAL(timeout()),
            this, SLOT(renderLoop()));
    if(shape){
        shape->setParent(this);
        renderTimer.start(1000/60);
    }
    deltaTimer.start();
    setMouseTracking(false);
    setEnabled(false);
}

void ShapeRenderer::resetShape(AP::Shape* newShape){
    renderTimer.stop();

    if(renderShape){
        newShape->setFromShape(renderShape);
        delete renderShape;
    }

    renderShape = newShape;
    renderShape->setParent(this);
    renderTimer.start(1000/60);
}


//OTHER FUNCTIONS

void ShapeRenderer::mouseMoveEvent(QMouseEvent* event){
    emit moveMouse(event->position());
}

void ShapeRenderer::updateShape(){
    int dtime = deltaTimer.elapsed();
    if(renderShape){
        renderShape->update(dtime);
    }
    deltaTimer.restart();
}


void ShapeRenderer::renderLoop(){
    updateShape();

    const std::bitset<3>& uInfo = renderShape->getUpdateInfo();
    if(uInfo.to_ulong() != 0){
        emit updatedShapeInfo();
    }

    repaint();
}

void ShapeRenderer::paintEvent(QPaintEvent* e){
    Q_UNUSED(e);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(QRect(0,0, width(), height()), Qt::black);
    painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
    painter.setPen(Qt::green);

    if(renderShape){
        renderShape->draw(&painter);
    }
}

void ShapeRenderer::startTransformation(const AP::Transform& td){
    renderShape->setTransformation(td);
    renderShape->startTransform();
}

AP::Shape* ShapeRenderer::getShape(){
    return renderShape;
}
