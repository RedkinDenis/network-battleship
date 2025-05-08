/**
 * @file server.hpp
 * @brief Основной класс сервера для сетевой игры "Морской бой"
 * 
 * Этот класс реализует TCP-сервер, который обрабатывает подключения клиентов,
 * управляет игровыми сессиями и обеспечивает обмен данными между игроками.
 */

#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
//#include <QtSerialPort/QSerialPort>
#include <vector>
#include <QTextBrowser>
#include <QLabel>
#include "client.hpp"
#include "gamecontroller.hpp"
#include "dbcontroller.hpp"
#include <QDateTime>

/**
 * @brief Класс сервера
 * 
 * Наследуется от QTcpServer и реализует всю логику серверной части игры.
 * Управляет подключениями клиентов, игровыми сессиями и обменом данными.
 */
class Server: public QTcpServer
{
    Q_OBJECT

public:
    /**
     * @brief Состояния сервера
     */
    enum ServerState
    {
        ST_NSTARTED = 0,  ///< Сервер не запущен
        ST_STARTED     ,  ///< Сервер запущен и работает
        ST_STOPPED     ,  ///< Сервер остановлен
    };

public:
    /**
     * @brief Конструктор
     * @param port Порт для прослушивания подключений
     */
    Server(quint16 port);
    
    /**
     * @brief Конструктор копирования
     * @param other Объект для копирования
     */
    Server(const Server& other);
    
    /**
     * @brief Оператор присваивания
     * @param other Объект для копирования
     * @return Ссылка на текущий объект
     */
    Server& operator=(const Server& other);
    
    /**
     * @brief Деструктор
     */
    ~Server();

    QTextBrowser* browser;  ///< Виджет для отображения логов сервера

    /**
     * @brief Обновить состояние сервера
     * @param state Новое состояние
     */
    void updateState(ServerState state);
    
    /**
     * @brief Получить текущее состояние сервера
     * @return Текущее состояние
     */
    ServerState getServerState();

    /**
     * @brief Проверить логин пользователя
     * @param login Проверяемый логин
     * @return true если логин валидный
     */
    bool checkLogin(QString& login);
    
    /**
     * @brief Проверить авторизацию пользователя
     * @param login Логин пользователя
     * @return true если пользователь авторизован
     */
    bool is_logined(QString& login);
    
    /**
     * @brief Найти клиента по логину
     * @param login Логин пользователя
     * @return Итератор на клиента
     */
    ClientsIterator findClient(QString& login);
    
    /**
     * @brief Обработать данные от клиента
     * @param data Полученные данные
     * @param clientId ID клиента
     */
    void handleData(const QByteArray& data, int clientId);
    
    /**
     * @brief Обработать отключение клиента
     * @param cit Итератор на отключающегося клиента
     */
    void clientDisconnect(ClientsIterator& cit);
    
    /**
     * @brief Обработать запрос списка пользователей
     */
    void handleUsersRequest();
    
    /**
     * @brief Обработать запрос обновления состояния
     */
    void handleUpdateRequest();
    
    /**
     * @brief Обработать запрос готовности к игре
     */
    void handleReadinessRequest();
    
    /**
     * @brief Обработать запрос на подключение
     */
    void handleConnectionRequest();
    
    /**
     * @brief Обработать запрос на выход
     */
    void handleExitRequest();
    
    /**
     * @brief Обработать запрос состояния поля
     */
    void handleFieldRequest();
    
    /**
     * @brief Отправить сообщение всем клиентам
     * @param message Текст сообщения
     */
    void sendMessageToAll(const QString& message);
    
    /**
     * @brief Удалить отключенных клиентов
     */
    void removeDisconnectedClients();
    
    /**
     * @brief Отметить уничтоженный корабль
     * @param cIt Итератор на клиента
     * @param x Координата X
     * @param y Координата Y
     */
    void drawKilledShip(ClientsIterator cIt, int x, int y);
    
    /**
     * @brief Отправить состояние поля пользователям
     * @param cIt Итератор на клиента
     */
    void sendFieldDrawToUsers(ClientsIterator cIt);
    
    /**
     * @brief Отправить историю игр пользователям
     * @param gamesHistoryList Список игр
     */
    void sendGamesHistoryListToUsers(QStringList& gamesHistoryList);

    /**
     * @brief Начать игру между двумя игроками
     * @param login1 Логин первого игрока
     * @param login2 Логин второго игрока
     */
    void startGame(QString login1, QString login2);
    
    /**
     * @brief Завершить игру
     * @param gameId ID игры
     */
    void finishGame(int gameId);

    /**
     * @brief Тестирование базы данных
     */
    void testDB();

private:
    quint16 port_;                    ///< Порт сервера
    QTcpSocket* socket_;              ///< Сокет для подключений
    QByteArray data_;                 ///< Буфер для данных
    Clients clients_;                 ///< Список подключенных клиентов
    QMap<quintptr, QString> logins_;  ///< Маппинг сокетов к логинам
    ServerState state_;               ///< Текущее состояние сервера
    int timerId_;                     ///< ID таймера
    Games games_;                     ///< Активные игры
    DBController dbController_;       ///< Контроллер базы данных

protected:
    /**
     * @brief Обработчик таймера
     * @param event Событие таймера
     */
    void timerEvent(QTimerEvent* event);

public slots:
    /**
     * @brief Запустить сервер
     * @param textBrowser Виджет для логов
     */
    void startServer(QTextBrowser* textBrowser);
    
    /**
     * @brief Остановить сервер
     */
    void stopServer();
    
    /**
     * @brief Обработчик нового подключения
     * @param socketDescriptor Дескриптор сокета
     */
    void incomingConnection(qintptr socketDescriptor);
    
    /**
     * @brief Обработчик установки соединения
     */
    void on_sockConnect();
    
    /**
     * @brief Обработчик получения данных
     */
    void on_receiveData();
    
    /**
     * @brief Обработчик отключения
     */
    void on_sockDisconnect();
    
    /**
     * @brief Обработчик ошибок сокета
     * @param error Код ошибки
     */
    void on_sockError(QAbstractSocket::SocketError error);
};

#endif // SERVER_H
