#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSpinBox>
#include <QMessageBox>
#include <QtMath>
#include <QDebug>


#include "rectangle.h"
#include "reghexagon.h"
#include "weirdstar.h"
#include "star.h"
#include "circle.h"
#include "rhombus.h"
#include "triangle.h"
#include "line.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedHeight(geometry().height());
    setFixedWidth(geometry().width());

    setMaximumSpinboxValues();
    setDefaultWidgetValues();

    foreach(QSpinBox* sbox, findChildren<QSpinBox*>(QRegularExpression("Value$"))){
        connect(sbox, SIGNAL(valueChanged(int)),
                this, SLOT(refreshShape()));
    }
    foreach(QDoubleSpinBox* sbox, findChildren<QDoubleSpinBox*>(QRegularExpression("Value$"))){
        connect(sbox, SIGNAL(valueChanged(qreal)),
                this, SLOT(refreshShape()));
    }


    foreach(QSpinBox* sbox, findChildren<QSpinBox*>(QRegularExpression("Transform$"))){
        connect(sbox, SIGNAL(valueChanged(int)),
                this, SLOT(startTransform()));
    }
    foreach(QDoubleSpinBox* sbox, findChildren<QDoubleSpinBox*>(QRegularExpression("Transform$"))){
        connect(sbox, SIGNAL(valueChanged(qreal)),
                this, SLOT(startTransform()));
    }

    foreach(QSpinBox* sbox, findChildren<QSpinBox*>(QRegularExpression("^triangle"))){
        connect(sbox, SIGNAL(valueChanged(int)),
                this, SLOT(refreshTriangleBounds()));
    }

    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(refreshShape()));

    connect(ui->shapeRenderer, SIGNAL(updatedShapeInfo()),
            this, SLOT(refreshShapeInfo()));

    connect(ui->resetButton, SIGNAL(clicked()),
            this, SLOT(resetTransform()));

    connect(ui->shapeRenderer, &ShapeRenderer::moveMouse,
            this, &MainWindow::mouseRefreshShape);


    refreshShape();
}

MainWindow::~MainWindow()
{
    delete ui;
}




AP::Shape *MainWindow::constructShape(bool isGiven, QPointF point)
{
    if(!isGiven){
        point = {(qreal)ui->shapeRenderer->width()/2,
                 (qreal)ui->shapeRenderer->height()/2};
    }
    AP::Shape* constructed = nullptr;

    if(ui->comboBox->currentIndex() != (int)ComboBoxShape::Drawing){
        ui->shapeRenderer->setEnabled(false);
    }

    switch(ui->comboBox->currentIndex()){
    case (int)ComboBoxShape::Square:
        constructed = new Rectangle(point.x(),
                                 point.y(),
                                 ui->sideValue->value(),
                                 ui->sideValue->value());
        break;

    case (int)ComboBoxShape::Rectangle:
        constructed = new Rectangle(point.x(),
                                    point.y(),
                                    ui->sideAValue->value(),
                                    ui->sideBValue->value());
        break;

    case (int)ComboBoxShape::Star:
        constructed = new Star(point.x(),
                               point.y(),
                               ui->NValue->value(),
                               0.3*ui->shapeRenderer->width(),
                               ui->ratioValue->value());
        break;

    case (int)ComboBoxShape::Rhombus:
        constructed = new Rhombus(point.x(),
                                  point.y(),
                                  ui->rhombusSideValue->value(),
                                  M_PI / 180.0 * ui->rhombusAngleValue->value());
        break;
    case (int)ComboBoxShape::WeirdStar:
    constructed  = new WeirdStar(point.x(),
                                point.y(),
                                ui->weirdNValue->value(),
                                   0.2 * ui->shapeRenderer->width());
        break;

    case (int)ComboBoxShape::Hexagon:
        constructed = new RegHexagon(point.x(),
                                     point.y(),
                                     ui->hexagonRadiusValue->value());
        break;

    case (int)ComboBoxShape::Circle:
        constructed = new Circle(point.x(),
                                   point.y(),
                                 ui->circleRadiusValue->value());
        break;
    case (int)ComboBoxShape::Triangle:
        constructed = new Triangle(ui->triangleAValue->value(),
                                   ui->triangleBValue->value(),
                                   ui->triangleCValue->value());
        break;
    case (int)ComboBoxShape::Drawing:
        resetTransform();
        ui->shapeRenderer->setEnabled(true);
        constructed = new Line(point.x(),
                               point.y());
        break;
    }

    return constructed;
}


