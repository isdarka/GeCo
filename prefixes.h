#ifndef PREFIXES_H
#define PREFIXES_H

#include <QMainWindow>
#include <QSettings>
#include <QRect>
#include <QDesktopWidget>
namespace Ui {
class Prefixes;
}

class Prefixes : public QMainWindow
{
    Q_OBJECT

public:
    explicit Prefixes(QWidget *parent = 0);
    ~Prefixes();
    void show(QSettings *settings);
private slots:
    void on_cancel_clicked();

    void on_save_clicked();

private:
    Ui::Prefixes *ui;
    QSettings *settings;
};

#endif // PREFIXES_H
