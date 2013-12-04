#ifndef MYSQLSETTINGS_H
#define MYSQLSETTINGS_H

#include <QMainWindow>
#include <QSettings>
#include <QRect>
#include <QDesktopWidget>
#include <QMessageBox>
namespace Ui {
class MysqlSettings;
}

class MysqlSettings : public QMainWindow
{
    Q_OBJECT

public:
    explicit MysqlSettings(QWidget *parent = 0);
    ~MysqlSettings();
    void show(QSettings *settings);
private slots:
    void on_pushButton_2_clicked();

    void on_saveButton_clicked();

private:
    Ui::MysqlSettings *ui;
    QSettings *settings;
};

#endif // MYSQLSETTINGS_H
