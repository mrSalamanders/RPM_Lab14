#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QtNetwork>
#include <list>

class Server : public QObject
{
    Q_OBJECT
private:
    /**
     * @brief tcpServer
     * Сервер отвечающий за работу TCP соединения
     */
    QTcpServer* tcpServer;
    /**
     * @brief client
     * Переменная которая будет хранить соединениес клиентом
     */
    QTcpSocket* client;

public:
    ///
    /// @brief Это конструктор класса Server
    /// @param parent принимает в качестве аргумента ссылку на объект-родитель
    ///
    explicit Server(QObject *parent = nullptr);
    /// Это список подключённых пользователей
    std::list<QTcpSocket*> users;
    /// Это список последних сообщений чата
    std::list<QByteArray> someList;
    ///
    /// @brief Эта функция комплектует системное сообщение
    /// @param msg этот параметр - текст системного сообщения
    /// @return возвращает QByteArray, содержащий системное сообщение
    ///
    QByteArray SystemCall(QString msg);
    /// Это директория с файлом-архивом
    QDir directory;
    /// Это абсолютный путь к файлу-архиву
    QString filepath;
    ///
    /// @brief Эта функция записывает последние 10 сообщений в файл
    /// Можно было бы реализовать запись по закрытию программы
    /// Но для этого нужно было бы использовать системные сигналы
    /// Которые сильно разнятся от системы к системе
    /// Поэтому ради совместимости платформ было решено записывать историю после каждого нового сообщения
    /// Это также увеличит надёжность в случаях перебоев питания
    ///
    void archive();
private slots:
    ///
    /// @brief Это слот, который срабатывает при подключении пользователя
    ///
    void connectUser();
    ///
    /// @brief Это слот, срабатывающий по сигналу readyRead со стороны пользователя
    ///
    void runClientAction();
    ///
    /// @brief Этот слот срабатывает, когда пользователь отключается
    ///
    void userLeft();
};

#endif // SERVER_H
