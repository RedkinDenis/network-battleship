#ifndef FIELD_H
#define FIELD_H

#include <QVector>
#include <QDebug>
#include <QString>
#include "./config.h"

/**
 * @brief Перечисление, определяющее возможные состояния ячейки игрового поля
 */
enum Cell
{
    CELL_EMPTY = 0  ,   // Пустое поле
    CELL_DOT        ,   // Промах
    CELL_PART       ,   // Поле частично подбитого корабля
    CELL_KILL       ,   // Поле полностью подбитого корабля
    CELL_MARK       ,   // Помеченное пользователем поле
};

/**
 * @brief Класс, представляющий игровое поле для игры "Морской бой"
 * 
 * Класс Field предоставляет функциональность для работы с игровым полем:
 * - Получение и установка состояния ячеек
 * - Преобразование поля в строку и обратно
 * - Очистка поля
 * - Получение размеров поля
 */
class Field
{
public:
    /**
     * @brief Конструктор по умолчанию
     * Создает пустое поле с размерами, определенными в config.h
     */
    Field();
    
    /**
     * @brief Конструктор с параметром
     * @param field Строка, представляющая поле в текстовом формате
     * Создает поле на основе строкового представления
     */
    Field(QString field);
    
    /**
     * @brief Деструктор
     */
    ~Field();

    /**
     * @brief Получить состояние ячейки по координатам
     * @param x Координата X
     * @param y Координата Y
     * @return Состояние ячейки (Cell)
     */
    Cell getCell(int x, int y) const;
    
    /**
     * @brief Установить состояние ячейки по координатам
     * @param x Координата X
     * @param y Координата Y
     * @param cell Новое состояние ячейки
     */
    void setCell(int x, int y, Cell cell);
    
    /**
     * @brief Получить строковое представление поля
     * @return Строка, представляющая поле
     */
    QString getFieldStr();
    
    /**
     * @brief Установить поле из строкового представления
     * @param field Строка, представляющая поле
     */
    void setField(QString field);
    
    /**
     * @brief Установить поле из вектора состояний ячеек
     * @param field Вектор состояний ячеек
     */
    void setField(QVector<Cell> field);
    
    /**
     * @brief Очистить поле (установить все ячейки в CELL_EMPTY)
     */
    void clear();

    /**
     * @brief Получить ширину поля
     * @return Ширина поля
     */
    int getWidth() const;
    
    /**
     * @brief Получить высоту поля
     * @return Высота поля
     */
    int getHeight() const;

private:
    int width_;      // Ширина поля
    int height_;     // Высота поля
    int area_;       // Площадь поля (width_ * height_)
    QVector<Cell> field_; // Вектор, хранящий состояния всех ячеек поля
};

#endif // FIELD_H