void MainWindow::setMaximumSpinboxValues(){

    QRect rgeometry = ui->shapeRenderer->geometry();

    //--SQUARE & RECTANGLE--
    ui->sideValue->setMaximum( std::min(rgeometry.width(), rgeometry.height()) );
    ui->sideAValue->setMaximum(rgeometry.width());
    ui->sideBValue->setMaximum(rgeometry.height());

    //-- STAR & Weird STAR --
    ui->NValue->setMaximum(64);
    ui->ratioValue->setMaximum(5.0);
    ui->ratioValue->setMinimum(1.3);
    ui->weirdNValue->setMaximum(32);

    //-- CIRCLE & HEXAGON --
    ui->circleRadiusValue->setMaximum( std::min(rgeometry.width(), rgeometry.height()) / 2);
    ui->hexagonRadiusValue->setMaximum( std::min(rgeometry.width(), rgeometry.height()) / 2 );

    // -- RHOMBUS --
    ui->rhombusAngleValue->setMaximum(90);
    ui->rhombusSideValue->setMaximum(std::min(rgeometry.width(), rgeometry.height()));


    // -- TRANSFORM --
    ui->anchorXTransform->setMaximum(rgeometry.width()/4);
    ui->anchorYTransform->setMaximum(rgeometry.height()/4);
    ui->anchorXTransform->setMinimum(-rgeometry.width()/4);
    ui->anchorYTransform->setMinimum(-rgeometry.height()/4);
    ui->moveToXTransform->setMaximum(rgeometry.width());
    ui->moveToYTransform->setMaximum(rgeometry.height());
    ui->angleTransform->setMaximum( 360 * 5 );
    ui->angleTransform->setMinimum( - 360 * 5);
    ui->scaleTransform->setMaximum( 5.0 );
    ui->scaleTransform->setMinimum( 0.3 );


}
void MainWindow::setDefaultWidgetValues()
{
    QRect rgeometry = ui->shapeRenderer->geometry();

    ui->sideValue->setValue(0.15* rgeometry.width());
    ui->sideAValue->setValue(0.15*rgeometry.width());
    ui->sideBValue->setValue(0.07*rgeometry.height());

    //-- STAR & Weird STAR --
    ui->NValue->setValue(5);
    ui->ratioValue->setValue(3.0);
    ui->weirdNValue->setValue(6);

    //-- CIRCLE & HEXAGON --
    ui->circleRadiusValue->setValue( std::min(rgeometry.width(), rgeometry.height()) * 0.12 );
    ui->hexagonRadiusValue->setValue( std::min(rgeometry.width(), rgeometry.height()) * 0.12 );

    // -- RHOMBUS --
    ui->rhombusAngleValue->setValue(30);
    ui->rhombusSideValue->setValue(std::min(rgeometry.width(), rgeometry.height()) * 0.3);

    // -- TRIANGLE --
    ui->triangleAValue->setValue(100);
    ui->triangleBValue->setValue(100);
    ui->triangleCValue->setValue(100);
    ui->triangleAValue->setMaximum(400);
    ui->triangleBValue->setMaximum(400);
    ui->triangleCValue->setMaximum(400);


    // -- TRANSFORM --
    ui->anchorXTransform->setValue(0);
    ui->anchorYTransform->setValue(0);
    ui->moveToXTransform->setValue(rgeometry.width()/2);
    ui->moveToYTransform->setValue(rgeometry.height()/2);
    ui->angleTransform->setValue(0);
    ui->scaleTransform->setValue(1);
}

void MainWindow::refreshShape(){
    AP::Shape* constructedShape = constructShape();

    if(!constructedShape){
        qFatal("nullptr shape");
    }

    ui->shapeRenderer->resetShape(constructedShape);
    refreshShapeInfo();
}

void MainWindow::mouseRefreshShape(const QPointF& point){
    Line* line = qobject_cast<Line*>( ui->shapeRenderer->getShape() );
    qDebug() << "handle mouse";
    qDebug() << (line == nullptr);

    if(line){
        line->addPoint(point);
    }

    refreshShapeInfo();
}

void MainWindow::startTransform(){

    AP::Transform t;
    t.targetAngle = M_PI / 180 * ui->angleTransform->value();
    t.targetCenter = {(qreal)ui->moveToXTransform->value(), (qreal)ui->moveToYTransform->value()};
    t.targetScale = ui->scaleTransform->value();
    t.totalTime = animationTime;
    t.anchor = {(qreal)ui->anchorXTransform->value(), (qreal)ui->anchorYTransform->value()};

    ui->shapeRenderer->startTransformation(t);
}


void MainWindow::refreshShapeInfo(){

    AP::Shape* shape = ui->shapeRenderer->getShape();

    ui->squareValue->setText(QString::number(shape->getSquare()));
    ui->perimeterValue->setText(QString::number(shape->getPerimeter()));
    QPointF shapeCenter = shape->getCenter();
    ui->coordX->setText(QString::number(shapeCenter.x()));
    ui->coordY->setText(QString::number(shapeCenter.y()));
    ui->angleText->setText(QString::number(180 / M_PI * shape->getAngle()));
    ui->scaleText->setText(QString::number(shape->getScale()));

}

void MainWindow::refreshTriangleBounds(){

    ui->triangleAValue->setMaximum(  ui->triangleBValue->value() + ui->triangleCValue->value() );
    ui->triangleBValue->setMaximum(  ui->triangleCValue->value() + ui->triangleAValue->value() );
    ui->triangleCValue->setMaximum(  ui->triangleAValue->value() + ui->triangleBValue->value() );

}

void MainWindow::resetTransform(){
    QSizeF rsize = ui->shapeRenderer->size();

    ui->shapeRenderer->getShape()->resetToCenter(rsize);

    foreach(QSpinBox* sbox, findChildren<QSpinBox*>(QRegularExpression("Transform$"))){
        sbox->blockSignals(true);
    }
    foreach(QDoubleSpinBox* sbox, findChildren<QDoubleSpinBox*>(QRegularExpression("Transform$"))){
        sbox->blockSignals(true);
    }

    ui->anchorXTransform->setValue(0);
    ui->anchorYTransform->setValue(0);
    ui->moveToXTransform->setValue(rsize.width()/2);
    ui->moveToYTransform->setValue(rsize.height()/2);
    ui->angleTransform->setValue(0);
    ui->scaleTransform->setValue(1);

    foreach(QSpinBox* sbox, findChildren<QSpinBox*>(QRegularExpression("Transform$"))){
        sbox->blockSignals(false);
    }
    foreach(QDoubleSpinBox* sbox, findChildren<QDoubleSpinBox*>(QRegularExpression("Transform$"))){
        sbox->blockSignals(false);
    }

    refreshShapeInfo();
}
