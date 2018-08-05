#ifndef BARCODESCANNERUPDATE_H
#define BARCODESCANNERUPDATE_H

#include <QDialog>
#include <QSerialPort>
#include <QByteArray>
#include <QSqlDatabase>
#include <QStringList>

namespace Ui {
class BarcodeScannerUpdate;
}

class BarcodeScannerUpdate : public QDialog
{
    Q_OBJECT

public:
    explicit BarcodeScannerUpdate(QWidget *parent = 0);
    ~BarcodeScannerUpdate();

private slots:
    void product_update();

    void SerialUpdateReceived();

    void updateTextBox(const QString port_reading);

    void on_buttonBox_accepted();

private:
    Ui::BarcodeScannerUpdate *ui;
    QSerialPort *updateserial=nullptr;
    QSqlDatabase updatedb;
    QByteArray serialUpdateData;
    QString updateserialBuffer,parsed_data;
     QString product_name,product_date,product_type;
    QStringList bufferupdate;
};

#endif // BARCODESCANNERUPDATE_H
