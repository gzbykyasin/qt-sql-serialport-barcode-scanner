#include "widget.h"
#include "ui_widget.h"
#include "barcodescannerupdate.h"
#include "barcodescannerdeleted.h"

#include <QSerialPortInfo>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    serial=new QSerialPort(this);

    QList<QSerialPortInfo> com_ports = QSerialPortInfo::availablePorts();
    QSerialPortInfo port;
    foreach(port, com_ports)
    {
        ui->PortComboBox->addItem(port.portName()+","+port.manufacturer());
    }

    const QString DRIVER("QSQLITE");
    if(QSqlDatabase::isDriverAvailable(DRIVER))
    {
        mydb = QSqlDatabase::addDatabase(DRIVER);
        mydb.setDatabaseName("C:/SqLite/barcode.db");
        if(mydb.open())
        {
            ui->SqlSelectLabel->setStyleSheet("color: rgb(0, 170, 0)");
            ui->SqlSelectLabel->setText("DATABASE CONNECTION READY");


        }
        else
        {
            ui->SqlSelectLabel->setStyleSheet("color: rgb(255, 0, 0)");
            ui->SqlSelectLabel->setText("DATABASE CONNECTION READY "+mydb.lastError().text());
        }

    }

}

Widget::~Widget()
{
    delete ui;
    serial->close();
    mydb.close();
}


void Widget::on_AddPushButton_clicked()
{


        PortNameBuffer=ui->PortComboBox->currentText().split(",");
        QSqlQuery myquery;
        if(!myquery.exec("UPDATE portsettings SET name='"+PortNameBuffer.first()+"' WHERE id='1'"))
        {
               ui->SqlSelectLabel->setText("PORT UPDATING FAILURE "+myquery.lastError().text());
        }

        BarcodeScannerUpdate bcu;      
        bcu.exec();

}


void Widget::on_UpdatePushButton_clicked()
{
    QSqlQueryModel *urunquerymodel=new QSqlQueryModel;

    urunquerymodel->setQuery("SELECT * FROM urunler");
    ui->tableView->setModel(urunquerymodel);
}

void Widget::on_DeletePushButton_clicked()
{
    PortNameBuffer=ui->PortComboBox->currentText().split(",");
    QSqlQuery myquery;
    if(!myquery.exec("UPDATE portsettings SET name='"+PortNameBuffer.first()+"' WHERE id='1'"))
    {
           ui->SqlSelectLabel->setText("PORT UPDATING FAILURE "+myquery.lastError().text());
    }

    BarcodeScannerDeleted bcd;
    bcd.exec();
}
