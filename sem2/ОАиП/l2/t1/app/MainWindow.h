//
// Created by artemious on 04.03.24.
//

#ifndef T1_MAINWINDOW_H
#define T1_MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include "Date.h"

namespace L2 {
    QT_BEGIN_NAMESPACE
    namespace Ui { class MainWindow; }
    QT_END_NAMESPACE

    class MainWindow : public QMainWindow {
    Q_OBJECT

    private:
        QFile* currentFile = nullptr;
        QTextStream* tstream = nullptr;

        QScopedPointer<Date> bday;

    public:
        explicit MainWindow(QWidget *parent = nullptr);

        ~MainWindow() override;

    private:
        Ui::MainWindow *ui;
        std::optional<L2::Date> constructDate(QString def = "DD.MM.YYYY");
        void setDateEditorActive(bool isActive);
        void cleanUpEditor();
    private slots:
        void addDate();
        void changeDate();
        void deleteDate();
        void openFile();
        void newFile();
        void getInfo();
        void inputBDay();

    };
} // L2

#endif //T1_MAINWINDOW_H
