#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QByteArray>
#include <QSqlDatabase>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:

    void on_AddPushButton_clicked();

    void on_UpdatePushButton_clicked();

    void on_DeletePushButton_clicked();

private:
    Ui::Widget *ui;
    QSerialPort *serial=nullptr;
    QStringList PortNameBuffer;
    QSqlDatabase mydb;
};

#endif // WIDGET_H
