#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QRect>
#include <QDesktopWidget>

#include "mysqlsettings.h"
#include "DataBase/connection.h"
#include "prefixes.h"
#include "GeCo/geco.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QSettings  *appSettings;
    QFileDialog fileDialog;

private slots:
    void on_actionMySql_Configuration_triggered();

    void on_actionExit_triggered();

    void on_actionPath_To_Save_triggered();

    void on_actionTest_Connection_triggered();

    void on_actionLoad_Model_triggered();

    void on_actionPrefixes_triggered();

    void on_actionDefault_Model_triggered();

    void on_actionGenerate_triggered();

private:
    Ui::MainWindow *ui;
    MysqlSettings mysqlSettingsForm;
    Prefixes prefixesForm;
    Connection *connection;
    GeCo geco;
};

#endif // MAINWINDOW_H
