#ifndef BARCODESCANNERDELETED_H
#define BARCODESCANNERDELETED_H

#include <QDialog>
#include <QSerialPort>
#include <QByteArray>
#include <QSqlDatabase>
#include <QStringList>

namespace Ui {
class BarcodeScannerDeleted;
}

class BarcodeScannerDeleted : public QDialog
{
    Q_OBJECT

public:
    explicit BarcodeScannerDeleted(QWidget *parent = 0);
    ~BarcodeScannerDeleted();

private slots:
    void product_deleted();

    void SerialUpdateReceived();

    void updateTextBox(const QString port_reading);

    void on_buttonBox_accepted();

private:
    Ui::BarcodeScannerDeleted *ui;
    QSerialPort *updateserial=nullptr;
    QSqlDatabase updatedb;
    QByteArray serialUpdateData;
    QString updateserialBuffer,parsed_data;
    QString product_name,product_date,product_type;
    QStringList bufferupdate;
};

#endif // BARCODESCANNERDELETED_H
