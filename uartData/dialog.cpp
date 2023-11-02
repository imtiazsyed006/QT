#include "dialog.h"
#include "ui_dialog.h"
#include "QSerialPort"
#include "QSerialPortInfo"
#include <string>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    serialBuffer = "";
    ui->pushButton->setText("Connect");
    arduino = new QSerialPort(this);
    const auto portInfos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &portInfo : portInfos)
    {
        ui->comboBox->addItem(portInfo.portName());
    }
    currentPortName = ui->comboBox->currentText();
    qDebug() << currentPortName;


    QObject::connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(getCurrentPortName()));
//    QObject::connect(ui->pushButton,&QPushButton::clicked,this,&Dialog::connect);
    QObject::connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(connect()));
    QObject::connect(ui->refresh_button,SIGNAL(clicked(bool)),this,SLOT(refreshComboBox()));
    QObject::connect(ui->send_Button,SIGNAL(clicked(bool)),this,SLOT(sendSerialData()));
}

Dialog::~Dialog()
{
    if (arduino->isOpen())
    {
        qDebug() << "I am closing";
        arduino->close();
    }
    delete ui;

}

void Dialog::readSerial()
{
    data = arduino->readLine();
    serialBuffer = QString::fromStdString(data.toStdString());
    ui->textEdit->insertPlainText(serialBuffer);
    scrollBar = ui->textEdit->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());
    serialBuffer.replace("\r\n","");
    Dialog::updateLCD(serialBuffer);
}

void Dialog::updateLCD(const QString value)
{
    ui->lcdNumber->display(value);
}

void Dialog::connect()
{
    if (arduino->isOpen())
    {
        arduino->close();
        delete arduino; // Delete the old QSerialPort instance
        arduino = nullptr; // Set the pointer to nullptr
        arduino = new QSerialPort(this);
        ui->pushButton->setText("Connect");
    }
    else
    {
        arduino->setPortName(currentPortName);
        arduino->open(QSerialPort::ReadWrite);
        arduino->setBaudRate(QSerialPort::Baud9600);
        arduino->setDataBits(QSerialPort::Data8);
        arduino->setFlowControl(QSerialPort::NoFlowControl);
        arduino->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);
        QObject::connect(arduino, SIGNAL(readyRead()),this, SLOT(readSerial()));
        ui->pushButton->setText("Disconnect");
    }
}

void Dialog::getCurrentPortName()
{
    currentPortName = ui->comboBox->currentText();
    qDebug() << currentPortName;
}

void Dialog::refreshComboBox()
{
    ui->comboBox->clear();
    const auto portInfos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &portInfo : portInfos)
    {
        ui->comboBox->addItem(portInfo.portName());
    }
    currentPortName = ui->comboBox->currentText();
    qDebug() << currentPortName;
}
void Dialog::sendSerialData()
{
    if (arduino->isOpen())
    {
        QString temp = ui->textEdit_2->toPlainText();
        QByteArray array = temp.toUtf8();
        arduino->write(array);
    }
}
