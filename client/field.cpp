/**
 * @file field.cpp
 * @brief Реализация класса Field для игры "Морской бой".
 * 
 * Этот файл содержит реализацию класса Field, который управляет состоянием игрового поля,
 * размещением кораблей и обработкой выстрелов. Класс является основным компонентом
 * для представления игрового поля и его логики.
 */

#include <QPixmap>
#include <QPainter>
#include "field.hpp"
#include "images.hpp"
#include "qmath.h"
#include <QDebug>

/**
 * @brief Конструктор по умолчанию.
 * 
 * Инициализирует поле стандартного размера и очищает его.
 */
Field::Field() :
    width_(FIELD_WIDTH_DEFAULT),
    height_(FIELD_HEIGHT_DEFAULT),
    area_(FIELD_WIDTH_DEFAULT*FIELD_HEIGHT_DEFAULT)
{
    clear();
}

/**
 * @brief Конструктор с параметром строки.
 * @param field Строка, представляющая начальное состояние поля
 * 
 * Инициализирует поле стандартного размера и устанавливает его состояние из строки.
 */
Field::Field(QString field) :
    width_(FIELD_WIDTH_DEFAULT),
    height_(FIELD_HEIGHT_DEFAULT),
    area_(FIELD_WIDTH_DEFAULT*FIELD_HEIGHT_DEFAULT)
{
    qDebug() << "Field(str) constructor: " << field;
    setStateField(field);
}

/**
 * @brief Оператор присваивания.
 * @param other Поле, из которого копируются данные
 * @return Ссылка на текущий объект
 * 
 * Копирует все данные из другого поля в текущее.
 */
Field& Field::operator=(const Field& other)
{
    if (this == &other)
        return *this;

    width_      = other.width_      ;
    height_     = other.height_     ;
    area_       = other.area_       ;
    fieldState_ = other.fieldState_ ;
    fieldDraw_  = other.fieldDraw_  ;

    return *this;
}

/**
 * @brief Деструктор.
 * 
 * Очищает все векторы данных поля.
 */
Field::~Field()
{
    fieldState_.clear();
    fieldDraw_.clear();
}

/**
 * @brief Получение состояния ячейки для отображения.
 * @param x Координата X
 * @param y Координата Y
 * @return Состояние ячейки для отображения
 * 
 * Возвращает состояние ячейки для отображения по заданным координатам.
 * Если координаты некорректны, возвращает CELL_EMPTY.
 */
CellDraw Field::getCell(int x, int y) const
{
    if(x >= 0 && y >= 0 && x < width_ && y < height_)
    {
        return fieldDraw_[width_*y+x];
    }

    qDebug() << "Wrong cell indexes";
    return CELL_EMPTY;
}

/**
 * @brief Установка состояния ячейки для отображения.
 * @param x Координата X
 * @param y Координата Y
 * @param cell Новое состояние ячейки
 * 
 * Устанавливает состояние ячейки для отображения по заданным координатам.
 * Если координаты некорректны, выводит сообщение об ошибке.
 */
void Field::setDrawCell(int x, int y, CellDraw cell)
{
    if(x >= 0 && y >= 0 && x < width_ && y < height_)
    {
        fieldDraw_[width_*y+x] = cell;
        return;
    }

    qDebug() << "ERROR: no such cell (" << x << "," << y << ")";
}

/**
 * @brief Установка внутреннего состояния ячейки.
 * @param x Координата X
 * @param y Координата Y
 * @param cell Новое внутреннее состояние ячейки
 * 
 * Устанавливает внутреннее состояние ячейки по заданным координатам.
 * Если координаты некорректны, выводит сообщение об ошибке.
 */
void Field::setStateCell(int x, int y, CellState cell)
{
    if(x >= 0 && y >= 0 && x < width_ && y < height_)
    {
        fieldState_[width_*y+x] = cell;
        return;
    }

    qDebug() << "ERROR: no such cell (" << x << "," << y << ")";
}

void Field::setStateCell(int index_bordered, CellState cell)
{
    if(index_bordered < (width_ + 2) * (height_ + 2))
    {
        return;
    }
}

QString Field::getStateFieldStr() const
{
    QString result = "";

    for(QVector<CellState>::const_iterator cell_it = fieldState_.begin(); cell_it != fieldState_.end(); ++cell_it)
    {
        result += QString::number(*cell_it);
    }

    return result;
}

