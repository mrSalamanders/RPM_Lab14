#include <QCoreApplication>
#include <QObject>
#include <csignal>
#include "server.h"

///
/// \brief Это функция мэйн
/// \param argc
/// \param argv
/// \return Возвращает результат выполнения программы
///
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Server serv;
    return a.exec();
}

