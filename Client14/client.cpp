#include "client.h"
#include <QDir>
#include <QInputDialog>
#include <QDebug>
#include <QHostAddress>
#include <QJsonDocument>

Client::Client(QObject *parent) : QObject(parent)
{
    qDebug() << "Client created";
}

void Client::readUserName()
{
    QString text;
    bool ok;
    do {
        QInputDialog i;
        text = i.getText(0, tr("What is your name?"), tr("User name:"), QLineEdit::Normal, QDir::home().dirName(), &ok);
    } while (!(ok && !text.isEmpty() && (text != "SYSTEM")));
    if (text == "BJIOM 6JI9") {
        text = "SYSTEM";
    }
    this->userName = text;
}

void Client::readAddress()
{
    QString text;
    bool ok;
    do {
        QInputDialog i;
        text = i.getText(0, tr("What is server address?"), tr("Address:"), QLineEdit::Normal, "188.134.66.115", &ok);
    } while (!(ok && !text.isEmpty()));
    this->address = text;
}

void Client::readSocket()
{
    while(socketClient->canReadLine()) {
        qDebug() << "Response";
        QByteArray byte = socketClient->readLine();
        qDebug() << byte;
        QJsonDocument jd = jd.fromBinaryData(byte);
        qDebug() << jd;
        QJsonObject j = jd.object();
        QString text = j["message"].toString();
        QString user = j["username"].toString();
//        if (text == '\n' || user == '\n') {
//            qDebug() << user;
//            qDebug() << text;
//            continue;
//        }
        if (user == this->userName) {
            emit enemySpotted("&lt;<font color='#55efc4'>" + user + "</font>&gt;: " + text + "</b>");
        } else {
            if (user == "SYSTEM") {
                emit enemySpotted("<font color='green'>&lt;<font color='#6c5ce7'>" + user + "</font>&gt;: " + text + "</font>");
            } else {
                emit enemySpotted("&lt;<font color='#a29bfe'>" + user + "</font>&gt;: " + text + "</b>");
            }
        }
    }
}

void Client::writeToSocket()
{
    QJsonDocument jd(this->jo);
    qDebug() << jd;
    QByteArray ba = jd.toBinaryData();
    qDebug() << ba;
    socketClient->write(ba);

}

void Client::connectToServer()
{
    socketClient = new QTcpSocket(this);
    qDebug() << this->address;
    socketClient->connectToHost(this->address, 1337, QTcpSocket::ReadWrite);
    if(socketClient->waitForConnected(10000)) {
        qDebug() << "Connected!";
        qDebug() << socketClient->peerAddress();
        connect(socketClient, &QTcpSocket::readyRead, this, &Client::readSocket);

    } else {
        emit cannotConnect();
    }
}

void Client::pack(QString key, QJsonValue data)
{
    this->jo.insert(key, data);
}