QString Field::getDrawFieldStr() const
{
    QString result = "";

    for(QVector<CellDraw>::const_iterator cell_it = fieldDraw_.begin(); cell_it != fieldDraw_.end(); ++cell_it)
    {
        result += QString::number(*cell_it);
    }

    return result;
}

QVector<CellDraw> Field::getDrawField() const
{
    return fieldDraw_;
}

QVector<CellDraw> fieldDrawFromStr(QString fieldDrawStr)
{
    QVector<CellDraw> fieldDraw;

    if (fieldDrawStr.size() < FIELD_WIDTH_DEFAULT*FIELD_HEIGHT_DEFAULT)
    {
        qDebug() << "Wrong fieldDrawStr size";
        return fieldDraw;
    }

    for(QString::iterator cell_it = fieldDrawStr.begin(); cell_it != fieldDrawStr.end(); ++cell_it)
    {
        if (cell_it->digitValue() < (int)CellDraw::CELL_EMPTY || cell_it->digitValue() > (int)CellDraw::CELL_MARK)
        {
            qDebug() << "fieldDrawFromStr(str): wrong string!";
            fieldDraw.clear();
            break;
        }

        fieldDraw.push_back((CellDraw)cell_it->digitValue());
    }

    return fieldDraw;
}

void Field::setStateField(QString field)
{
    fieldState_.clear();

    for(QString::iterator cell_it = field.begin(); cell_it != field.end(); ++cell_it)
    {
        if (cell_it->digitValue() < (int)CL_ST_EMPTY || cell_it->digitValue() > (int)CL_ST_UNDEFINED)
        {
            qDebug() << "setStateField(str): wrong string!";
            fieldState_.clear();
            return;
        }

        fieldState_.push_back((CellState)cell_it->digitValue());
    }
}

void Field::setDrawField(QString field)
{
    if (field.size() != area_)
        return;

    fieldDraw_.clear();

    for(QString::iterator cell_it = field.begin(); cell_it != field.end(); ++cell_it)
    {
        if (cell_it->digitValue() < (int)CELL_EMPTY || cell_it->digitValue() > (int)CELL_MARK)
        {
            qDebug() << "setStateField(str): wrong string!";
            fieldDraw_.clear();
            return;
        }

        fieldDraw_.push_back((CellDraw)cell_it->digitValue());
    }
}

void Field::setStateField(QVector<CellState> field)
{
    if (field.size() != area_)
        return;

    fieldState_ = field;
}

void Field::setDrawField(QVector<CellDraw> field)
{
    if (field.size() != area_)
        return;

    qDebug() << "setDrawField: " << getDrawFieldStr();

    fieldDraw_ = field;
}

void Field::initMyDrawField()
{
    fieldDraw_.clear();

    for(int i = 0; i < area_; i++)
    {
        if (fieldState_[i] == CL_ST_EMPTY)
            fieldDraw_.push_back(CELL_EMPTY);

        else
            fieldDraw_.push_back(CELL_LIVE);
    }
}

void Field::clear()
{
    fieldDraw_.fill(CELL_EMPTY, area_);
    fieldState_.fill(CL_ST_EMPTY, area_);
}

int Field::getWidth() const
{
    return width_;
}

int Field::getHeight() const
{
    return height_;
}

int Field::getArea() const
{
    return area_;
}

QImage Field::getFieldImage()
{
    QImage image(FIELD_IMG_WIDTH_DEFAULT, FIELD_IMG_HEIGHT_DEFAULT, QImage::Format_ARGB32);
    CellDraw cell;
    image.fill(0);  // empty image
    QPainter painter(&image);

    double cfx = 1.0 * FIELD_IMG_WIDTH_DEFAULT /FIELD_WIDTH_DEFAULT ;
    double cfy = 1.0 * FIELD_IMG_HEIGHT_DEFAULT/FIELD_HEIGHT_DEFAULT;

    for(int i = 0; i < width_; i++)
    {
        for(int j = 0; j < height_; j++)
        {
            cell = getCell(i, j);

            int x = i*cfx;
            int y = j*cfy;

            switch(cell)
            {
            case CELL_DOT:
            {
                painter.drawImage(x, y, pictures.get("dot"));
                break;
            }

            case CELL_LIVE:
            {
                painter.drawImage(x, y, pictures.get("live"));
                break;
            }

            case CELL_DAMAGED:
            {
                painter.drawImage(x, y, pictures.get("damaged"));
                break;
            }

            case CELL_KILLED:
            {
                painter.drawImage(x, y, pictures.get("killed"));
                break;
            }

            case CELL_MARK:
            {
                painter.drawImage(x, y+1, pictures.get("flag"));
                break;
            }

            default:
                break;
            }
        }
    }

    painter.end();

    return image;
}

