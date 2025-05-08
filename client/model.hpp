#ifndef MODEL_H
#define MODEL_H


#include <QString>
#include <QObject>
#include "config.hpp"
#include "constants.hpp"
#include "field.hpp"

/**
 * @brief Состояния модели игры
 */
enum ModelState
{
    ST_GAME_NSTARTED = 0,   ///< Игра не начата
    ST_PLACING_SHIPS    ,   ///< Расстановка кораблей
    ST_WAITING_PLACING  ,   ///< Ожидание расстановки противником
    ST_WAITING_STEP     ,   ///< Ожидание хода противника
    ST_MAKING_STEP      ,   ///< Выполнение хода
    ST_GAME_FINISHED    ,   ///< Игра завершена
};

/**
 * @brief Класс модели игры
 * 
 * Управляет состоянием игры, включая поля игроков,
 * текущее состояние игры и взаимодействие с контроллером.
 */
class Model: public QObject
{
    Q_OBJECT

public:
    Model();
    ~Model();

    /**
     * @brief Получить текущее состояние игры
     * @return Состояние игры
     */
    ModelState getState() const;

    /**
     * @brief Обновить состояние игры
     * @param new_state Новое состояние
     */
    void updateState(ModelState);

    /**
     * @brief Установить логин игрока
     * @param str Логин
     */
    void setLogin(const QString& str);

    /**
     * @brief Получить логин игрока
     * @return Логин
     */
    QString getLogin() const;

    /**
     * @brief Получить ID текущей игры
     * @return ID игры
     */
    int getGameId() const;

    // Методы для работы с полем игрока
    CellDraw getMyCell(int x, int y) const;
    void setMyDrawCell(int x, int y, CellDraw cell);
    void setMyStateCell(int x, int y, CellState cell);
    CellDraw getEnemyCell(int x, int y) const;
    void setEnemyCell(int x, int y, CellDraw cell);
//    void setMyField(Field field);
    void setMyField(QVector<CellDraw>  field);
    void setMyField(QVector<CellState> field);
    void setMyField(QString field);
    void initMyDrawField();
    QString getMyFieldStr() const;
    Field getMyField() const;
    Field getEnemyField() const;
    bool isMyFieldCorrect() const;
    void clearMyField();

    /**
     * @brief Обновить отображение поля игрока
     * @param field Новое состояние поля
     */
    void updateMyFieldDraw(QVector<CellDraw>& field);

    /**
     * @brief Обновить отображение поля противника
     * @param field Новое состояние поля
     */
    void updateEnemyFieldDraw(QVector<CellDraw>& field);

    /**
     * @brief Передать ход
     */
    void switchStep();

    void setStartedFlag(bool val);
    bool getStartedFlag();
    void setEnemyLogin(QString login);
    QString getEnemyLogin();

    /**
     * @brief Сгенерировать случайную расстановку кораблей
     */
    void generateMyField();

    /**
     * @brief Начать бой
     */
    void startFight();

    /**
     * @brief Начать игру
     * @param enemy_login Логин противника
     * @param gameId ID игры
     */
    void startGame(QString enemy_login, int gameId);

    /**
     * @brief Завершить игру
     */
    void finishGame();

public:
    Field* myField_;      ///< Поле игрока
    Field* enemyField_;   ///< Поле противника

private:
    bool amIStarted_;     ///< Флаг первого хода
    ModelState state_;    ///< Текущее состояние игры
    QString login_;       ///< Логин игрока
    QString enemyLogin_;  ///< Логин противника
    int gameId_;         ///< ID текущей игры
};


#endif // MODEL_H
