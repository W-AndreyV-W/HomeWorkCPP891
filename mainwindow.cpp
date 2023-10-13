#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    udpWorker = new UDPworker(this);
    timer = new QTimer(this);

    udpWorker->InitSocketTime();
    udpWorker->initSocketText();

    QObject::connect(udpWorker, &UDPworker::sig_sendTimeToGUI, this, &MainWindow::DisplayTime);
    QObject::connect(udpWorker, &UDPworker::sig_sendTextToGUI, this, &MainWindow::DisplayText);
    QObject::connect(timer, &QTimer::timeout, this, [&] () {

        QDateTime dateTime = QDateTime::currentDateTime();
        QByteArray dataToSend;
        QDataStream outStr(&dataToSend, QIODevice::WriteOnly);

        outStr << dateTime;

        udpWorker->SendDatagram(dataToSend);
        timer->start(TIMER_DELAY);
    });
}

MainWindow::~MainWindow() {

    delete ui;
}
\


void MainWindow::DisplayTime(QDateTime data) {

    counterPck++;

    if(counterPck % 20 == 0) {

        ui->te_result->clear();
    }

    ui->te_result->append("Текущее время: " + data.toString() + ". " +
                          "Принято пакетов " + QString::number(counterPck));
}

void MainWindow::DisplayText(QString address, uint size) {

    ui->te_result->append("Принято сооьбщение от " + address +
                          ", размер сообщения(байт) " + QString::number(size) + ".");
}



void MainWindow::on_pb_start_clicked() {

    timer->start(TIMER_DELAY);
}

void MainWindow::on_pb_stop_clicked() {

    timer->stop();
}

void MainWindow::on_pb_sendDatagram_clicked() {

    udpWorker->SendText(ui->te_message->toPlainText().toUtf8());
}