void Field::generate()
{
    qDebug() << "\"generate\" clicked: Generating new field";

    // TODO: add generated fields and atabase

    QString field_example = "8888088800"\
                            "0000000000"\
                            "8880880880"\
                            "0000000000"\
                            "8808080808"\
                            "0000000000"\
                            "0000000000"\
                            "0000000000"\
                            "0000000000"\
                            "0000000000";   // example for testing

    setStateField(field_example);
    initMyDrawField();

    qDebug() << "Generated field (state): " + getStateFieldStr();
    qDebug() << "Generated field (draw ): " + getDrawFieldStr();
}

void printField(const QVector<CellState>& field)
{
    int width = sqrt(field.size());
    int height = width;

    QDebug debugOut = qDebug();
    debugOut << "\n";
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
           debugOut << (int)(field[i*width+j]) << " ";
        }
        debugOut << "\n";
    }
}

void printField(const QVector<CellDraw>& field)
{
    int width = sqrt(field.size());
    int height = width;

    QDebug debugOut = qDebug();
    debugOut << "\n";
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
           debugOut << field[i*width+j] << " ";
        }
        debugOut << "\n";
    }
}

int Field::shipNum(int size, const QVector<CellState>& fieldStateWithBorders)  // checks the number of ships with <size> blocks
{
    if (fieldStateWithBorders.size() != (height_+2)*(width_+2))
    {
        qDebug() << "Wrong size of the fieldStateWithBorders :" << fieldStateWithBorders.size() << " instead of " << (height_+2)*(width_+2);
        return false;
    }

    int shipNumber = 0;

    for(int j = 0; j < width_+2; j++)
    {
        for(int i = 0; i < height_+2; i++)
        {
            if(isShip(size, i, j, fieldStateWithBorders))
            {
                shipNumber++;
                qDebug() << "найден" << shipNumber << "й корабль размера size =" << size;
            }
        }
    }

    return shipNumber;
}

bool Field::isShip(int size, int x, int y, const QVector<CellState>& fieldStateWithBorders)
{
    if (size < 0 || size > SHIP_MAXLEN)
        return false;

    int index = (width_+2)*y+x;
    int length = 0;
    int delta = 0;

    if (fieldStateWithBorders[index] == CL_ST_TOP)
    {
        delta = width_+2;

        index += delta;
        length = 1;

        while (fieldStateWithBorders[index] != CL_ST_EMPTY)
        {
            length++;
            index += delta;
        }
    }

    else if (fieldStateWithBorders[index] == CL_ST_LEFT)
    {
        delta = 1;

        index += delta;
        length = 1;

        while (fieldStateWithBorders[index] != CL_ST_EMPTY)
        {
            length++;
            index += delta;
        }
    }

    else if (fieldStateWithBorders[index] == CL_ST_CENTER)
    {
        length = 1;
    }

    if (length == size)
        return true;

    return false;
}

bool Field::CheckDiagonalCollisions(const QVector<CellState>& fieldStateWithBorders)
{
    if (fieldStateWithBorders.size() != (height_+2)*(width_+2))
    {
        qDebug() << "Wrong size of the fieldStateWithBorders :" << fieldStateWithBorders.size() << " instead of " << (height_+2)*(width_+2);
        return false;
    }

    for(int i = 0; i < height_; i++)
    {
        for(int j = 0; j < width_; j++)
        {
            int index = (width_+2)*(i+1)+(j+1);
            if(fieldStateWithBorders[index] == CL_ST_EMPTY)
            {
                continue;
            }

            if(fieldStateWithBorders[index - (width_ + 2) - 1] != CL_ST_EMPTY ||
               fieldStateWithBorders[index - (width_ + 2) + 1] != CL_ST_EMPTY ||
               fieldStateWithBorders[index + (width_ + 2) + 1] != CL_ST_EMPTY ||
               fieldStateWithBorders[index + (width_ + 2) - 1] != CL_ST_EMPTY   )
            {
                return false;
            }
        }
    }

    return true;
}

