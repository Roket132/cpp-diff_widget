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

    file_data &file_data = file_data.get_instance();
    std::pair<size_t, bits> key = file_data._KEYS[name];
    auto row = file_data._FILES[key];

    fs::path item_file;
    for (auto it : row) {
        if (it.filename() == name) {
            item_file = it;
            break;
        }
    }


    std::string sha256 = file_data._SHA256[item_file];
    std::vector<fs::path> same;

    for (auto it : row) {
        if (it.filename() != name) {
            std::string sha256_it = file_data._SHA256[it];
            if (sha256 == sha256_it) {
                same.push_back(it);
            }
        }
    }

    QStringList List;

    for (auto it : same) {
        List << QString::fromStdString(it.filename());
    }

    add_Items_Same(List);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    QString str = QString(QChar(key));
    if (str == Qt::Key_Delete) {

    }
}







