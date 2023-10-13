#include "udpworker.h"

UDPworker::UDPworker(QObject *parent)
    : QObject{parent}
{

}



void UDPworker::InitSocketTime() {

    serviceUdpSocket = new QUdpSocket(this);

    serviceUdpSocket->bind(QHostAddress::LocalHost, BIND_PORT);

    QObject::connect(serviceUdpSocket, &QUdpSocket::readyRead, this, &UDPworker::readPendingDatagrams);
}

void UDPworker::initSocketText() {

    serviceUdpText = new QUdpSocket(this);

    serviceUdpText->bind(QHostAddress::LocalHost, TEXT_PORT);

    QObject::connect(serviceUdpText, &QUdpSocket::readyRead, this, &UDPworker::readPendingText);
}



void UDPworker::ReadDatagram(QNetworkDatagram datagram) {

    QByteArray data;
    data = datagram.data();


    QDataStream inStr(&data, QIODevice::ReadOnly);
    QDateTime dateTime;
    inStr >> dateTime;

    emit sig_sendTimeToGUI(dateTime);
}

void UDPworker::readText(QNetworkDatagram datagram) {

    QByteArray data = datagram.data();

    emit sig_sendTextToGUI(datagram.senderAddress().toString(), data.size());
}



void UDPworker::SendDatagram(QByteArray data) {

    serviceUdpSocket->writeDatagram(data, QHostAddress::LocalHost, BIND_PORT);
}

void UDPworker::SendText(QByteArray text) {

    serviceUdpText->writeDatagram(text, QHostAddress::LocalHost, TEXT_PORT);
}



void UDPworker::readPendingDatagrams(void) {

    while(serviceUdpSocket->hasPendingDatagrams()) {

        QNetworkDatagram datagram = serviceUdpSocket->receiveDatagram();

        ReadDatagram(datagram);
    }
}

void UDPworker::readPendingText() {

    while(serviceUdpText->hasPendingDatagrams()) {

        readText(serviceUdpText->receiveDatagram());
    }
}
