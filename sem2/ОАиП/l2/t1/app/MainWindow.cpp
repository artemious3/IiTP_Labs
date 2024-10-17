//
// Created by artemious on 04.03.24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>
#include "MainWindow.h"
#include "ui_MainWindow.h"


namespace L2 {
    MainWindow::MainWindow(QWidget *parent) :
            QMainWindow(parent), ui(new Ui::MainWindow) {
        ui->setupUi(this);
        setFixedSize(this->geometry().size());

        setDateEditorActive(false);

        // -- MODIFY -
        connect(ui->deleteButton, &QPushButton::pressed,
                this, &MainWindow::deleteDate);
        connect(ui->editButton, &QPushButton::pressed,
                this, &MainWindow::changeDate);
        connect(ui->addButton, &QPushButton::pressed,
                this, &MainWindow::addDate);

        //-- FILE --
        connect(ui->openButton, &QPushButton::pressed,
                this, &MainWindow::openFile);
        connect(ui->newButton, &QPushButton::pressed,
                this, &MainWindow::newFile);

        // -- INFO --
        connect(ui->getInfo, &QPushButton::pressed,
                this, &MainWindow::getInfo);
        connect(ui->bdayButton, &QPushButton::pressed,
                this, &MainWindow::inputBDay);


    }

    MainWindow::~MainWindow() {
        cleanUpEditor();
        delete ui;
    }

    void MainWindow::addDate() {
        auto newDate = constructDate();
        if (newDate) {
            ui->dateTable->pushDate(newDate.value());
            if (currentFile) {
                tstream->seek((ui->dateTable->rowCount() - 1) * DateInfo::cDateLength);
                QString newDateStr = QString::fromStdString(newDate.value().ToString()) + '\n';
                *tstream << newDateStr;
            } else {
                qFatal("File is nullptr");
            }
        }

    }

    void MainWindow::deleteDate() {
        int curRow = ui->dateTable->currentRow();
        if (!ui->dateTable->selectedItems().empty()) {
            ui->dateTable->deleteDate(curRow);

            if (currentFile) {
                auto dates = ui->dateTable->GetDates();
                currentFile->remove();
                currentFile->open(QIODevice::OpenModeFlag::ReadWrite);
                foreach(auto date, dates) {
                    *tstream << QString::fromStdString(date.ToString() + '\n');
                }
            } else {
                qFatal() << "File is nullptr";
            }

        }

    }

    void MainWindow::changeDate() {
        int curRow = ui->dateTable->currentRow();
        if (!ui->dateTable->selectedItems().empty()) {
            auto newDate = constructDate(ui->dateTable->item(curRow,
                                                             0)->text());
            if (newDate) {
                ui->dateTable->changeDate(newDate.value(), curRow);
                if (currentFile) {
                    tstream->seek(DateInfo::cDateLength * curRow);
                    QString newDateStr = QString::fromStdString(newDate.value().ToString()) + '\n';
                    *tstream << newDateStr;
                } else {
                    qFatal() << "File is nullptr";
                }
            }

        }

    }

    std::optional<L2::Date> MainWindow::constructDate(QString def) {
        bool input;
        QString dateStr = QInputDialog::getText(this, "Input the date",
                                                "Date in format DD.MM.YYYY:", QLineEdit::Normal,
                                                def, &input);
        if (!input) {
            return {};
        }

        try {
            Date newDate(dateStr.toStdString());
            return newDate;
        } catch (const std::exception &ex) {
            QMessageBox::warning(this, "Warning", ex.what());
            return {};
        }
    }

    void MainWindow::openFile() {
        auto filePath = QFileDialog::getOpenFileName(this, "Open file", {}, "Date text files (*.txt)");
        if (!filePath.isEmpty()) {

            cleanUpEditor();

            currentFile = new QFile(filePath);
            bool isSuccess = currentFile->open(QFile::OpenModeFlag::ReadWrite);
            if (isSuccess) {
                try {
                    QString s = currentFile->readAll();
                    ui->dateTable->loadFromString(&s);
                    setDateEditorActive(true);
                } catch (const std::exception &ex) {
                    QMessageBox::warning(this, "Warning", ex.what());
                    cleanUpEditor();
                }
                tstream = new QTextStream(currentFile);
            }
            else{
                QMessageBox::warning(this, "Warning", "File ahs not been opened");
                cleanUpEditor();
            }
        }
    }

    void MainWindow::newFile() {
        QString filter = "Date file (*.txt)";
        auto fileName = QFileDialog::getSaveFileName(this, "New file", {}, filter, &filter);
        if(!fileName.isEmpty()){
            cleanUpEditor();

            if(!fileName.endsWith(".txt")){
                fileName += ".txt";
            }
            currentFile = new QFile(fileName);
            bool isSuccess = currentFile->open(QFile::OpenModeFlag::ReadWrite);
            if(isSuccess){
                tstream = new QTextStream(currentFile);
                setDateEditorActive(true);
            } else {
                QMessageBox::warning(this, "Warning", "Something went wrong while creating file");
            }
        }

    }


    void MainWindow::setDateEditorActive(bool isActive) {
        ui->dateTable->setEnabled(isActive);
        ui->editButton->setEnabled(isActive);
        ui->addButton->setEnabled(isActive);
        ui->deleteButton->setEnabled(isActive);
        ui->getInfo->setEnabled(isActive);
    }

    void MainWindow::cleanUpEditor() {
        if (tstream) {
            delete tstream;
            tstream = nullptr;
        }
        if (currentFile) {
            currentFile->close();
            currentFile->deleteLater();
            currentFile = nullptr;
        }

        ui->dateTable->clear();
        ui->dateTable->setRowCount(0);
        setDateEditorActive(false);
    }

    void MainWindow::getInfo() {
        auto curDate = ui->dateTable->getCurrentDate();
        auto todayDate = Date::Today();
      ui->todayLabel->setText(QString::fromStdString(todayDate.ToString()));
        if (curDate) {
            ui->nextDayLabel->setText(QString::fromStdString(curDate->NextDay().ToString()));

            if(*curDate == Date (1,1,1)){
                ui->prevDayLabel->setText(QString("--"));
            } else {
                ui->prevDayLabel->setText(QString::fromStdString(curDate->PreviousDay().ToString()));
            }
            ui->dayOfWeekLabel->setText(QString::fromStdString(curDate->GetDayOfWeekStr()));
            ui->durationLabel->setNum((int) curDate->Duration(Date::Today()));
            ui->isLeapLabel->setText( curDate->IsLeap() ? "Yes" : "No");
            ui->numOfWeekLabel->setNum(curDate->WeekNumber());

            if (bday) {
                ui->bdayDurationLabel->setNum(curDate->DaysTillYourBirthday(bday.data()));
            } else {
                ui->bdayDurationLabel->setText("-- input b-day --");
            }
        } else {
          foreach(QLabel* label, findChildren<QLabel*>(QRegularExpression("Label$"))){
            label->setText("--");
          }
        }
    }

    void MainWindow::inputBDay() {
        auto constructedBday = constructDate();
        if (constructedBday) {
            bday.reset(new Date(constructedBday.value()));
            ui->bdayLbl->setText(QString::fromStdString(constructedBday->ToString()));
        }
    }
} // L2
