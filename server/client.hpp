/**
 * @file client.hpp
 * @brief Класс клиента для серверной части игры "Морской бой"
 * 
 * Этот класс представляет клиента на стороне сервера и хранит всю информацию,
 * связанную с подключенным игроком, включая его состояние, поле и сокет.
 */

#ifndef CLIENT_H
#define CLIENT_H

#include <QMap>
#include <QTcpSocket>
#include "field.hpp"

/**
 * @brief Класс клиента
 * 
 * Хранит информацию о подключенном игроке, его состоянии,
 * игровом поле и сокете для коммуникации.
 */
class Client
{
public:
    /**
     * @brief Конструктор
     */
    Client();
    
    /**
     * @brief Деструктор
     */
    ~Client();

    typedef QMap<int, Client>::iterator ClientIterator;  ///< Тип итератора для списка клиентов

    /**
     * @brief Состояния клиента
     */
    enum ClientStatus
    {
        ST_DISCONNECTED = 0,  ///< Клиент отключен
        ST_CONNECTED       ,  ///< Клиент подключен
        ST_AUTHORIZED      ,  ///< Клиент авторизован
        // more
    };

    /**
     * @brief Состояния готовности к игре
     */
    enum Readiness
    {
        ST_NREADY       = 0,  ///< Не готов к игре
        ST_READY           ,  ///< Готов к игре
        ST_PLAYING         ,  ///< В процессе игры
    };

    /**
     * @brief Установить логин клиента
     * @param login Логин пользователя
     */
    void setLogin(const QString& login);
    
    /**
     * @brief Установить отображение поля
     * @param field Вектор состояний клеток для отображения
     */
    void setFieldDraw(QVector<Field::CellDraw> field);
    
    /**
     * @brief Получить логин клиента
     * @return Логин пользователя
     */
    QString getLogin();
    
    /**
     * @brief Получить игровое поле
     * @return Объект игрового поля
     */
    Field getField();
    
    /**
     * @brief Проверить авторизацию клиента
     * @return true если клиент авторизован
     */
    bool isAuthorized() const;
    
    /**
     * @brief Обновить состояние клиента
     * @param state Новое состояние
     */
    void updateState(ClientStatus state);

    /**
     * @brief Инициализировать пустое поле
     */
    void initField();
    
    /**
     * @brief Инициализировать поле из строки
     * @param field Строка с состоянием поля
     */
    void initField(QString field);
    
    /**
     * @brief Инициализировать поле из строк с состоянием и отображением
     * @param field Строка с состоянием поля
     * @param fieldState Строка с отображением поля
     */
    void initField(QString field, QString fieldState);
    
    /**
     * @brief Получить строковое представление поля
     * @return Строка с состоянием поля
     */
    QString getFieldStr();
    
    /**
     * @brief Проверить пустоту клетки
     * @param x Координата X
     * @param y Координата Y
     * @return true если клетка пуста
     */
    bool isCellEmpty(int x, int y);
    
    /**
     * @brief Проверить уничтожение корабля
     * @param x Координата X
     * @param y Координата Y
     * @return true если корабль уничтожен
     */
    bool isKilled(int x, int y);
    
    /**
     * @brief Установить состояние клетки
     * @param x Координата X
     * @param y Координата Y
     * @param state Новое состояние
     */
    void setCellState(int x, int y, Field::CellState state);
    
    /**
     * @brief Установить отображение клетки
     * @param x Координата X
     * @param y Координата Y
     * @param state Новое отображение
     */
    void setCellDraw(int x, int y, Field::CellDraw state);

public:
    QTcpSocket*  socket_;     ///< Сокет для коммуникации с клиентом
    ClientStatus status_;     ///< Текущее состояние клиента
    Readiness readiness_;     ///< Готовность к игре
    ClientIterator enemy_;    ///< Итератор на противника
    QString login_;          ///< Логин пользователя

private:
    Field* field_;           ///< Игровое поле клиента
};

typedef QMap<int, Client> Clients;           ///< Тип для хранения списка клиентов
typedef Clients::iterator ClientsIterator;    ///< Тип итератора для списка клиентов

#endif // CLIENT_H
