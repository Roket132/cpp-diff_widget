#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "reader.h"
#include "sha256.h"
#include "file_data.h"

#include <QApplication>
#include <QFileDialog>
#include <QStringListModel>
#include <iostream>
#include <string>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    //ui->statusBar->addPermanentWidget(ui->progressBar);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::add_Items_Main(QStringList list_Name)
{
    ui->listMainFile->clear();
    for (int i = 0; i < list_Name.length(); i++) {
        ui->listMainFile->addItem(new QListWidgetItem(QIcon(":/icons/file"), list_Name[i]));
    }
}

void MainWindow::add_Items_Same(QStringList list_Name)
{
    ui->listSameFile->clear();
    for (int i = 0; i < list_Name.length(); i++) {
        ui->listSameFile->addItem(new QListWidgetItem(QIcon(":/icons/file"), list_Name[i]));
    }
}


void MainWindow::on_Reload_triggered()
{
    /*if (DIRECTORY_NAME != "") {
        add_Items_Main(read(DIRECTORY_NAME, ));
    }*/
}


void MainWindow::onActiontestTriggered(){}

void MainWindow::on_Open_triggered()
{
    QString str;
    try {
        str = QFileDialog::getExistingDirectory(0, "Directory Dialog", "");
    } catch (...) {
        str = QString::fromStdString(DIRECTORY_NAME);
    }

    DIRECTORY_NAME = str.toStdString();
    if (DIRECTORY_NAME != "") {
        QProgressBar *bar = new QProgressBar();
        ui->statusBar->addPermanentWidget(bar);

        add_Items_Main(read(DIRECTORY_NAME, bar));

        ui->statusBar->removeWidget(bar);
        delete bar;
    }
}

void MainWindow::on_listMainFile_itemPressed(QListWidgetItem *item)
{
    QString QName = item->text();
    std::string name = QName.toStdString();
    PRESSED_MAIN = name;

    add_Items_Same(get_same(PRESSED_MAIN));
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    QString str = QString(QChar(key));
    if (str == Qt::Key_Delete) {
        std::cout << "GO delete" << std::endl;
        for (auto it : ui->listSameFile->selectedItems()) {
            QString QStr = it->text();
            fs::path del_path = DIRECTORY_NAME + "/" + QStr.toStdString();
            std::cout << del_path << std::endl;
            fs::remove(del_path);
        }
        QProgressBar *bar = new QProgressBar();
        ui->statusBar->addPermanentWidget(bar);

        add_Items_Main(read_update(DIRECTORY_NAME, bar));
        add_Items_Same(get_same(PRESSED_MAIN));

        ui->statusBar->removeWidget(bar);
        delete bar;
    }
}
