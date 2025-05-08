#ifndef FIELD_H
#define FIELD_H

#include <QVector>
#include <QDebug>
#include <QString>
#include "config.hpp"
#include "constants.hpp"

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
 * @brief Состояния клетки для внутреннего представления
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
 * @brief Класс игрового поля
 * 
 * Управляет состоянием игрового поля, включая расстановку кораблей,
 * обработку выстрелов и проверку правил игры.
 */
class Field
{
public:
    /**
     * @brief Владелец поля
     */
    enum Owner
    {
        MY_FIELD    = 0,    ///< Поле игрока
        ENEMY_FIELD = 1,    ///< Поле противника
    };

public:
    Field();
    Field(QString field);
    Field& operator=(const Field& other);

    ~Field();

    /**
     * @brief Получить состояние клетки для отрисовки
     * @param x Координата X
     * @param y Координата Y
     * @return Состояние клетки
     */
    CellDraw getCell(int x, int y) const;

    /**
     * @brief Установить состояние клетки для отрисовки
     * @param x Координата X
     * @param y Координата Y
     * @param cell Новое состояние
     */
    void setDrawCell(int x, int y, CellDraw cell);

    /**
     * @brief Установить внутреннее состояние клетки
     * @param x Координата X
     * @param y Координата Y
     * @param cell Новое состояние
     */
    void setStateCell(int x, int y, CellState cell);
    void setStateCell(int index_bordered, CellState cell);

    /**
     * @brief Получить строковое представление внутреннего состояния поля
     * @return Строка с состоянием поля
     */
    QString getStateFieldStr() const;

    /**
     * @brief Получить строковое представление состояния отрисовки поля
     * @return Строка с состоянием отрисовки
     */
    QString getDrawFieldStr() const;
    QVector<CellDraw> getDrawField() const;

    void setStateField(QString field);
    void setDrawField(QString field);
    void setDrawField(QVector<CellDraw> field);
    void setStateField(QVector<CellState> field);

    /**
     * @brief Инициализировать поле для отрисовки
     */
    void initMyDrawField();

    /**
     * @brief Очистить поле
     */
    void clear();

    /**
     * @brief Получить изображение поля
     * @return QImage с отрисованным полем
     */
    QImage getFieldImage();

    /**
     * @brief Сгенерировать случайную расстановку кораблей
     */
    void generate();

    int getWidth() const;
    int getHeight() const;
    int getArea() const;

    /**
     * @brief Проверить корректность расстановки кораблей
     * @return true если расстановка корректна
     */
    bool isCorrect();
    bool CheckDiagonalCollisions(const QVector<CellState>& fieldStateWithBorders);
    bool CheckLength(QVector<CellState>& fieldState);

private:
    /**
     * @brief Подсчитать количество кораблей заданного размера
     * @param size Размер корабля
     * @param fieldStateWithBorders Состояние поля с границами
     * @return Количество кораблей
     */
    int shipNum(int size, const QVector<CellState>& fieldStateWithBorders);

    /**
     * @brief Проверить, является ли клетка началом корабля заданного размера
     * @param size Размер корабля
     * @param x Координата X
     * @param y Координата Y
     * @param fieldStateWithBorders Состояние поля с границами
     * @return true если клетка является началом корабля
     */
    bool isShip(int size, int x, int y, const QVector<CellState>& fieldStateWithBorders);

    /**
     * @brief Проверить возможность размещения корабля
     * @param x Координата X начала корабля
     * @param y Координата Y начала корабля
     * @param size Размер корабля
     * @param horizontal true если корабль горизонтальный
     * @return true если корабль можно разместить
     */
    bool canPlaceShip(int x, int y, int size, bool horizontal);

    /**
     * @brief Разместить корабль на поле
     * @param x Координата X начала корабля
     * @param y Координата Y начала корабля
     * @param size Размер корабля
     * @param horizontal true если корабль горизонтальный
     */
    void placeShip(int x, int y, int size, bool horizontal);

private:
    int width_;          ///< Ширина поля
    int height_;         ///< Высота поля
    int area_;           ///< Площадь поля
    QVector<CellState> fieldState_;  ///< Внутреннее состояние поля
    QVector<CellDraw> fieldDraw_;    ///< Состояние поля для отрисовки
};

/**
 * @brief Преобразовать строку в вектор состояний отрисовки
 * @param fieldDrawStr Строка с состояниями
 * @return Вектор состояний
 */
QVector<CellDraw> fieldDrawFromStr(QString fieldDrawStr);

#endif // FIELD_H
