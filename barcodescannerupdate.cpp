#include "barcodescannerupdate.h"
#include "ui_barcodescannerupdate.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>

BarcodeScannerUpdate::BarcodeScannerUpdate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BarcodeScannerUpdate)
{
            ui->setupUi(this);
            updateserial=new QSerialPort(this);
            updateserialBuffer="";
            QSqlQuery updatequery;
            QString name;
            updatequery.exec("SELECT name FROM portsettings where id='1'");
            while(updatequery.next())
            {
                name=updatequery.value(0).toString();
            }
            updateserial->setPortName(name);
            updateserial->setBaudRate(QSerialPort::Baud9600);
            updateserial->setDataBits(QSerialPort::Data8);
            updateserial->setParity(QSerialPort::NoParity);
            updateserial->setStopBits(QSerialPort::OneStop);
            updateserial->setFlowControl(QSerialPort::NoFlowControl);
            updateserial->open(QIODevice::ReadOnly);
            connect(updateserial,SIGNAL(readyRead()),this,SLOT(SerialUpdateReceived()));
}

BarcodeScannerUpdate::~BarcodeScannerUpdate()
{
    delete ui;
    updateserial->close();
}

void BarcodeScannerUpdate::product_update()
{
    QSqlQuery addquery;
    QSqlQuery checkquery;
    QString messageBox="",barcode_text="",sqlcontrol="";
    QMessageBox msgBox;
    int checkproduct=0;
    barcode_text=ui->textEdit->toPlainText();
    checkquery.exec("SELECT barkod_no FROM urunler WHERE barkod_no='"+barcode_text+"' ");
    while (checkquery.next()) {
        sqlcontrol=checkquery.value(0).toString();
    }

    if(sqlcontrol!="")
    {
        addquery.exec("SELECT urun_miktari,urun_adi,urun_skk,urun_turu FROM urunler WHERE barkod_no='"+barcode_text+"'");
        while(addquery.next())
        {
              checkproduct=addquery.value(0).toInt();
              product_name=addquery.value(1).toString();
              product_date=addquery.value(2).toString();
              product_type=addquery.value(3).toString();
        }
        ui->textEdit_2->setText(product_name);
        ui->textEdit_3->setText(product_date);
        ui->textEdit_4->setText(product_type);
        checkproduct++;
        addquery.prepare("UPDATE urunler SET urun_miktari=:checkproduct WHERE barkod_no=:checkname");
        addquery.bindValue(":checkproduct",checkproduct);
        addquery.bindValue(":checkname",barcode_text);
        addquery.exec();
        messageBox=QString::number(checkproduct);
        msgBox.setText("Product Amount increased:\t"+messageBox);
        msgBox.exec();

    }
    else if(sqlcontrol=="")
    {
        addquery.prepare("INSERT INTO urunler(barkod_no,urun_adi,urun_skk,urun_turu,urun_miktari) VALUES(:barcode_no,:product_name,:product_date,:product_type,:product_amount)");
        addquery.bindValue(":barkod_no",barcode_text);
        addquery.bindValue(":product_name",ui->textEdit_2->toPlainText());
        addquery.bindValue(":product_date",ui->textEdit_3->toPlainText());
        addquery.bindValue(":product_type",ui->textEdit_4->toPlainText());
        addquery.bindValue(":product_amount",1);
        addquery.exec();

        msgBox.setText("Product successfully added");
        msgBox.exec();        
        ui->textEdit_2->clear();
        ui->textEdit_3->clear();
        ui->textEdit_4->clear();
    }

}


void BarcodeScannerUpdate::SerialUpdateReceived()
{
        serialUpdateData=updateserial->readAll();
        updateserialBuffer=updateserialBuffer+QString::fromStdString(serialUpdateData.toStdString());
        serialUpdateData.clear();


        bufferupdate=updateserialBuffer.split("\r");
        updateserialBuffer="";
        if(bufferupdate.length()==1)
        {
            parsed_data=bufferupdate[0];
            BarcodeScannerUpdate::updateTextBox(parsed_data);
            BarcodeScannerUpdate::product_update();
        }

}
void BarcodeScannerUpdate::updateTextBox(QString port_reading)
{
    ui->textEdit->cursor();
    ui->textEdit->setText(port_reading);
}

void BarcodeScannerUpdate::on_buttonBox_accepted()
{
    QDialog::reject();
}
