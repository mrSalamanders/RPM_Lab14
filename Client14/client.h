#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>

class Client : public QObject
{
    Q_OBJECT
public:
    bool isConnected = false; /// подключение к серверу
    QString userName; /// ник конкретного клиента
    QString address; /// адрес сервера
    QTcpSocket* socketClient; /// сокет клиента
    QJsonObject jo; /// JSON-документ для сообщения
    /**
     * @brief Client конструктор
     * @param parent родительский объект
     */
    explicit Client(QObject *parent = nullptr);
    /**
     * @brief readUserName считать ник пользователя
     */
    void readUserName();
    /**
     * @brief readAddress считать адрес сервера
     */
    void readAddress();
    /**
     * @brief readSocket считать данные из сокета
     */
    void readSocket();
    /**
     * @brief writeToSocket отправить в сокет
     */
    void writeToSocket();
    /**
     * @brief connectToServer подключиться к серверу
     */
    void connectToServer();
    /**
     * @brief pack функция-упаковщик в JSON
     * @param key ключ, по которому класть данные
     * @param data данные
     */
    void pack(QString key, QJsonValue data);
signals:
    /**
     * @brief enemySpotted сигнал обнаружения данных в сокете
     * @param mes текст того, что нашлось
     */
    void enemySpotted(QString mes);
    /**
     * @brief cannotConnect сигнал невозможности подключения к серверу
     */
    void cannotConnect();
};

#endif // CLIENT_H
