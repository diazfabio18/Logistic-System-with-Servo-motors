#include "mainwindow.h"
#include "ui_mainwindow.h"

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


void MainWindow::on_BT_exit_clicked()
{
    destroy(ui);
}

void MainWindow::on_BT_conect_clicked()
{
    if(1){
        
    }
    else qErrnoWarning("Error trying to conect");
}
