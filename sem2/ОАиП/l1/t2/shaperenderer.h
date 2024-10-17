#ifndef SHAPERENDERER_H
#define SHAPERENDERER_H

#include <QWidget>
#include <QPointer>
#include <QTimer>
#include <QElapsedTimer>
#include <QMouseEvent>
#include "shape.h"

class ShapeRenderer : public QWidget
{
    Q_OBJECT
private:
    AP::Shape* renderShape;
    QTimer renderTimer;
    QElapsedTimer deltaTimer;
    bool wasPressed = false;

    void updateShape();

public:
    ShapeRenderer(QWidget *parent = nullptr, AP::Shape* shape = nullptr);
    void paintEvent(QPaintEvent* e) override;
    void resetShape(AP::Shape* shape = nullptr);
    void startTransformation(const AP::Transform& td);
    AP::Shape * getShape();
    void mouseMoveEvent(QMouseEvent* event) override;



public slots:
    void renderLoop();

signals:
    void updatedShapeInfo();
    void moveMouse(const QPointF& pos);
};

#endif // SHAPERENDERER_H
