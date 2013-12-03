#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->appSettings = new QSettings("IsdarkA","GeCo");
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Open mysql setting
void MainWindow::on_actionMySql_Configuration_triggered()
{
    mysqlSettingsForm.show(this->appSettings);
}
