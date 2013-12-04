#include "mysqlsettings.h"
#include "ui_mysqlsettings.h"

MysqlSettings::MysqlSettings(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MysqlSettings)
{
    ui->setupUi(this);
    // Move Window to Center
    QRect position = frameGeometry();
    position.moveCenter(QDesktopWidget().availableGeometry().center());
    move(position.topLeft());
    // /END Move Window to Center
}

MysqlSettings::~MysqlSettings()
{
    delete ui;
}

void MysqlSettings::show(QSettings *settings)
{
    this->settings = settings;
    this->settings->beginGroup("MysqlSettings");
    ui->hostname->setText(this->settings->value("hostname").toString());
    ui->username->setText(this->settings->value("username","3306").toString());
    ui->password->setText(this->settings->value("password").toString());
    ui->schema->setText(this->settings->value("schema").toString());
    ui->port->setText(this->settings->value("port").toString());

    settings->endGroup();
    this->setVisible(true);
}

// Close window
void MysqlSettings::on_pushButton_2_clicked()
{
    this->close();
}

// Save and close window
void MysqlSettings::on_saveButton_clicked()
{
    this->settings->beginGroup("MysqlSettings");
    this->settings->setValue("hostname",ui->hostname->text());
    this->settings->setValue("username",ui->username->text());
    this->settings->setValue("password",ui->password->text());
    this->settings->setValue("schema",ui->schema->text());
    this->settings->setValue("port",ui->port->text());
    this->settings->endGroup();
    QMessageBox::information(this, "Info", "MySql Settings Saved.");

    this->close();
}
