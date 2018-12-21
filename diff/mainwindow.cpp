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
    ui->listSameFile->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->choised_dir->setText("Directory: ");
    ui->statusBar->addWidget(ui->choised_dir);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::add_Items_Main(QStringList list_Name)
{
    ui->listMainFile->clear();
    for (int i = 0; i < list_Name.length(); i++) {
        QListWidgetItem *item = new QListWidgetItem(QIcon(":/icons/file"), list_Name[i]);
        ui->listMainFile->addItem(item);
        if (PRESSED_MAIN == list_Name[i].toStdString()) {
            item->setSelected(true);
        }
    }
}

void MainWindow::add_Items_Same(QStringList list_Name)
{
    ui->listSameFile->clear();
    for (int i = 0; i < list_Name.length(); i++) {
        ui->listSameFile->addItem(new QListWidgetItem(QIcon(":/icons/file"), list_Name[i]));
    }
}

void MainWindow::on_Play_triggered()
{
    QProgressBar *bar = new QProgressBar();
    ui->statusBar->addPermanentWidget(bar);
    try {
        add_Items_Main(read(DIRECTORY_NAME, bar, MODE));
        ui->statusBar->removeWidget(bar);
        delete bar;
    } catch (...) {
        std::cout << "enter directory" << std::endl;
        delete bar;
    }
    MODE_COMPILED = MODE;
}

void MainWindow::on_Reload_triggered()
{
    QProgressBar *bar = new QProgressBar();
    ui->statusBar->addPermanentWidget(bar);
    try {
        add_Items_Main(read_update(DIRECTORY_NAME, bar, MODE));
        ui->statusBar->removeWidget(bar);
        delete bar;
    } catch (...) {
        std::cout << "failed update" << std::endl;
        delete bar;
    }
    MODE_COMPILED = MODE;
}

void MainWindow::onActiontestTriggered(){}

void MainWindow::on_Open_triggered()
{
    QString str;
    try {
        str = QFileDialog::getExistingDirectory(nullptr, "Directory Dialog", "");
        if (str == "") {
            throw "todo";
        }
    } catch (...) {
        std::cout << "stop" << std::endl;
        str = QString::fromStdString(DIRECTORY_NAME);
    }

    DIRECTORY_NAME = str.toStdString();
    ui->choised_dir->setText("Directory: " + QString::fromStdString(DIRECTORY_NAME));

}

void MainWindow::on_listMainFile_itemPressed(QListWidgetItem *item)
{
    QString QName = item->text();
    std::string name = QName.toStdString();
    PRESSED_MAIN = name;

    add_Items_Same(get_same(PRESSED_MAIN, MODE_COMPILED));
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    QString str = QString(QChar(key));
    if (str == Qt::Key_Delete && ui->listSameFile->selectedItems().size() > 0) {
        delete_selected_files();
    }
}

void MainWindow::delete_selected_files()
{
    try {
        for (auto it : ui->listSameFile->selectedItems()) {
            QString QStr = it->text();
            fs::path del_path = DIRECTORY_NAME + "/" + QStr.toStdString();
            fs::remove(del_path);
        }
    } catch(fs::filesystem_error e) {
        std::cout << e.what() << std::endl;
    }
    QProgressBar *bar = new QProgressBar();
    ui->statusBar->addPermanentWidget(bar);

    add_Items_Main(read_update(DIRECTORY_NAME, bar, MODE));
    add_Items_Same(get_same(PRESSED_MAIN, MODE_COMPILED));

    ui->statusBar->removeWidget(bar);
    delete bar;
}


void MainWindow::on_speed_triggered()
{
    if (MODE == 1) {
        ui->speed->setIcon(QIcon(":/icons/fast"));
    } else {
        ui->speed->setIcon(QIcon(":/icons/slow"));
    }
    MODE = (MODE == 1 ? 2 : 1);
}

void MainWindow::on_Delete_triggered()
{
    if (ui->listSameFile->selectedItems().size() > 0) {
        delete_selected_files();
    }
}
