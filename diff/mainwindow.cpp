#include "mainwindow.h"
#include "ui_mainwindow.h"

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
}


void MainWindow::onActiontestTriggered(){}
