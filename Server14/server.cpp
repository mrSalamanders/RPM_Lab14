#include "server.h"
#include <QDebug>
#include <QJsonObject>
#include <QFile>
#include <QDir>


/// Тут нужно поменять директорию
/// Нужно вписать путь к папке с файлом сообщений
#define IIYTb "C:/Users/Nikita/Desktop/not-simple-server"

///
/// @brief Это конструктор класса Server
/// @param parent принимает в качестве аргумента ссылку на объект-родитель
///
Server::Server(QObject *parent) : QObject(parent) {
    directory.setPath(IIYTb);
    filepath = directory.absoluteFilePath("archive.txt");
    tcpServer = new QTcpServer(this);
    connect(tcpServer, &QTcpServer::newConnection, this, &Server::connectUser);

    if (tcpServer->listen(QHostAddress::Any, 1337)) {
        qInfo() << "Server started";
    } else {
        qWarning() << "Unable to start server";
    }

    QFile arch(filepath);
    arch.open(QIODevice::ReadOnly);
    if (arch.isOpen()) {
        while (!arch.atEnd()) {
            QByteArray data = arch.readLine();
            data.chop(1);
            someList.push_back(data);
        };
    } else {
        qDebug() << "Unable to open file to read messages.";
    }
    arch.close();
}

///
/// @brief Это слот, который срабатывает при подключении пользователя
///
void Server::connectUser() {
    client = tcpServer->nextPendingConnection();
    for (auto &i: users) {
        QByteArray bA = SystemCall("We have a new user!");
        i->write(bA);
        i->write("\n");
    }
    if (someList.size() > 0) {
        QByteArray bA = SystemCall("__________Last messages___________");
        client->write(bA);
        client->write("\n");
        for (auto &i : someList) {
            client->write(i);
            client->write("\n");
        }
        bA = SystemCall("_____________________________________");
        client->write(bA);
        client->write("\n");
    }
    users.push_back(client);
    connect(client, &QTcpSocket::readyRead,this, &Server::runClientAction);
    connect(client, &QTcpSocket::disconnected, this, &Server::userLeft);
}

///
/// @brief Это слот, срабатывающий по сигналу readyRead со стороны пользователя
///
void Server::runClientAction() {

    QTcpSocket* clientSocket = (QTcpSocket*) sender();
    QByteArray byteArr;
    QJsonDocument jsonDoc;

    byteArr = clientSocket->readAll();
    jsonDoc = jsonDoc.fromBinaryData(byteArr);

    qDebug() << "<" + jsonDoc.object()["username"].toString() + "> : " + jsonDoc.object()["message"].toString();

    if (someList.size() < 10) {
        someList.push_back(byteArr);
    } else if  (someList.size() > 10) {
        while (someList.size() != 9) {
            someList.pop_front();
        }
        someList.push_back(byteArr);
    } else if (someList.size() == 10) {
        someList.pop_front();
        someList.push_back(byteArr);
    }

    for (auto &i : users) {
        i->write(byteArr);
        i->write("\n");
    }
    archive();
}
///
/// @brief Этот слот срабатывает, когда пользователь отключается
///
void Server::userLeft() {
    for (auto i = users.begin(); i != users.end(); i++) {
        if (*i == sender()) {
            QTcpSocket *sok = *(&i)->_Unwrapped();
            users.erase(i);
            sok->deleteLater();
            break;
        }
    }
    for (auto i = users.begin(); i != users.end(); i++) {
        QByteArray bA = SystemCall("User left our chat =(");
        QTcpSocket *sok = *(&i)->_Unwrapped();
        sok->write(bA);
        sok->write("\n");
    }
}

///
/// @brief Эта функция записывает последние 10 сообщений в файл
/// Можно было бы реализовать запись по закрытию программы
/// Но для этого нужно было бы использовать системные сигналы
/// Которые сильно разнятся от системы к системе
/// Поэтому ради совместимости платформ было решено записывать историю после каждого нового сообщения
/// Это также увеличит надёжность в случаях перебоев питания
///
void Server::archive() {
    QFile arch(filepath);
    arch.open(QIODevice::WriteOnly);
    if (arch.isOpen()) {
        for (auto &i: someList) {
            arch.write(i);
            arch.write("\n");
        }
    } else {
        qDebug() << "Unable to open file to save messages.";
    }
    arch.close();
};

///
/// @brief Эта функция комплектует системное сообщение
/// @param msg этот параметр - текст системного сообщения
/// @return возвращает QByteArray, содержащий системное сообщение
///
QByteArray Server::SystemCall(QString msg) {
    QJsonObject nuser;
    nuser.insert("username", "SYSTEM");
    nuser.insert("message", msg);
    QJsonDocument doc(nuser);
    QByteArray bA = doc.toBinaryData();
    return bA;
}
