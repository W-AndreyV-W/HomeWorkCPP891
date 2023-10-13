#ifndef UDPWORKER_H
#define UDPWORKER_H

#include <QObject>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QDateTime>
#include <QtConcurrent>
#include <QFuture>

#define BIND_PORT 12345
#define TEXT_PORT 12346

class UDPworker : public QObject {

    Q_OBJECT

public:

    explicit UDPworker(QObject *parent = nullptr);

    void InitSocketTime(void);
    void SendDatagram(QByteArray data);
    void initSocketText(void);
    void SendText(QByteArray text);

signals:

    void sig_sendTimeToGUI(QDateTime data);
    void sig_sendTextToGUI(QString address, uint size);

private slots:

    void readPendingDatagrams(void);
    void readPendingText(void);

private:

    QUdpSocket* serviceUdpSocket;
    QUdpSocket* serviceUdpText;

    void ReadDatagram(QNetworkDatagram datagram);
    void readText(QNetworkDatagram text);
};

#endif // UDPWORKER_H
