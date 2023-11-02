#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QSerialPort>
#include <QScrollBar>

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();
private slots:
    void readSerial();
    void updateLCD(const QString value);
    void connect();
    void getCurrentPortName();
    void refreshComboBox();
    void sendSerialData();
private:
    Ui::Dialog *ui;
    QSerialPort *arduino;
    QByteArray data;
    QString serialBuffer;
    QString currentPortName;
    QScrollBar *scrollBar;
};
#endif // DIALOG_H
