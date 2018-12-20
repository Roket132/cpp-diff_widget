#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "reader.h"

#include <QListWidget>
#include <QMainWindow>
#include <QProgressBar>
#include <QKeyEvent>

static std::string DIRECTORY_NAME = "";
static std::string PRESSED_MAIN = "";



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

    void add_Items_Main(QStringList name);

    void add_Items_Same(QStringList name);

private slots:    
    void onActiontestTriggered();

    void on_Reload_triggered();

    void on_Open_triggered();

    void on_listMainFile_itemPressed(QListWidgetItem *item);

protected:
    virtual void keyPressEvent(QKeyEvent *event);
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
