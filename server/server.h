#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
//#include <QtSerialPort/QSerialPort>
#include <vector>
#include <QTextBrowser>
#include "client.h"

/**
 * @brief Класс, представляющий сервер игры
 * 
 * Класс Server является центральным компонентом серверной части игры. Он:
 * - Управляет подключением клиентов
 * - Обрабатывает авторизацию пользователей
 * - Передает сообщения между клиентами
 * - Отслеживает состояние игры
 */
class Server: public QTcpServer
{
    Q_OBJECT

public:
    /**
     * @brief Перечисление, определяющее состояния сервера
     */
    enum ServerState
    {
        ST_NSTARTED = 0,  // Сервер не запущен
        ST_STARTED,       // Сервер запущен
        ST_STOPPED,       // Сервер остановлен
    };

public:
    /**
     * @brief Конструктор
     * @param port Порт, на котором будет работать сервер
     */
    Server(quint16 port);
    
    /**
     * @brief Конструктор копирования
     * @param other Другой объект Server
     */
    Server(const Server& other);
    
    /**
     * @brief Оператор присваивания
     * @param other Другой объект Server
     * @return Ссылка на текущий объект
     */
    Server& operator=(const Server& other);
    
    /**
     * @brief Деструктор
     */
    ~Server();

    QTextBrowser* browser;  // Текстовый браузер для вывода логов

    /**
     * @brief Обновить состояние сервера
     * @param state Новое состояние сервера
     */
    void updateState(ServerState state);
    
    /**
     * @brief Получить текущее состояние сервера
     * @return Текущее состояние сервера
     */
    ServerState getServerState();

    /**
     * @brief Проверить логин на уникальность
     * @param login Логин для проверки
     * @return true, если логин уникален, иначе false
     */
    bool checkLogin(QString& login);
    
    /**
     * @brief Проверить, авторизован ли пользователь с указанным логином
     * @param login Логин для проверки
     * @return true, если пользователь авторизован, иначе false
     */
    bool is_logined(QString& login);
    
    /**
     * @brief Обработать данные, полученные от клиента
     * @param data Данные, полученные от клиента
     * @param clientId Идентификатор клиента
     */
    void handleData(const QByteArray& data, int clientId);
    
    /**
     * @brief Обработать отключение клиента
     * @param cit Итератор клиента в списке клиентов
     */
    void clientDisconnect(ClientsIterator& cit);
    
    /**
     * @brief Обработать запрос списка пользователей
     * Отправляет всем клиентам актуальный список пользователей
     */
    void handleUsersRequest();
    
    /**
     * @brief Обработать запрос выхода
     * Обрабатывает сообщение о выходе пользователя
     */
    void handleExitRequest();
    
    /**
     * @brief Отправить сообщение всем клиентам
     * @param message Сообщение для отправки
     */
    void sendMessageToAll(const QString& message);
    
    /**
     * @brief Удалить отключенных клиентов
     * Удаляет из списка клиентов тех, кто отключился
     */
    void removeDisconnectedClients();

private:
    quint16 port_;         // Порт сервера
    QTcpSocket* socket_;   // Сокет сервера
    QByteArray data_;      // Данные, полученные от сокета и для отправки в сокет
    Clients clients_;      // Список клиентов
    QMap<quintptr, QString> logins_;  // Карта соответствия идентификаторов сокетов и логинов
    ServerState state_;    // Текущее состояние сервера
    int timerId_;          // Идентификатор таймера для периодических действий

protected:
    /**
     * @brief Обработчик события таймера
     * @param event Событие таймера
     */
    void timerEvent(QTimerEvent* event);


public slots:
    /**
     * @brief Запустить сервер
     * @param textBrowser Текстовый браузер для вывода логов
     */
    void startServer(QTextBrowser* textBrowser);
    
    /**
     * @brief Остановить сервер
     */
    void stopServer();
    
    /**
     * @brief Обработчик входящего подключения
     * @param socketDescriptor Дескриптор сокета нового клиента
     */
    void incomingConnection(qintptr socketDescriptor);
    
    /**
     * @brief Обработчик события подключения к сокету
     */
    void on_sockConnect();
    
    /**
     * @brief Обработчик события получения данных от сокета
     */
    void on_receiveData();
    
    /**
     * @brief Обработчик события отключения от сокета
     */
    void on_sockDisconnect();
    
    /**
     * @brief Обработчик события ошибки сокета
     * @param error Код ошибки
     */
    void on_sockError(QAbstractSocket::SocketError error);
};

#endif // SERVER_H
