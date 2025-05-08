/**
 * @file field.hpp
 * @brief Класс игрового поля для серверной части игры "Морской бой"
 * 
 * Этот класс реализует игровое поле, управляет размещением кораблей,
 * их состоянием и отображением.
 */

#ifndef FIELD_H
#define FIELD_H

#include <QVector>
#include <QDebug>
#include <QString>
#include "./config.hpp"

/**
 * @brief Состояния клетки поля
 */
enum Cell
{
    CELL_EMPTY = 0  ,   ///< Пустая клетка
    CELL_SHIP       ,   ///< Клетка с кораблем
};

/**
 * @brief Класс игрового поля
 * 
 * Реализует игровое поле, управляет размещением кораблей,
 * их состоянием и отображением.
 */
class Field
{
public:
    /**
     * @brief Конструктор по умолчанию
     */
    Field();
    
    /**
     * @brief Конструктор с инициализацией поля
     * @param field Строка с состоянием поля
     */
    Field(QString field);
    
    /**
     * @brief Конструктор с инициализацией поля и его состояния
     * @param field Строка с состоянием поля
     * @param fieldState Строка с состоянием клеток
     */
    Field(QString field, QString fieldState);
    
    /**
     * @brief Деструктор
     */
    ~Field();

    /**
     * @brief Состояния клетки
     */
    enum CellState
    {
        CL_ST_EMPTY     = 0,    ///< Пустая клетка
        CL_ST_CENTER    = 1,    ///< Центральная клетка (единичный корабль)
        CL_ST_TOP       = 2,    ///< Верхняя клетка корабля (вертикального)
        CL_ST_BOTTOM    = 3,    ///< Нижняя клетка корабля (вертикального)
        CL_ST_VMIDDLE   = 4,    ///< Серединная клетка вертикально ориентированного корабля
        CL_ST_HMIDDLE   = 5,    ///< Серединная клетка горизонтально ориентированного корабля
        CL_ST_LEFT      = 6,    ///< Левая клетка корабля (горизонтального)
        CL_ST_RIGHT     = 7,    ///< Правая клетка корабля (горизонтального)
        CL_ST_UNDEFINED = 8,    ///< Неопределённое непустое состояние клетки
    };

    /**
     * @brief Состояния клетки для отрисовки
     */
    enum CellDraw
    {
        CELL_EMPTY = 0  ,   ///< Пустое поле
        CELL_LIVE       ,   ///< Часть живого корабля (ещё не попали)
        CELL_DOT        ,   ///< Выстрел - промах
        CELL_DAMAGED    ,   ///< Подбитое поле частично подбитого корабля
        CELL_KILLED     ,   ///< Поле полностью подбитого корабля
        CELL_MARK       ,   ///< Помеченное пользователем поле (как флажок в сапёре)
    };

    /**
     * @brief Получить состояние клетки
     * @param x X-координата
     * @param y Y-координата
     * @return Состояние клетки
     */
    Cell getCell(int x, int y);
    
    /**
     * @brief Получить поле
     * @return Вектор состояний клеток
     */
    QVector<Cell> getField();
    
    /**
     * @brief Получить ширину поля
     * @return Ширина поля
     */
    int getWidth();
    
    /**
     * @brief Получить высоту поля
     * @return Высота поля
     */
    int getHeight();
    
    /**
     * @brief Получить строковое представление поля
     * @return Строка с состоянием поля
     */
    QString getFieldStr();
    
    /**
     * @brief Получить строковое представление состояния поля
     * @return Строка с состоянием клеток
     */
    QString getFieldStateStr();
    
    /**
     * @brief Получить строковое представление отображения поля
     * @return Строка с состоянием отображения
     */
    QString getFieldDrawStr();
    
    /**
     * @brief Получить состояние поля
     * @return Вектор состояний клеток
     */
    QVector<CellState> getFieldState();
    
    /**
     * @brief Получить отображение поля
     * @return Вектор состояний отображения
     */
    QVector<CellDraw> getFieldDraw();

    /**
     * @brief Установить состояние клетки
     * @param x X-координата
     * @param y Y-координата
     * @param cell Состояние клетки
     */
    void setCell(int x, int y, Cell cell);
    
    /**
     * @brief Установить поле
     * @param field Строка с состоянием поля
     */
    void setField(QString field);
    
    /**
     * @brief Установить состояние клетки
     * @param x X-координата
     * @param y Y-координата
     * @param state Состояние клетки
     */
    void setCellState(int x, int y, CellState state);
    
    /**
     * @brief Установить отображение клетки
     * @param x X-координата
     * @param y Y-координата
     * @param state Состояние отображения
     */
    void setCellDraw(int x, int y, CellDraw state);
    
    /**
     * @brief Установить состояние поля
     * @param field Строка с состоянием клеток
     */
    void setFieldState(QString field);
    
    /**
     * @brief Установить отображение поля
     * @param field Вектор состояний отображения
     */
    void setFieldDraw(QVector<Field::CellDraw> field);

    /**
     * @brief Очистить поле
     */
    void clear();
    
    /**
     * @brief Сгенерировать случайное поле
     */
    void generate();
    
    /**
     * @brief Инициализировать отображение поля
     */
    void initFieldDraw();
    
    /**
     * @brief Инициализировать состояние поля
     */
    void initFieldState();

    /**
     * @brief Проверить, пуста ли клетка
     * @param x X-координата
     * @param y Y-координата
     * @return true, если клетка пуста
     */
    bool isCellEmpty(int x, int y);
    
    /**
     * @brief Проверить корректность поля
     * @return true, если поле корректно
     */
    bool isCorrect() const;
    
    /**
     * @brief Проверить, убит ли корабль
     * @param x X-координата
     * @param y Y-координата
     * @return true, если корабль убит
     */
    bool isKilled(int x, int y);

private:
    int width_;                    ///< Ширина поля
    int height_;                   ///< Высота поля
    int area_;                     ///< Площадь поля
    QVector<Cell> field_;          ///< Поле
    QVector<CellState> fieldState_; ///< Состояние поля
    QVector<CellDraw> fieldDraw_;   ///< Отображение поля
};

#endif // FIELD_H
