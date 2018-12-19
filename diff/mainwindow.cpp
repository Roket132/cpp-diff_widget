#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "reader.h"

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


void MainWindow::on_Reload_triggered()
{
    if (ui->listMainFile->count() > 0) {
        ui->listMainFile->item(0)->setSelected(true);
        ui->listMainFile->item(0)->setSelected(false);
    }
    if (DIRECTORY_NAME != "") {
        add_Items_Main(read(DIRECTORY_NAME));
    }
}


void MainWindow::onActiontestTriggered(){}

void MainWindow::on_Open_triggered()
{
    QString str = QFileDialog::getExistingDirectory(0, "Directory Dialog", "");
    DIRECTORY_NAME = str.toStdString();
    add_Items_Main(read(DIRECTORY_NAME));
}
