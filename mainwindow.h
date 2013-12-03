#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

#include "mysqlsettings.h"

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
private slots:
    void on_actionMySql_Configuration_triggered();

private:
    Ui::MainWindow *ui;
    MysqlSettings mysqlSettingsForm;
};

#endif // MAINWINDOW_H