bool Field::CheckLength(QVector<CellState>& fieldStateWithBorders) // тут если чё нихуя правильно не работает, надеюсь бы отдебажим как проснемся :))
{
    if (fieldStateWithBorders.size() != (height_+2)*(width_+2))
    {
        qDebug() << "Wrong size of the fieldStateWithBorders :" << fieldStateWithBorders.size() << " instead of " << (height_+2)*(width_+2);
        return false;
    }

    for(int i = 0; i < height_; i++)
    {
        for(int j = 0; j < width_; j++)
        {
            int index = (width_+2)*(i + 1) + (j+1);
            if(fieldStateWithBorders[index] == CL_ST_UNDEFINED)
            {
                if(fieldStateWithBorders[index+1] != CL_ST_EMPTY) //horizontal ship (go from left toright)
                {
                    fieldStateWithBorders[index] = CL_ST_LEFT;

                    int length = 2;
                    int delta = 1;
                    index += delta;

                    while(fieldStateWithBorders[index+delta] != CL_ST_EMPTY)
                    {
                         fieldStateWithBorders[index] = CL_ST_HMIDDLE;
                         index += delta;
                         length++;
                    }

                    fieldStateWithBorders[index] = CL_ST_RIGHT;

                    qDebug() << "length of the horizontal ship: " << length;

                    if(length > SHIP_MAXLEN)
                        return false;

                    continue;
                }

                if(fieldStateWithBorders[index+width_+2] != CL_ST_EMPTY) // vertical ship (go from up to down)
                {
                    fieldStateWithBorders[index] = CL_ST_TOP;

                    int length = 2;
                    int delta = width_+2;
                    index += delta;

                    while(fieldStateWithBorders[index+delta] != CL_ST_EMPTY)
                    {
                         fieldStateWithBorders[index] = CL_ST_VMIDDLE;
                         index += delta;
                         length++;
                    }

                    fieldStateWithBorders[index] = CL_ST_BOTTOM;

                    qDebug() << "length of the vertical ship: " << length;

                    if(length > SHIP_MAXLEN)
                        return false;

                    continue;
                 }

                 fieldStateWithBorders[index] = CL_ST_CENTER;
                 qDebug() << "length of the ship: 1";
            }
        }
    }

    printField(fieldStateWithBorders);

    return true;
}

/**
 * @brief Проверка корректности размещения кораблей.
 * @return true если размещение корректно, false в противном случае
 * 
 * Проверяет корректность размещения кораблей на поле согласно правилам игры:
 * - Корабли не должны соприкасаться
 * - Корабли должны быть размещены в пределах поля
 * - Количество кораблей каждого типа должно соответствовать правилам
 */
bool Field::isCorrect()
{
    qDebug() << "isCorrect() input field dump::";
    printField(fieldState_);

    QVector<CellState> fieldStateWithBorders((width_+2)*(height_+2));
    fieldStateWithBorders.fill(CL_ST_EMPTY);

    for(int i = 0; i < height_; i++)
    {
        for(int j = 0; j < width_; j++)
        {
            fieldStateWithBorders[(width_+2)*(i+1)+(j+1)] = fieldState_[width_*i+j];
        }
    }

    qDebug() << "Copied field dump:";
    printField(fieldStateWithBorders);

    if(CheckDiagonalCollisions(fieldStateWithBorders) == false)
    {
        return false;
    }

    if(CheckLength(fieldStateWithBorders) == false)
    {
        return false;
    }

    qDebug() << "Copied field dump after CheckLength():";
    printField(fieldStateWithBorders);

    return  (shipNum(1, fieldStateWithBorders) == SHIP1_NUM &&
             shipNum(2, fieldStateWithBorders) == SHIP2_NUM &&
             shipNum(3, fieldStateWithBorders) == SHIP3_NUM &&
             shipNum(4, fieldStateWithBorders) == SHIP4_NUM   );
}
