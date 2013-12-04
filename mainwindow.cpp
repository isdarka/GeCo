#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Move Window to Center
    QRect position = frameGeometry();
    position.moveCenter(QDesktopWidget().availableGeometry().center());
    move(position.topLeft());
    // /END Move Window to Center
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

//Close Main Window
void MainWindow::on_actionExit_triggered()
{
    this->close();
}

//Path to Save
void MainWindow::on_actionPath_To_Save_triggered()
{
    this->appSettings->beginGroup("parameters");
    this->fileDialog.setFileMode(QFileDialog::Directory);
    this->fileDialog.setDirectory(this->appSettings->value("pathToSave",QDir::currentPath()).toString());
    this->fileDialog.setOption(QFileDialog::ShowDirsOnly, true);

    if ( this->fileDialog.exec() == QDialog::Accepted )
    {
        this->appSettings->setValue("pathToSave",this->fileDialog.selectedFiles());
        QMessageBox::information(this, "Info", "Path Saved.");
    }
    this->appSettings->endGroup();

}


// Test Connection
void MainWindow::on_actionTest_Connection_triggered()
{
    try{
        this->connection = new Connection(this->appSettings);
        this->connection->connect();
        if(this->connection->connect().isOpen())
            QMessageBox::information(this, "Info", "Connection Ok");
        else{
           this->connection->connect().open();
            if(this->connection->connect().isOpen())
                QMessageBox::information(this, "Info", "Connection Ok");
            else
                QMessageBox::warning(this, "Info", QString("Error:"));
        }
    }catch(QString e){
        QMessageBox::warning(NULL, "Error", e);
    }


}

// LOAD Model
void MainWindow::on_actionLoad_Model_triggered()
{
    try{
        this->appSettings->beginGroup("parameters");
        this->fileDialog.setFileMode(QFileDialog::ExistingFile);
        this->fileDialog.setDirectory(this->appSettings->value("pathToSave",QDir::currentPath()).toString());
        this->fileDialog.setNameFilter("XML (*.xml)");
        if ( this->fileDialog.exec() == QDialog::Accepted )
        {
            this->appSettings->setValue("fileToLoad",this->fileDialog.selectedFiles());

            if(!QFile::exists(this->appSettings->value("fileToLoad").toString()))
                throw QString("File ").append(this->appSettings->value("fileToLoad").toString().append(" don't exist!"));
            QMessageBox::information(this, "Info", "Model Loaded.");
        }
        this->appSettings->endGroup();

    }catch(QString e){
        QMessageBox::warning(NULL, "Error", e);
    }
}
