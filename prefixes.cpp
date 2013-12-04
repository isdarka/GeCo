#include "prefixes.h"
#include "ui_prefixes.h"

Prefixes::Prefixes(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Prefixes)
{
    ui->setupUi(this);
    // Move Window to Center
    QRect position = frameGeometry();
    position.moveCenter(QDesktopWidget().availableGeometry().center());
    move(position.topLeft());
    // /END Move Window to Center
}

Prefixes::~Prefixes()
{
    delete ui;
}

void Prefixes::show(QSettings *settings)
{
    this->settings = settings;
    this->settings->beginGroup("parameters");
    ui->prefixes->setText(this->settings->value("prefixes").toString());
    this->settings->endGroup();
    this->setVisible(true);
}

void Prefixes::on_cancel_clicked()
{
    this->close();
}

void Prefixes::on_save_clicked()
{
    this->settings->beginGroup("parameters");
    this->settings->setValue("prefixes",ui->prefixes->text());
    this->settings->endGroup();
    this->close();

}
