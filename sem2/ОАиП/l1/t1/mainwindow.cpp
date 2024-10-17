#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QRect renderGeometry = ui->renderArea->geometry();
    ui->renderArea->addRenderFace(renderGeometry.width(), renderGeometry.height());

    setFixedHeight(geometry().height());
    setFixedWidth(geometry().width());

    connect(ui->moveEyes, SIGNAL(pressed()), ui->renderArea->getFace(), SLOT(moveEyes()));
    connect(ui->openMouth, SIGNAL(pressed()), ui->renderArea->getFace(), SLOT(openMouth()));
    connect(ui->infoButton, &QPushButton::pressed, this, [=](){
        QMessageBox::information(this, tr("Info"), tr("Made by Артем Подгайский, group 353501"));
    });
}



MainWindow::~MainWindow()
{
    delete ui;
}
