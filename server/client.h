#ifndef CLIENT_H
#define CLIENT_H

#include <QMap>
#include <QTcpSocket>
#include "field.h"

/**
 * @brief Класс, представляющий клиента на сервере
 * 
 * Класс Client хранит информацию о подключенном клиенте:
 * - Сокет для связи с клиентом
 * - Статус клиента
 * - Логин клиента
 * - Игровое поле клиента
 */
class Client
{
public:
    /**
     * @brief Конструктор
     * Инициализирует объект с пустым полем и статусом ST_DISCONNECTED
     */
    Client();
    
    /**
     * @brief Деструктор
     * Освобождает ресурсы
     */
    ~Client();

    /**
     * @brief Тип итератора для карты клиентов
     */
    typedef QMap<int, Client>::iterator ClientIterator;

    /**
     * @brief Перечисление, определяющее состояния клиента
     * 
     * Клиент может находиться в одном из следующих состояний:
     * - ST_DISCONNECTED: Клиент не подключен
     * - ST_CONNECTED: Клиент подключен, но не авторизован
     * - ST_AUTHORIZED: Клиент авторизован
     * - ST_READY: Клиент готов к игре
     * - ST_WAITING_STEP: Клиент ожидает хода
     * - ST_MAKING_STEP: Клиент делает ход
     */
    enum ClientStatus
    {
        ST_DISCONNECTED = 0,  // Клиент не подключен
        ST_CONNECTED,         // Клиент подключен, но не авторизован
        ST_AUTHORIZED,        // Клиент авторизован
        ST_READY,             // Клиент готов к игре
        ST_WAITING_STEP,      // Клиент ожидает хода
        ST_MAKING_STEP,       // Клиент делает ход
        // Дополнительные состояния могут быть добавлены здесь
    };

    /**
     * @brief Установить логин клиента
     * @param login Логин клиента
     */
    void setLogin(const QString& login);
    
    /**
     * @brief Получить логин клиента
     * @return Логин клиента
     */
    QString getLogin();

    /**
     * @brief Инициализировать пустое игровое поле
     */
    void initField();
    
    /**
     * @brief Инициализировать игровое поле из строкового представления
     * @param field Строка, представляющая поле
     */
    void initField(QString field);
    
    /**
     * @brief Получить строковое представление игрового поля
     * @return Строка, представляющая поле
     */
    QString getFieldStr();

public:
    QTcpSocket* socket_;    // Сокет для связи с клиентом
    ClientStatus status_;   // Текущий статус клиента
//    ClientIterator playingWith;
    QString login_;         // Логин клиента

private:
    Field* field_;          // Игровое поле клиента
};

/**
 * @brief Тип для карты клиентов
 * Карта, где ключ - идентификатор клиента, значение - объект Client
 */
typedef QMap<int, Client> Clients;

/**
 * @brief Тип для итератора по карте клиентов
 */
typedef Clients::iterator ClientsIterator;

#endif // CLIENT_H
