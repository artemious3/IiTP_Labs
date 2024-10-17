#ifndef ELLIPSERENDERER_H
#define ELLIPSERENDERER_H

#include <QWidget>
#include <QTimer>
#include <QList>
#include <QPainter>
#include <QSharedPointer>
#include <QElapsedTimer>
#include <QChildEvent>
#include "myface.h"


class FaceRenderer : public QWidget
{
    Q_OBJECT

public:
    explicit FaceRenderer(QWidget *parent = nullptr);
    void addRenderFace(int parentWidth, int parentHeight);
    MyFace* getFace() const;

private:
    void paintEvent(QPaintEvent*);


public slots:
    void animationLoop();


public:
    static const int fps = 60;
    static constexpr int update_time = 1000 / fps;

private:
    QPainter renderer;
    QTimer timer;
    QElapsedTimer elapsed_timer;
};

#endif // ELLIPSERENDERER_H
