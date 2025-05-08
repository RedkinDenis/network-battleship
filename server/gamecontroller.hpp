/**
 * @file gamecontroller.hpp
 * @brief Контроллер игры для серверной части "Морской бой"
 * 
 * Этот класс управляет игровым процессом между двумя игроками,
 * контролирует ходы, проверяет условия победы и хранит состояние игры.
 */

#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "config.hpp"
#include "client.hpp"
#include <QTimer>
#include <QDateTime>

/**
 * @brief Класс контроллера игры
 * 
 * Управляет игровым процессом между двумя игроками,
 * контролирует ходы и проверяет условия победы.
 */
class GameController
{
public:
    /**
     * @brief Состояния игры
     */
    enum GameState
    {
        ST_NSTARTED =  0,   ///< Игра не начата
        ST_PLACING      ,   ///< Пользователи располагают корабли
        ST_STARTED_STEP ,   ///< Ход пользователя, начавшего игру
        ST_ACCEPTED_STEP,   ///< Ход пользователя, принявшего приглашение
        ST_FINISHED     ,   ///< Игра завершена
    };

    /**
     * @brief Конструктор
     * @param gameId ID игры
     * @param clientStarted Итератор на клиента, начавшего игру
     * @param clientAccepted Итератор на клиента, принявшего игру
     */
    GameController(int gameId, ClientsIterator clientStarted, ClientsIterator clientAccepted);
    
    /**
     * @brief Деструктор
     */
    ~GameController();

    /**
     * @brief Получить итератор на клиента, начавшего игру
     * @return Итератор на клиента
     */
    ClientsIterator getClientStartedIt();
    
    /**
     * @brief Получить итератор на клиента, принявшего игру
     * @return Итератор на клиента
     */
    ClientsIterator getClientAcceptedIt();
    
    /**
     * @brief Получить ID игры
     * @return ID игры
     */
    int getGameId();
    
    /**
     * @brief Получить текущее состояние игры
     * @return Состояние игры
     */
    GameState getState();
    
    /**
     * @brief Обновить состояние игры
     * @param state Новое состояние
     */
    void updateState(GameController::GameState state);

    /**
     * @brief Увеличить счетчик размещенных кораблей
     */
    void incNPlaced();
    
    /**
     * @brief Получить количество размещенных кораблей
     * @return Количество кораблей
     */
    int getNPlaced();
    
    /**
     * @brief Увеличить счетчик поврежденных клеток
     * @param isStartedDamaged true если поврежден корабль инициатора игры
     */
    void incNDamaged(bool isStartedDamaged);
    
    /**
     * @brief Проверить завершение игры
     * @param isStartedKilled true если уничтожен корабль инициатора игры
     * @return true если игра завершена
     */
    bool checkGameFinish(bool isStartedKilled);

    /**
     * @brief Установить поле клиента, начавшего игру
     * @param field Строка с состоянием поля
     */
    void setClientStartedField(QString field);
    
    /**
     * @brief Установить поле клиента, принявшего игру
     * @param field Строка с состоянием поля
     */
    void setClientAcceptedField(QString field);
    
    /**
     * @brief Установить состояние поля клиента, начавшего игру
     * @param field Строка с состоянием поля
     */
    void setClientStartedFieldState(QString field);
    
    /**
     * @brief Установить состояние поля клиента, принявшего игру
     * @param field Строка с состоянием поля
     */
    void setClientAcceptedFieldState(QString field);

public:
    QString winnerLogin_;    ///< Логин победителя

    QDate startDate_;        ///< Дата начала игры
    QDateTime startTime_;    ///< Время начала игры

    QDate endDate_;          ///< Дата окончания игры
    QDateTime endTime_;      ///< Время окончания игры

private:
    int gameId_;             ///< ID игры
    GameState state_;        ///< Текущее состояние игры
    int nPlaced_;            ///< Количество размещенных кораблей
    int nAcceptedDamaged_;   ///< Количество поврежденных клеток принявшего игру
    int nStartedDamaged_;    ///< Количество поврежденных клеток начавшего игру
    int nDecks_;             ///< Общее количество палуб

    ClientsIterator clientStarted_;     ///< Итератор на клиента, начавшего игру
    ClientsIterator clientAccepted_;    ///< Итератор на клиента, принявшего игру
    Field clientStartedField_;          ///< Поле клиента, начавшего игру
    Field clientAcceptedField_;         ///< Поле клиента, принявшего игру
};

typedef QMap<int, GameController> Games;           ///< Тип для хранения активных игр
typedef Games::iterator GamesIterator;             ///< Тип итератора для списка игр

#endif // GAMECONTROLLER_H
