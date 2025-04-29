#ifndef MODEL_H
#define MODEL_H


#include <QString>
#include <QObject>
#include "field.h"

/**
 * @brief Перечисление, определяющее состояния модели игры
 * 
 * Модель может находиться в одном из следующих состояний:
 * - ST_PLACING_SHIPS: Игрок расставляет корабли
 * - ST_WAITING_STEP: Игрок ожидает хода противника
 * - ST_MAKING_STEP: Игрок делает ход
 */
enum ModelState
{
    ST_PLACING_SHIPS,  // Игрок расставляет корабли
    ST_WAITING_STEP,   // Игрок ожидает хода противника
    ST_MAKING_STEP     // Игрок делает ход
};

/**
 * @brief Класс, представляющий модель игры
 * 
 * Класс Model является центральным компонентом игры, который:
 * - Хранит состояние игры
 * - Управляет игровым полем
 * - Проверяет корректность расстановки кораблей
 * - Обрабатывает ходы игрока
 */
class Model: public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор
     * Инициализирует модель с пустым полем и состоянием ST_PLACING_SHIPS
     */
    Model();
    
    /**
     * @brief Деструктор
     * Освобождает ресурсы
     */
    ~Model();

    /**
     * @brief Получить текущее состояние модели
     * @return Текущее состояние модели
     */
    ModelState getState() const;
    
    /**
     * @brief Установить состояние модели
     * @param state Новое состояние модели
     */
    void setState(ModelState);
    
    /**
     * @brief Установить логин игрока
     * @param str Логин игрока
     */
    void setLogin(const QString& str);
    
    /**
     * @brief Получить логин игрока
     * @return Логин игрока
     */
    QString getLogin() const;

    /**
     * @brief Получить состояние ячейки на поле игрока
     * @param x Координата X
     * @param y Координата Y
     * @return Состояние ячейки
     */
    Cell getMyCell(int x, int y) const;
    
    /**
     * @brief Установить состояние ячейки на поле игрока
     * @param x Координата X
     * @param y Координата Y
     * @param cell Новое состояние ячейки
     */
    void setMyCell(int x, int y, Cell cell);
    
    /**
     * @brief Установить поле игрока из вектора состояний ячеек
     * @param field Вектор состояний ячеек
     */
    void setMyField(QVector<Cell> field);
    
    /**
     * @brief Установить поле игрока из строкового представления
     * @param field Строка, представляющая поле
     */
    void setMyField(QString field);
    
    /**
     * @brief Получить строковое представление поля игрока
     * @return Строка, представляющая поле
     */
    QString getMyFieldStr() const;
    
    /**
     * @brief Получить объект поля игрока
     * @return Объект поля игрока
     */
    Field getMyField() const;
    
    /**
     * @brief Проверить корректность расстановки кораблей на поле игрока
     * @return true, если расстановка корректна, иначе false
     */
    bool checkMyField() const;
    
    /**
     * @brief Очистить поле игрока
     */
    void clearMyField();

private:
    /**
     * @brief Подсчитать количество кораблей заданного размера на поле
     * @param size Размер корабля
     * @return Количество кораблей заданного размера
     */
    int shipNum(int size) const;
    
    /**
     * @brief Проверить, есть ли корабль заданного размера в указанной позиции
     * @param size Размер корабля
     * @param x Координата X
     * @param y Координата Y
     * @return true, если корабль найден, иначе false
     */
    bool isShip(int size, int x, int y) const;

private:
    Field* myField_;      // Поле игрока
    ModelState state_;    // Текущее состояние модели
    QString login_;       // Логин игрока
};


#endif // MODEL_H
